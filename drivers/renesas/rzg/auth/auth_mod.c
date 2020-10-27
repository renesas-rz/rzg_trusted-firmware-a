/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <plat/common/platform.h>

static int32_t (*secure_boot_api)(void);

int auth_mod_get_parent_id(unsigned int img_id, unsigned int *parent_id)
{
	return 1;
}

int auth_mod_verify_img(unsigned int img_id, void *ptr, unsigned int len)
{
	int32_t ret = 0;

#if IMAGE_BL2 && RZG2_SECURE_BOOT

	switch (img_id) {
	case BL31_IMAGE_ID:
	case BL32_IMAGE_ID:
	case BL33_IMAGE_ID:
	case BL332_IMAGE_ID:
	case BL333_IMAGE_ID:
	case BL334_IMAGE_ID:
	case BL335_IMAGE_ID:
	case BL336_IMAGE_ID:
	case BL337_IMAGE_ID:
	case BL338_IMAGE_ID:
		return ret;

	default:
		return -1;
	}
#endif
	return ret;
}

static int32_t normal_boot_verify(void)
{
#if RZG2_SECURE_BOOT
	return -1;
#else
	return 0;
#endif
}

void auth_mod_init(void)
{
#if RZG2_SECURE_BOOT
	NOTICE("BL2: Secure boot\n");
	secure_boot_api = &normal_boot_verify;
#else
	NOTICE("BL2: Normal boot\n");
	secure_boot_api = &normal_boot_verify;
#endif
}
