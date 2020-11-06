/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>

#include <platform_def.h>

#include <common/debug.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_semihosting.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_fip.h>
#include <drivers/io/io_memmap.h>

#include "io_common.h"
#include "io_private.h"
#include "rzg2l_def.h"

static uintptr_t memdrv_dev_handle;
static uintptr_t fip_dev_handle;

static uintptr_t boot_io_drv_id;

static const io_block_spec_t rzg2l_block_spec = {
	.offset = FLASH0_BASE,
	.length = FLASH0_SIZE
};

static const io_block_spec_t bl31_file_spec = {
	.offset = BL31_IMAGE_ID,
};

static const io_block_spec_t bl33_file_spec = {
	.offset = BL33_IMAGE_ID,
};

static int32_t open_memmap(const uintptr_t spec);
static int32_t open_fipdrv(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy policies[] = {
	[FIP_IMAGE_ID] = {
			  &memdrv_dev_handle,
			  (uintptr_t) &rzg2l_block_spec,
			  &open_memmap},
	[BL31_IMAGE_ID] = {
			   &fip_dev_handle,
			   (uintptr_t) &bl31_file_spec,
			   &open_fipdrv},
	[BL33_IMAGE_ID] = {
			   &fip_dev_handle,
			   (uintptr_t) &bl33_file_spec,
			   &open_fipdrv},
	{ 0, 0, 0}
};

static int32_t open_fipdrv(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;
	
	result = io_dev_init(fip_dev_handle, boot_io_drv_id);
	if (result != IO_SUCCESS)
		return result;

	result = io_open(fip_dev_handle, spec, &handle);
	if (result == IO_SUCCESS)
		io_close(handle);

	return result;
}

static int32_t open_memmap(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(memdrv_dev_handle, 0);
	if (result != IO_SUCCESS)
		return result;

	result = io_open(memdrv_dev_handle, spec, &handle);
	if (result == IO_SUCCESS)
		io_close(handle);

	return result;
}

void rzg2l_io_setup(void)
{
	const io_dev_connector_t *memmap;
	const io_dev_connector_t *rzg2l;

	boot_io_drv_id = FIP_IMAGE_ID;

	register_io_dev_fip(&rzg2l);
	register_io_dev_memmap(&memmap);
	io_dev_open(rzg2l, 0, &fip_dev_handle);
	io_dev_open(memmap, 0, &memdrv_dev_handle);
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
				uintptr_t *image_spec)
{
	const struct plat_io_policy *policy;
	int result;

	policy = &policies[image_id];

	result = policy->check(policy->image_spec);
	if (result != IO_SUCCESS)
		return result;

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return IO_SUCCESS;
}

