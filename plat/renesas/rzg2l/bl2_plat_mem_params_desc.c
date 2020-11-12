/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/desc_image_load.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include <plat/common/platform.h>

#if (RZG2L_BL33_EXECUTION_EL == 0)
#define BL33_MODE MODE_EL1
#else
#define BL33_MODE MODE_EL2
#endif

static bl_mem_params_node_t bl2_mem_params_descs[] = {
	{
		.image_id = BL31_IMAGE_ID,

		SET_STATIC_PARAM_HEAD(ep_info, PARAM_EP, VERSION_2,
			entry_point_info_t, SECURE | EXECUTABLE | EP_FIRST_EXE),
		.ep_info.spsr = SPSR_64(MODE_EL3,
			MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS),
		.ep_info.pc = BL31_BASE,


		SET_STATIC_PARAM_HEAD(image_info, PARAM_EP, VERSION_2,
			image_info_t, IMAGE_ATTRIB_PLAT_SETUP),
		.image_info.image_max_size = BL31_LIMIT - BL31_BASE,
		.image_info.image_base = BL31_BASE,

		.next_handoff_image_id = BL33_IMAGE_ID,
	},
	{
		.image_id = BL33_IMAGE_ID,

		SET_STATIC_PARAM_HEAD(ep_info, PARAM_EP, VERSION_2,
			entry_point_info_t, NON_SECURE | EXECUTABLE),
		.ep_info.spsr = SPSR_64(BL33_MODE, MODE_SP_ELX,
			DISABLE_ALL_EXCEPTIONS),
		.ep_info.pc = BL33_BASE,
		SET_STATIC_PARAM_HEAD(image_info, PARAM_EP, VERSION_2,
			image_info_t, 0),
		.image_info.image_max_size =
				(uint32_t) (BL33_LIMIT - BL33_BASE),
		.image_info.image_base = BL33_BASE,

		.next_handoff_image_id = INVALID_IMAGE_ID,
	}
};

REGISTER_BL_IMAGE_DESCS(bl2_mem_params_descs)
