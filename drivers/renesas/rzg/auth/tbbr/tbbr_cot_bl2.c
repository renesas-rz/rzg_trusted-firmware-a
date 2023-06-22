/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>

#include <platform_def.h>

#include <drivers/auth/auth_mod.h>
#include <drivers/auth/tbbr_cot_common.h>

static const auth_img_desc_t sec_keyring = {
	.img_id = SEC_KEYRING_ID,
	.img_type = IMG_RAW,
	.parent = NULL,
};

static const auth_img_desc_t bl31_image = {
	.img_id = BL31_IMAGE_ID,
	.img_type = IMG_RAW,
	.parent = &sec_keyring,
};

static const auth_img_desc_t bl32_image = {
	.img_id = BL32_IMAGE_ID,
	.img_type = IMG_RAW,
	.parent = &bl31_image,
};

static const auth_img_desc_t bl33_image = {
	.img_id = BL33_IMAGE_ID,
	.img_type = IMG_RAW,
# ifdef BL32_BASE
	.parent = &bl32_image,
# else
	.parent = &bl31_image,
# endif
};

static const auth_img_desc_t * const cot_desc[] = {
	[SEC_KEYRING_ID]		=	&sec_keyring,
	[BL31_IMAGE_ID]			=	&bl31_image,
	[BL32_IMAGE_ID]			=	&bl32_image,
	[BL33_IMAGE_ID]			=	&bl33_image,
};

/* Register the CoT in the authentication module */
REGISTER_COT(cot_desc);
