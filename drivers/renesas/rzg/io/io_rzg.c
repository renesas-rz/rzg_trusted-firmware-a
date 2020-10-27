/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdint.h>
#include <string.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <lib/mmio.h>
#include <plat/common/platform.h>
#include <tools_share/firmware_image_package.h>
#include <tools_share/uuid.h>

#include "io_rzg.h"
#include "io_common.h"
#include "io_private.h"

extern int32_t plat_get_drv_source(uint32_t id, uintptr_t *dev,
				   uintptr_t *image_spec);

extern int auth_mod_verify_img(unsigned int img_id, void *ptr,
				unsigned int len);

static int32_t rzg_dev_open(const uintptr_t dev_spec __attribute__ ((unused)),
			     io_dev_info_t **dev_info);
static int32_t rzg_dev_close(io_dev_info_t *dev_info);

typedef struct {
	const int32_t name;
	const uint32_t offset;
	const uint32_t attr;
} plat_rzg_name_offset_t;

typedef struct {
	/* Put position above the struct to allow {0} on static init.
	 * It is a workaround for a known bug in GCC
	 * http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53119
	 */
	uint32_t position;
	uint32_t no_load;
	uintptr_t offset;
	uint32_t size;
	uintptr_t dst;
	uintptr_t partition;	/* for eMMC */
	/* RZG_EMMC_PARTITION_BOOT_0 */
	/* RZG_EMMC_PARTITION_BOOT_1 */
	/* RZG_EMMC_PARTITION_USER   */
} file_state_t;

#define RZG_GET_FLASH_ADR(a, b)	((uint32_t)((0x40000U * (a)) + (b)))
#define RZG_ATTR_SET_CALCADDR(a)	((a) & 0xF)
#define RZG_ATTR_SET_ISNOLOAD(a)	(((a) & 0x1) << 16U)
#define RZG_ATTR_SET_CERTOFF(a)	(((a) & 0xF) << 8U)
#define RZG_ATTR_SET_ALL(a, b, c)	((uint32_t)(RZG_ATTR_SET_CALCADDR(a) |\
					RZG_ATTR_SET_ISNOLOAD(b) | 	\
					RZG_ATTR_SET_CERTOFF(c)))

#define RZG_ATTR_GET_CALCADDR(a)	((a) & 0xFU)
#define RZG_ATTR_GET_ISNOLOAD(a)	(((a) >> 16) & 0x1U)
#define RZG_ATTR_GET_CERTOFF(a)	((uint32_t)(((a) >> 8) & 0xFU))

#define RZG_MAX_BL3X_IMAGE		(8U)
#define RZG_SECTOR6_CERT_OFFSET	(0x400U)
#define RZG_SDRAM_certESS		(0x43F00000U)
#define RZG_CERT_SIZE			(0x800U)
#define RZG_CERT_INFO_SIZE_OFFSET	(0x264U)
#define RZG_CERT_INFO_DST_OFFSET	(0x154U)
#define RZG_CERT_LOAD			(1U)

#define RZG_FLASH_CERT_HEADER		RZG_GET_FLASH_ADR(6U, 0U)
#define RZG_EMMC_CERT_HEADER		(0x00030000U)

#define RZG_COUNT_LOAD_BL31		(0U)
#define RZG_COUNT_LOAD_BL33		(2U)

#define RZG_MAX_LOAD_IMAGE		(RZG_MAX_BL3X_IMAGE + RZG_COUNT_LOAD_BL33)

static const plat_rzg_name_offset_t name_offset[] = {

	{BL31_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL31,    0, RZG_COUNT_LOAD_BL31)  },

	/* BL3-2 is optional in the platform */
	{BL32_IMAGE_ID,  0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL31+1, 0, RZG_COUNT_LOAD_BL31+1)},

	/* BL33 is non trusted firmwarel in the platform */
	{BL33_IMAGE_ID,  0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33,   0, RZG_COUNT_LOAD_BL33)  },
	{BL332_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+1, 0, RZG_COUNT_LOAD_BL33+1)},
	{BL333_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+2, 0, RZG_COUNT_LOAD_BL33+2)},
	{BL334_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+3, 0, RZG_COUNT_LOAD_BL33+3)},
	{BL335_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+4, 0, RZG_COUNT_LOAD_BL33+4)},
	{BL336_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+5, 0, RZG_COUNT_LOAD_BL33+5)},
	{BL337_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+6, 0, RZG_COUNT_LOAD_BL33+6)},
	{BL338_IMAGE_ID, 0U, RZG_ATTR_SET_ALL(RZG_COUNT_LOAD_BL33+7, 0, RZG_COUNT_LOAD_BL33+7)},
};

static file_state_t current_file = { 0 };

static uintptr_t rzg_handle, rzg_spec;
static uint64_t rzg_image_header[RZG_MAX_LOAD_IMAGE] = { 0U };
static uint64_t rzg_image_header_prttn[RZG_MAX_LOAD_IMAGE] = { 0U };
static uint64_t rzg_image_number = { 0U };
static uint32_t rzg_cert_load = { 0U };

static io_type_t device_type_rzg(void)
{
	return IO_TYPE_FIRMWARE_IMAGE_PACKAGE;
}

static int32_t file_to_offset(const int32_t name, uintptr_t *offset,
			      uint32_t *cert, uint32_t *no_load,
			      uintptr_t *partition)
{
	uint32_t addr;
	int32_t i;

	for (i = 0; i < ARRAY_SIZE(name_offset); i++) {
		if (name != name_offset[i].name)
			continue;

		addr = RZG_ATTR_GET_CALCADDR(name_offset[i].attr);
		if ((rzg_image_number + RZG_COUNT_LOAD_BL33) < addr)
			continue;

		*offset = rzg_image_header[addr];
		*cert = RZG_CERT_SIZE;
		*cert *= RZG_ATTR_GET_CERTOFF(name_offset[i].attr);
		*cert += RZG_SDRAM_certESS;
		*no_load = RZG_ATTR_GET_ISNOLOAD(name_offset[i].attr);
		*partition = rzg_image_header_prttn[addr];
		return IO_SUCCESS;
	}

	return -EINVAL;
}

void rzg_read_certificate(uint64_t cert, uint32_t *len, uintptr_t *dst)
{
	uintptr_t size, dsth, dstl;

	cert &= 0xFFFFFFFFU;

	size = cert + RZG_CERT_INFO_SIZE_OFFSET;
	*len = mmio_read_32(size) * 4U;
	dstl = cert + RZG_CERT_INFO_DST_OFFSET;
	dsth = dstl + 4U;
	*dst = ((uintptr_t) mmio_read_32(dsth) << 32) +
	    ((uintptr_t) mmio_read_32(dstl));
}

static int32_t check_load_area(uintptr_t dst, uintptr_t len)
{
	uint32_t legacy = dst + len <= UINT32_MAX - 1 ? 1 : 0;
	uintptr_t dram_start, dram_end;
	uintptr_t prot_start, prot_end;
	int32_t result = IO_SUCCESS;

	dram_start = legacy ? DRAM1_BASE : DRAM_40BIT_BASE;

	dram_end = legacy ? DRAM1_BASE + DRAM1_SIZE :
	    DRAM_40BIT_BASE + DRAM_40BIT_SIZE;

	prot_start = legacy ? DRAM_PROTECTED_BASE : DRAM_40BIT_PROTECTED_BASE;

	prot_end = prot_start + DRAM_PROTECTED_SIZE;

	if (dst < dram_start || dst > dram_end - len) {
		ERROR("BL2: dst address is on the protected area.\n");
		result = IO_FAIL;
		goto done;
	}

	/* load image is within SDRAM protected area */
	if (dst >= prot_start && dst < prot_end) {
		ERROR("BL2: dst address is on the protected area.\n");
		result = IO_FAIL;
	}

	if (dst < prot_start && dst > prot_start - len) {
		ERROR("BL2: loaded data is on the protected area.\n");
		result = IO_FAIL;
	}
done:
	if (result == IO_FAIL)
		ERROR("BL2: Out of range : dst=0x%lx len=0x%lx\n", dst, len);

	return result;
}

static int32_t load_bl33x(void)
{
	static int32_t loaded = IO_NOT_SUPPORTED;
	uintptr_t dst, partition, handle;
	uint32_t noload, cert, len, i;
	uintptr_t offset;
	int32_t rc;
	size_t cnt;
	const int32_t img[] = {
		BL33_IMAGE_ID,
		BL332_IMAGE_ID,
		BL333_IMAGE_ID,
		BL334_IMAGE_ID,
		BL335_IMAGE_ID,
		BL336_IMAGE_ID,
		BL337_IMAGE_ID,
		BL338_IMAGE_ID
	};

	if (loaded != IO_NOT_SUPPORTED)
		return loaded;

	for (i = 1; i < rzg_image_number; i++) {
		rc = file_to_offset(img[i], &offset, &cert, &noload,
				    &partition);
		if (rc != IO_SUCCESS) {
			WARN("load_bl33x: failed to get offset\n");
			loaded = IO_FAIL;
			return loaded;
		}

		rzg_read_certificate((uint64_t) cert, &len, &dst);
		((io_drv_spec_t *) rzg_spec)->partition = partition;

		rc = io_open(rzg_handle, rzg_spec, &handle);
		if (rc != IO_SUCCESS) {
			WARN("Failed to open FIP (%i)\n", rc);
			loaded = IO_FAIL;
			return loaded;
		}

		rc = io_seek(handle, IO_SEEK_SET, offset);
		if (rc != IO_SUCCESS) {
			WARN("load_bl33x: failed to seek\n");
			loaded = IO_FAIL;
			return loaded;
		}

		rc = check_load_area(dst, len);
		if (rc != IO_SUCCESS) {
			WARN("load_bl33x: check load area\n");
			loaded = IO_FAIL;
			return loaded;
		}

		rc = io_read(handle, dst, len, &cnt);
		if (rc != IO_SUCCESS) {
			WARN("load_bl33x: failed to read\n");
			loaded = IO_FAIL;
			return loaded;
		}
#if TRUSTED_BOARD_BOOT
		rc = auth_mod_verify_img(img[i], (void *)dst, len);
		if (rc) {
			memset((void *)dst, 0x00, len);
			loaded = IO_FAIL;
			return loaded;
		}
#endif
		io_close(handle);
	}

	loaded = IO_SUCCESS;

	return loaded;
}

static int32_t rzg_dev_init(io_dev_info_t *dev_info, const uintptr_t name)
{
	uint64_t header[64] __aligned(FLASH_TRANS_SIZE_UNIT) = {
	0};
	uintptr_t handle;
	ssize_t offset;
	uint32_t i;
	int32_t rc;
	size_t cnt;

	/* Obtain a reference to the image by querying the platform layer */
	rc = plat_get_drv_source(name, &rzg_handle, &rzg_spec);
	if (rc != IO_SUCCESS) {
		WARN("Failed to obtain reference to img %ld (%i)\n", name, rc);
		return IO_FAIL;
	}

	if (RZG_CERT_LOAD == rzg_cert_load)
		return IO_SUCCESS;

	rc = io_open(rzg_handle, rzg_spec, &handle);
	if (rc != IO_SUCCESS) {
		WARN("Failed to access img %ld (%i)\n", name, rc);
		return IO_FAIL;
	}

	/* get start address list   */
	/* [0] address num          */
	/* [1] BL33-1 image address */
	/* [2] BL33-2 image address */
	/* [3] BL33-3 image address */
	/* [4] BL33-4 image address */
	/* [5] BL33-5 image address */
	/* [6] BL33-6 image address */
	/* [7] BL33-7 image address */
	/* [8] BL33-8 image address */
	offset = name == EMMC_DEV_ID ? RZG_EMMC_CERT_HEADER :
	    RZG_FLASH_CERT_HEADER;
	rc = io_seek(handle, IO_SEEK_SET, offset);
	if (rc != IO_SUCCESS) {
		WARN("Firmware Image Package header failed to seek\n");
		goto error;
	}
#if RZG_BL2_DCACHE == 1
	inv_dcache_range((uint64_t) header, sizeof(header));
#endif
	rc = io_read(handle, (uintptr_t) &header, sizeof(header), &cnt);
	if (rc != IO_SUCCESS) {
		WARN("Firmware Image Package header failed to read\n");
		goto error;
	}

	rzg_image_number = header[0];
	for (i = 0; i < rzg_image_number + RZG_COUNT_LOAD_BL33; i++) {
		rzg_image_header[i] = header[i * 2 + 1];
		rzg_image_header_prttn[i] = header[i * 2 + 2];
	}

	if (rzg_image_number == 0 || rzg_image_number > RZG_MAX_BL3X_IMAGE) {
		WARN("Firmware Image Package header check failed.\n");
		goto error;
	}

	rc = io_seek(handle, IO_SEEK_SET, offset + RZG_SECTOR6_CERT_OFFSET);
	if (rc != IO_SUCCESS) {
		WARN("Firmware Image Package header failed to seek cert\n");
		goto error;
	}
#if RZG_BL2_DCACHE == 1
	inv_dcache_range(RZG_SDRAM_certESS,
			 RZG_CERT_SIZE * (RZG_COUNT_LOAD_BL33 + rzg_image_number));
#endif
	rc = io_read(handle, RZG_SDRAM_certESS,
		     RZG_CERT_SIZE * (RZG_COUNT_LOAD_BL33 + rzg_image_number), &cnt);
	if (rc != IO_SUCCESS) {
		WARN("cert file read error.\n");
		goto error;
	}

	rzg_cert_load = RZG_CERT_LOAD;
error:

	if (rc != IO_SUCCESS)
		rc = IO_FAIL;

	io_close(handle);

	return rc;

}

static int32_t rzg_file_open(io_dev_info_t *info, const uintptr_t file_spec,
			      io_entity_t *entity)
{
	const io_drv_spec_t *spec = (io_drv_spec_t *) file_spec;
	uintptr_t partition, offset, dst;
	uint32_t noload, cert, len;
	int32_t rc;

	/* Only one file open at a time. We need to  track state (ie, file
	 * cursor position). Since the header lives at * offset zero, this entry
	 * should never be zero in an active file.
	 * Once the system supports dynamic memory allocation we will allow more
	 * than one open file at a time. */
	if (current_file.offset != 0U) {
		WARN("rzg_file_open : Only one open file at a time.\n");
		return IO_RESOURCES_EXHAUSTED;
	}

	rc = file_to_offset(spec->offset, &offset, &cert, &noload, &partition);
	if (rc != IO_SUCCESS) {
		WARN("Failed to open file name %ld (%i)\n", spec->offset, rc);
		return IO_FAIL;
	}

	if (noload) {
		current_file.offset = 1;
		current_file.dst = 0;
		current_file.size = 1;
		current_file.position = 0;
		current_file.no_load = noload;
		current_file.partition = 0;
		entity->info = (uintptr_t) &current_file;

		return IO_SUCCESS;
	}

	rzg_read_certificate((uint64_t) cert, &len, &dst);

	/*----------------*
	 * Baylibre: HACK *
	 *----------------*/
	if (BL31_IMAGE_ID == spec->offset && len < RZG_TRUSTED_SRAM_SIZE) {
		WARN("rzg2 ignoring the BL31 size from certificate,"
		     "using RZG_TRUSTED_SRAM_SIZE instead\n");
		len = RZG_TRUSTED_SRAM_SIZE;
	}

	current_file.partition = partition;
	current_file.no_load = noload;
	current_file.offset = offset;
	current_file.position = 0;
	current_file.size = len;
	current_file.dst = dst;
	entity->info = (uintptr_t) &current_file;

	return IO_SUCCESS;
}

static int32_t rzg_file_len(io_entity_t *entity, size_t *length)
{
	*length = ((file_state_t *) entity->info)->size;

	NOTICE("%s: len: 0x%08lx\n", __func__, *length);

	return IO_SUCCESS;
}

static int32_t rzg_file_read(io_entity_t *entity, uintptr_t buffer,
			      size_t length, size_t *cnt)
{
	file_state_t *fp = (file_state_t *) entity->info;
	ssize_t offset = fp->offset + fp->position;
	uintptr_t handle;
	int32_t rc;

#ifdef SPD_NONE
	static uint32_t load_bl33x_counter = 1;
#else
	static uint32_t load_bl33x_counter;
#endif
	if (current_file.no_load) {
		*cnt = length;
		return IO_SUCCESS;
	}

	((io_drv_spec_t *) rzg_spec)->partition = fp->partition;

	rc = io_open(rzg_handle, rzg_spec, &handle);
	if (rc != IO_SUCCESS) {
		WARN("Failed to open FIP (%i)\n", rc);
		return IO_FAIL;
	}

	rc = io_seek(handle, IO_SEEK_SET, offset);
	if (rc != IO_SUCCESS) {
		WARN("rzg_file_read: failed to seek\n");
		goto error;
	}

	if (load_bl33x_counter == RZG_COUNT_LOAD_BL33) {
		rc = check_load_area(buffer, length);
		if (rc != IO_SUCCESS) {
			WARN("rzg_file_read: load area err\n");
			goto error;
		}
	}

	rc = io_read(handle, buffer, length, cnt);
	if (rc != IO_SUCCESS) {
		WARN("Failed to read payload (%i)\n", rc);
		goto error;
	}

	fp->position += *cnt;
	io_close(handle);

	load_bl33x_counter += 1;
	if (load_bl33x_counter == (RZG_COUNT_LOAD_BL33 + 1))
		return load_bl33x();

	return IO_SUCCESS;
error:
	io_close(handle);
	return IO_FAIL;
}

static int32_t rzg_file_close(io_entity_t *entity)
{
	if (current_file.offset)
		memset(&current_file, 0, sizeof(current_file));

	entity->info = 0U;

	return IO_SUCCESS;
}

static const io_dev_funcs_t rzg_dev_funcs = {
	.type = &device_type_rzg,
	.open = &rzg_file_open,
	.seek = NULL,
	.size = &rzg_file_len,
	.read = &rzg_file_read,
	.write = NULL,
	.close = &rzg_file_close,
	.dev_init = &rzg_dev_init,
	.dev_close = &rzg_dev_close,
};

static const io_dev_info_t rzg_dev_info = {
	.funcs = &rzg_dev_funcs,
	.info = (uintptr_t) 0
};

static const io_dev_connector_t rzg_dev_connector = {
	.dev_open = &rzg_dev_open
};

static int32_t rzg_dev_open(const uintptr_t dev_spec __attribute__ ((unused)),
			     io_dev_info_t **dev_info)
{
	*dev_info = (io_dev_info_t *) &rzg_dev_info;

	return IO_SUCCESS;
}

static int32_t rzg_dev_close(io_dev_info_t *dev_info)
{
	rzg_handle = 0;
	rzg_spec = 0;

	return IO_SUCCESS;
}

int32_t rzg_register_io_dev(const io_dev_connector_t **dev_con)
{
	int32_t result;

	result = io_register_device(&rzg_dev_info);
	if (result == IO_SUCCESS)
		*dev_con = &rzg_dev_connector;

	return result;
}
