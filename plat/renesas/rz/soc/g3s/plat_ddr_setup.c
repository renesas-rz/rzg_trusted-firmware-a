/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <arch.h>
#include <arch_features.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/auth/auth_mod.h>
#include <drivers/io/io_storage.h>
#include <lib/utils.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include <plat/common/platform.h>
#include <rz_soc_private.h>
#include <sys.h>
#include <ddr.h>

uint32_t ddr_csr_table[RET_CSR_SIZE] __aligned(8);

#if PLAT_SYSTEM_SUSPEND
static image_info_t ddr_config_info = {
	.h.type = (uint8_t)PARAM_IMAGE_BINARY,
	.h.version = (uint8_t)VERSION_2,
	.h.size = (uint16_t)sizeof(image_info_t),
	.h.attr = 0,
	.image_max_size = sizeof(ddr_csr_table),
	.image_base = (uintptr_t)&ddr_csr_table
};

static int save_ddr_config(unsigned int image_id, image_info_t *image_data)
{
	uintptr_t dev_handle;
	uintptr_t image_handle;
	uintptr_t image_spec;
	uintptr_t image_base;
	size_t image_size;
	size_t bytes_write;
	int io_result;

	assert(image_data != NULL);
	assert(image_data->h.version >= VERSION_2);

	image_base = image_data->image_base;

	io_result = plat_get_image_source(image_id, &dev_handle, &image_spec);
	if (io_result != 0) {
		WARN("Failed to obtain reference to image id=%u (%i)\n",
			image_id, io_result);
		return io_result;
	}

	io_result = io_open(dev_handle, image_spec, &image_handle);
	if (io_result != 0) {
		WARN("Failed to access image id=%u (%i)\n",
			image_id, io_result);
		return io_result;
	}

	INFO("Saving DDR retention info.\n");

	io_result = io_size(image_handle, &image_size);
	if ((io_result != 0) || (image_size == 0U)) {
		WARN("Failed to determine the size of the image id=%u (%i)\n",
			image_id, io_result);
		if ((io_result == 0) && (image_size == 0U))
			io_result = -EIO;
		goto exit;
	}

	if (image_size > image_data->image_max_size) {
		WARN("Image id=%u size out of bounds\n", image_id);
		io_result = -EFBIG;
		goto exit;
	}

	image_data->image_size = (uint32_t)image_size;

	io_result = io_write(image_handle, image_base, image_size, &bytes_write);
	if ((io_result != 0) || (bytes_write < image_size)) {
		WARN("Failed to save image id=%u (%i)\n", image_id, io_result);
		goto exit;
	}

	INFO("DDR Retention Info saved.\n");

exit:
	(void)io_close(image_handle);

	(void)io_dev_close(dev_handle);

	return io_result;
}

void plat_ddr_setup(void)
{
	if (!sys_is_resume_reboot()) {
		ddr_setup();

		if (save_ddr_config(DDR_CONFIG_ID, &ddr_config_info) != 0) {
			ERROR("Failed to save DDR retention info.\n");
			panic();
		}
	} else {
		if (load_auth_image(DDR_CONFIG_ID, &ddr_config_info) != 0) {
			ERROR("Failed to load DDR retention info.\n");
			panic();
		}

		ddr_retention_exit();
	}
}
#else
void plat_ddr_setup(void)
{
	ddr_setup();
}
#endif /* PLAT_SYSTEM_SUSPEND */
