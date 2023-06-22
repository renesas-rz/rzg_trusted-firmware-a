/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include <common/debug.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_semihosting.h>

#include "io_common.h"
#include "io_memdrv.h"
#include "io_emmcdrv.h"
#include "io_private.h"
#include "io_rcar.h"
#include <platform_def.h>

static uintptr_t emmcdrv_dev_handle;
static uintptr_t memdrv_dev_handle;
static uintptr_t rcar_dev_handle;

static uintptr_t boot_io_drv_id;

static const io_block_spec_t rcar_block_spec = {
	.offset = FLASH0_BASE,
	.length = FLASH0_SIZE
};

static const io_block_spec_t bl2_file_spec = {
	.offset = BL2_IMAGE_ID,
};

static const io_block_spec_t bl31_file_spec = {
	.offset = BL31_IMAGE_ID,
};

static const io_block_spec_t bl32_file_spec = {
	.offset = BL32_IMAGE_ID,
};

static const io_block_spec_t bl33_file_spec = {
	.offset = BL33_IMAGE_ID,
};

static const io_block_spec_t bl332_file_spec = {
	.offset = BL332_IMAGE_ID,
};

static const io_block_spec_t bl333_file_spec = {
	.offset = BL333_IMAGE_ID,
};

static const io_block_spec_t bl334_file_spec = {
	.offset = BL334_IMAGE_ID,
};

static const io_block_spec_t bl335_file_spec = {
	.offset = BL335_IMAGE_ID,
};

static const io_block_spec_t bl336_file_spec = {
	.offset = BL336_IMAGE_ID,
};

static const io_block_spec_t bl337_file_spec = {
	.offset = BL337_IMAGE_ID,
};

static const io_block_spec_t bl338_file_spec = {
	.offset = BL338_IMAGE_ID,
};

static int32_t open_emmcdrv(const uintptr_t spec);
static int32_t open_memmap(const uintptr_t spec);
static int32_t open_rcar(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static struct plat_io_policy policies[] = {
	[FIP_IMAGE_ID] = {
			  &memdrv_dev_handle,
			  (uintptr_t) &rcar_block_spec,
			  &open_memmap},
	[BL2_IMAGE_ID] = {
			  &rcar_dev_handle,
			  (uintptr_t) &bl2_file_spec,
			  &open_rcar},
	[BL31_IMAGE_ID] = {
			   &rcar_dev_handle,
			   (uintptr_t) &bl31_file_spec,
			   &open_rcar},
	[BL32_IMAGE_ID] = {
			   &rcar_dev_handle,
			   (uintptr_t) &bl32_file_spec,
			   &open_rcar},
	[BL33_IMAGE_ID] = {
			   &rcar_dev_handle,
			   (uintptr_t) &bl33_file_spec,
			   &open_rcar},
	[BL332_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl332_file_spec,
			    &open_rcar},
	[BL333_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl333_file_spec,
			    &open_rcar},
	[BL334_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl334_file_spec,
			    &open_rcar},
	[BL335_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl335_file_spec,
			    &open_rcar},
	[BL336_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl336_file_spec,
			    &open_rcar},
	[BL337_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl337_file_spec,
			    &open_rcar},
	[BL338_IMAGE_ID] = {
			    &rcar_dev_handle,
			    (uintptr_t) &bl338_file_spec,
			    &open_rcar},
#if RZG2_SECURE_BOOT
	[SEC_KEYRING_ID] = {
				0, 0, 0}, {
#else
					{
#endif
				0, 0, 0}
};

static io_drv_spec_t io_drv_spec_memdrv = {
	FLASH0_BASE,
	FLASH0_SIZE,
	0,
};

static io_drv_spec_t io_drv_spec_emmcdrv = {
	0,
	0,
	0,
};

static struct plat_io_policy drv_policies[] __attribute__ ((section(".data"))) = {
	/* FLASH_DEV_ID */
	{ &memdrv_dev_handle, (uintptr_t) &io_drv_spec_memdrv, &open_memmap, },
	/* EMMC_DEV_ID */
	{ &emmcdrv_dev_handle, (uintptr_t) &io_drv_spec_emmcdrv, &open_emmcdrv, }
};

#if RZG2_SECURE_BOOT
static io_drv_spec_t io_key_spec_memdrv = { FLASH0_BASE + KEYRING_OFFSET, KEYRING_LENGTH, 0 };

static io_drv_spec_t io_key_spec_emmcdrv = { KEYRING_OFFSET, KEYRING_LENGTH, 1 };

static struct plat_io_policy key_policies[] = {
	/* FLASH_DEV_ID */
	{ &memdrv_dev_handle, (uintptr_t) &io_key_spec_memdrv, &open_memmap },
	/* EMMC_DEV_ID */
	{ &emmcdrv_dev_handle, (uintptr_t) &io_key_spec_emmcdrv, &open_emmcdrv }
};
#endif /* RZG2_SECURE_BOOT */

static int32_t open_rcar(const uintptr_t spec)
{
	return io_dev_init(rcar_dev_handle, boot_io_drv_id);
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

static int32_t open_emmcdrv(const uintptr_t spec)
{
	return io_dev_init(emmcdrv_dev_handle, 0);
}

void rcar_io_setup(void)
{
	const io_dev_connector_t *memmap;
	const io_dev_connector_t *rcar;

	boot_io_drv_id = FLASH_DEV_ID;

	rcar_register_io_dev(&rcar);
	rcar_register_io_dev_memdrv(&memmap);
	io_dev_open(rcar, 0, &rcar_dev_handle);
	io_dev_open(memmap, 0, &memdrv_dev_handle);

#if RZG2_SECURE_BOOT
	policies[SEC_KEYRING_ID] = key_policies[boot_io_drv_id];
	io_dev_init(rcar_dev_handle, boot_io_drv_id);
#endif
}

void rcar_io_emmc_setup(void)
{
	const io_dev_connector_t *rcar;
	const io_dev_connector_t *emmc;

	boot_io_drv_id = EMMC_DEV_ID;

	rcar_register_io_dev(&rcar);
	rcar_register_io_dev_emmcdrv(&emmc);
	io_dev_open(rcar, 0, &rcar_dev_handle);
	io_dev_open(emmc, 0, &emmcdrv_dev_handle);

#if RZG2_SECURE_BOOT
	policies[SEC_KEYRING_ID] = key_policies[boot_io_drv_id];
	io_dev_init(rcar_dev_handle, boot_io_drv_id);
#endif
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

int32_t plat_get_drv_source(uint32_t io_drv_id, uintptr_t *dev_handle,
			    uintptr_t *image_spec)
{
	const struct plat_io_policy *policy;
	int32_t result;

	policy = &drv_policies[io_drv_id];

	result = policy->check(policy->image_spec);
	if (result != IO_SUCCESS)
		return result;

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return IO_SUCCESS;
}
