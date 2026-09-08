/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <arch_helpers.h>
#include <platform_def.h>

#include <common/debug.h>
#include <common/tbbr/cot_def.h>
#include <drivers/auth/auth_common.h>
#include <drivers/auth/auth_mod.h>
#include <lib/fconf/fconf_tbbr_getter.h>
#include <plat/common/platform.h>
#include "tsip/tsip_lib.h"

#define return_if_error(rc) \
	do { \
		if (rc != 0) { \
			return rc; \
		} \
	} while (0)

/*
 * Return the parent id in the output parameter '*parent_id'
 *
 * Return value:
 *   0 = Image has parent, 1 = Image has no parent or parent is authenticated
 */
int auth_mod_get_parent_id(unsigned int img_id, unsigned int *parent_id)
{
#if RZG2_SECURE_BOOT
	const auth_img_desc_t *img_desc = NULL;

	assert(parent_id != NULL);
	/* Get the image descriptor */
	img_desc = FCONF_GET_PROPERTY(tbbr, cot, img_id);

	/* Check if the image has no parent (ROT) */
	if (img_desc->parent == NULL) {
		*parent_id = 0;
		return 1;
	}

	/* Check if the parent has already been authenticated */
	if (auth_img_flags[img_desc->parent->img_id] & IMG_FLAG_AUTHENTICATED) {
		*parent_id = 0;
		return 1;
	}

	*parent_id = img_desc->parent->img_id;
	return 0;
#else
	return 1;
#endif /* RZG2_SECURE_BOOT */
}

int auth_mod_verify_img(unsigned int img_id, void *ptr, unsigned int len)
{
#if RZG2_SECURE_BOOT
	int rc = 0;
	uintptr_t img_ptr;
	unsigned long img_len;
	const auth_img_desc_t *img_desc = NULL;

	/* Get the image descriptor from the chain of trust */
	img_desc = FCONF_GET_PROPERTY(tbbr, cot, img_id);

#if (RCAR_BL2_DCACHE == 1) || (IMAGE_BL1)
	/* clean and disable */
	write_sctlr_el3(read_sctlr_el3() & ~SCTLR_C_BIT);
	dcsw_op_all(DCCISW);
#endif

	img_ptr = (uintptr_t)ptr + sizeof(uint64_t);
	img_len = *(uint64_t *)ptr;

	INFO("Verify image id=%u at address 0x%lx, size 0x%lx\n", img_id, img_ptr, img_len);

	/* Check that the image size to verify is within the size loaded */
	if (len < img_len)
		return 1;

	switch (img_id)
	{
	case SEC_KEYRING_ID:
		rc = tsip_auth_keys((void *)img_ptr, img_len);
		break;
	default:
		rc = tsip_auth_image((void *)img_ptr, img_len, ptr);
		break;
	}

#if (RCAR_BL2_DCACHE == 1) || (IMAGE_BL1)
	/* enable */
	write_sctlr_el3(read_sctlr_el3() | SCTLR_C_BIT);
#endif

	return_if_error(rc);

	INFO("Image id=%u verified: 0x%lx - 0x%lx\n", img_id, (uintptr_t)ptr, (uintptr_t)ptr + img_len);

	/* Mark image as authenticated */
	auth_img_flags[img_desc->img_id] |= IMG_FLAG_AUTHENTICATED;
#endif /* RZG2_SECURE_BOOT */
	return 0;
}

void auth_mod_init(void)
{
#if RZG2_SECURE_BOOT

#if IMAGE_BL2
	tsip_auth_init((void *)BL2_SECURE_RAM, (void *)BL2_SECURE_HEAP);
#endif /* IMAGE_BL2 */

#endif /* RZG2_SECURE_BOOT */
}
