/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
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
#include <ddr.h>
#include <pwrc.h>
#include <xspi.h>
#include <rz_private.h>


#if PLAT_SYSTEM_SUSPEND
uint32_t ddr_params[RZG3S_DDR_CONFIG_SIZE / sizeof(uint32_t)];

static image_info_t ddr_params_info = {
	.h.type = (uint8_t)PARAM_IMAGE_BINARY,
	.h.version = (uint8_t)VERSION_2,
	.h.size = (uint16_t)sizeof(image_info_t),
	.h.attr = 0,
	.image_max_size = sizeof(ddr_params),
	.image_base = (uintptr_t)&ddr_params
};

static int save_ddr_params(unsigned int image_id, image_info_t *image_data)
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

	INFO("Saving image id=%u\n", image_id);

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

	INFO("Image id=%u saved\n", image_id);

exit:
	(void)io_close(image_handle);

	(void)io_dev_close(dev_handle);

	return io_result;
}

void plat_ddr_setup(void)
{
	if (!pwrc_is_ddr_retention_mode())
	{
#if !DEBUG_FPGA
		ddr_setup();
#endif
		if (0 != save_ddr_params(DDR_CONFIG_ID, &ddr_params_info)) {
			ERROR("PLAT: Failed to save DDR retention parameters.\n");
			panic();
		}
	}
	else
	{
		if (0 != load_auth_image(DDR_CONFIG_ID, &ddr_params_info)) {
			ERROR("PLAT: Failed to load DDR retention parameters.\n");
			panic();
		}
#if !DEBUG_FPGA
		// TODO: ddr_resume();
#endif
	}
}
#else
void plat_ddr_setup(void)
{
	ddr_setup();
}
#endif /* PLAT_DDR_RETENTION */
