/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_fip.h>
#include <drivers/io/io_memmap.h>
#include <io_common.h>
#include <io_emmcdrv.h>
#include <io_sddrv.h>
#include <lib/mmio.h>
#include <tools_share/firmware_image_package.h>
#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
#include <plat_tbbr_img_def.h>
#include <io_xspidrv.h>
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */

#include <rz_soc_def.h>
#include <sys.h>
#if PLAT_SOC_RZG2L
#include <spi_multi.h>
#else
#include <xspi.h>
#endif
#include <emmc_def.h>

#include <sys_regs.h>


static uintptr_t fip_dev_handle;
static uintptr_t memdrv_dev_handle;
#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
static uintptr_t emmcdrv_dev_handle;
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */
static uintptr_t sddrv_dev_handle;
#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
static uintptr_t xspidrv_dev_handle;
#endif

static uintptr_t boot_io_drv_id;

static const io_block_spec_t spirom_block_spec = {
	.offset = RZ_SOC_SPIROM_FIP_BASE,
	.length = RZ_SOC_SPIROM_FIP_SIZE,
};

#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
static const io_drv_spec_t emmc_block_spec = {
	.offset = RZ_SOC_EMMC_FIP_BASE,
	.length = RZ_SOC_EMMC_FIP_SIZE,
};
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */

static const io_drv_spec_t sd_block_spec = {
	.offset = RZ_SOC_SD_FIP_BASE,
	.length = RZ_SOC_SD_FIP_SIZE,
};

static const io_uuid_spec_t bl31_file_spec = {
	.uuid = UUID_EL3_RUNTIME_FIRMWARE_BL31,
};

static const io_uuid_spec_t bl32_file_spec = {
	.uuid = UUID_SECURE_PAYLOAD_BL32,
};

static const io_uuid_spec_t bl33_file_spec = {
	.uuid = UUID_NON_TRUSTED_FIRMWARE_BL33,
};

#if TRUSTED_BOARD_BOOT
static const io_uuid_spec_t soc_fw_key_cert_file_spec = {
	.uuid = UUID_SOC_FW_KEY_CERT,
};

static const io_uuid_spec_t soc_fw_content_cert_file_spec = {
	.uuid = UUID_SOC_FW_CONTENT_CERT,
};

static const io_uuid_spec_t tos_fw_key_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_KEY_CERT,
};

static const io_uuid_spec_t tos_fw_content_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_CONTENT_CERT,
};

static const io_uuid_spec_t nt_fw_key_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_KEY_CERT,
};

static const io_uuid_spec_t nt_fw_content_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_CONTENT_CERT,
};
#endif

#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
static const io_block_spec_t spirom_ddr_cfg_spec = {
	.offset = RZ_SOC_SPIROM_DDR_CFG_BASE,
	.length = RZ_SOC_SPIROM_DDR_CFG_SIZE,
};
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */

#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
static int32_t open_emmcdrv(const uintptr_t spec);
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */
static int32_t open_memmap(const uintptr_t spec);
static int32_t open_fipdrv(const uintptr_t spec);
static int32_t open_sddrv(const uintptr_t spec);
#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
static int32_t open_xspidrv(const uintptr_t spec);
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy sd_fip_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_block_spec,
	&open_sddrv
};

#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
static const struct plat_io_policy emmc_fip_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_block_spec,
	&open_emmcdrv
};
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */

static const struct plat_io_policy spirom_fip_policy = {
	&memdrv_dev_handle,
	(uintptr_t) &spirom_block_spec,
	&open_memmap
};

#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
static const struct plat_io_policy spirom_ddr_config_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_ddr_cfg_spec,
	&open_xspidrv
};
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */

static struct plat_io_policy policies[MAX_NUMBER_IDS] = {
	/* FIP_IMAGE_ID structure is added to this array on a bootmode basis */

	[BL31_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl31_file_spec,
				&open_fipdrv},
	[BL32_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl32_file_spec,
				&open_fipdrv},
	[BL33_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl33_file_spec,
				&open_fipdrv},
#if TRUSTED_BOARD_BOOT
	[SOC_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_key_cert_file_spec,
				&open_fipdrv},
	[SOC_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_content_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_key_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_content_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_key_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_content_cert_file_spec,
				&open_fipdrv},
#endif /* TRUSTED_BOARD_BOOT */
	{0, 0, 0}
};

static int32_t open_fipdrv(const uintptr_t spec)
{
	int32_t result;

	result = io_dev_init(fip_dev_handle, boot_io_drv_id);
	if (result != 0)
		return result;

	return result;
}

static int32_t open_memmap(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(memdrv_dev_handle, 0);
	if (result != 0)
		return result;

	result = io_open(memdrv_dev_handle, spec, &handle);
	if (result == 0)
		io_close(handle);

	return result;
}

#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
static int32_t open_emmcdrv(const uintptr_t spec)
{
	return io_dev_init(emmcdrv_dev_handle, 0);
}
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */

static int32_t open_sddrv(const uintptr_t spec)
{
	return io_dev_init(sddrv_dev_handle, 0);
}

#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
static int32_t open_xspidrv(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(xspidrv_dev_handle, 0);
	if (result != 0)
		return result;

	result = io_open(xspidrv_dev_handle, spec, &handle);
	if (result == 0)
		io_close(handle);

	return result;
}
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */

static void update_dev_policies(uint16_t boot_mode)
{
	switch (boot_mode) {
	case SYS_BOOT_MODE_SPI_1_8:
	case SYS_BOOT_MODE_SPI_3_3:
		policies[FIP_IMAGE_ID] = spirom_fip_policy;
#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
		policies[V2H_DDR_CONFIG_ID] = spirom_ddr_config_policy;
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */
		break;
#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
	case SYS_BOOT_MODE_EMMC_1_8:
	case SYS_BOOT_MODE_EMMC_3_3:
		policies[FIP_IMAGE_ID] = emmc_fip_policy;
		break;
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */
	case SYS_BOOT_MODE_ESD:
		policies[FIP_IMAGE_ID] = sd_fip_policy;
		break;
	default:
		panic();
	}
}

void rz_io_setup(void)
{
	const io_dev_connector_t *memmap;
#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
	const io_dev_connector_t *emmc;
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */
	const io_dev_connector_t *rzsoc;
	const io_dev_connector_t *sd;
#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
	const io_dev_connector_t *xspi;
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */
	boot_mode_t boot_mode;

	boot_mode = sys_get_boot_mode();

	boot_io_drv_id = FIP_IMAGE_ID;

	register_io_dev_fip(&rzsoc);

	io_dev_open(rzsoc, 0, &fip_dev_handle);

	if (boot_mode == SYS_BOOT_MODE_SPI_1_8 ||
		boot_mode == SYS_BOOT_MODE_SPI_3_3) {
#if PLAT_SOC_RZG2L
		spi_multi_setup();
#else
		xspi_setup();
#endif /* PLAT_SOC_RZG2L */
		register_io_dev_memmap(&memmap);
		io_dev_open(memmap, 0, &memdrv_dev_handle);

#if (PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H)
		register_io_dev_xspidrv(&xspi);
		io_dev_open(xspi, 0, &xspidrv_dev_handle);
#endif /* PLAT_SYSTEM_SUSPEND && PLAT_SOC_RZV2H */
#ifndef BOOT_MODE_eMMC_NOT_SUPPORTED
	} else if  (boot_mode == SYS_BOOT_MODE_EMMC_1_8 ||
				boot_mode == SYS_BOOT_MODE_EMMC_3_3) {
		if (emmc_init() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC driver initialize.\n");
			panic();
		}
		emmc_memcard_power(EMMC_POWER_ON);
		if (emmc_mount() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC mount operation.\n");
			panic();
		}

		register_io_dev_emmcdrv(&emmc);
		io_dev_open(emmc, 0, &emmcdrv_dev_handle);
#endif /* BOOT_MODE_eMMC_NOT_SUPPORTED */
	} else if (boot_mode == SYS_BOOT_MODE_ESD) {
		register_io_dev_sddrv(&sd);
		io_dev_open(sd, 0, &sddrv_dev_handle);
	} else {
		ERROR("Unsupported IO device %d.\n", boot_mode);
		panic();
	}

	update_dev_policies(boot_mode);
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
				uintptr_t *image_spec)
{
	const struct plat_io_policy *policy;
	int result;

	policy = &policies[image_id];

	result = policy->check(policy->image_spec);
	if (result != 0)
		return result;

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return 0;
}

