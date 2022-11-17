/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/tzc400.h>
#include <lib/mmio.h>
#include <common/debug.h>

#include "rz_soc_def.h"
#include "sys_regs.h"
#include "plat_tzc_def.h"

typedef struct arm_tzc_regions_info {
	unsigned long long base;
	unsigned long long end;
	unsigned int sec_attr;
	unsigned int nsaid_permissions;
} arm_tzc_regions_info_t;


static const struct {
	uint32_t reg;
	uint32_t msk;
	uint32_t val;
} sys_acctl[] = {
	/* Master Access Control Register */
	{SYS_MSTACCCTL0,  0xBBBBBBBBU, 0x00000000U},
	{SYS_MSTACCCTL1,  0xBBBBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL2,  0x00BBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL3,  0x00BBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL4,  0x0000BBBBU, 0x55555555U},
	{SYS_MSTACCCTL5,  0x00BBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL6,  0x0000BBBBU, 0x55555555U},
	{SYS_MSTACCCTL7,  0x0000BBBBU, 0x55555555U},
	{SYS_MSTACCCTL8,  0xBBBBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL9,  0x0000BBBBU, 0x55555555U},
	{SYS_MSTACCCTL10, 0x00BBBBBBU, 0x55555555U},
	{SYS_MSTACCCTL11, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL12, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL13, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL14, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL15, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL16, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL17, 0x000000BBU, 0x55005500U},
	{SYS_MSTACCCTL18, 0xBBBBBBBBU, 0x55555555U},
	/* Slave Access Control Register */
	{SYS_SLVACCCTL0,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL1,  0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL2,  0x000003FFU, 0x55405540U},
	{SYS_SLVACCCTL3,  0x003FFFFFU, 0x00000000U},
	{SYS_SLVACCCTL4,  0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL5,  0x000FFFFFU, 0x55555555U},
	{SYS_SLVACCCTL6,  0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL7,  0x00000FFFU, 0x00000000U},
	{SYS_SLVACCCTL8,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL9,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL10, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL11, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL12, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL13, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL14, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL15, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL16, 0x0000000FU, 0x40004000U},
	{SYS_SLVACCCTL17, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL19, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL20, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL21, 0x0000000FU, 0x50005000U},
	{SYS_SLVACCCTL22, 0x000003FFU, 0x55405540U},
	{SYS_SLVACCCTL32, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL33, 0x003FFFC3U, 0x00000000U},
	{SYS_SLVACCCTL35, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL36, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL37, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL38, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL39, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL40, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL41, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL42, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL43, 0x000000FFU, 0x00000000U},
	{SYS_SLVACCCTL44, 0x000000FFU, 0x00000000U},
	{SYS_SLVACCCTL45, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL46, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL47, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL48, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL49, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL50, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL51, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL52, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL53, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL54, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL55, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL56, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL57, 0x00003FFFU, 0x55545554U},
	{SYS_SLVACCCTL58, 0x000FFFFFU, 0x55555555U},
	{SYS_SLVACCCTL59, 0x0003FFFFU, 0x55555555U},
	{SYS_SLVACCCTL64, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL65, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL66, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL67, 0x000000FFU, 0x00000000U},
	{SYS_SLVACCCTL68, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL80, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL81, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL82, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL87, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL88, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL89, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL90, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL91, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL92, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL93, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL94, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL95, 0x00000003U, 0x00000000U},
};


void plat_access_control_setup(void)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(sys_acctl); i++) {
		uint32_t val = mmio_read_32(sys_acctl[i].reg) & (~sys_acctl[i].msk);

		val |= (sys_acctl[i].val & sys_acctl[i].msk);

		mmio_write_32(sys_acctl[i].reg, val);
	}
}

uint8_t tzc400_get_num_filters(uintptr_t tzc_base)
{
	uint32_t tzc400_build;

	tzc400_build = mmio_read_32(tzc_base + BUILD_CONFIG_OFF);

	return (uint8_t)((tzc400_build >> BUILD_CONFIG_NF_SHIFT) & BUILD_CONFIG_NF_MASK) + 1U;
}

void plat_tzc400_setup(uintptr_t tzc_base, const arm_tzc_regions_info_t *tzc_regions)
{
	uint8_t num_filters;
	unsigned int region_index = 1U;
	const arm_tzc_regions_info_t *p;
	const arm_tzc_regions_info_t init_tzc_regions[] = {
		{0}
	};

	INFO("Configuring TrustZone Controller\n");

	tzc400_init(tzc_base);

	tzc400_disable_filters();

	tzc400_configure_region0(TZC_REGION_S_RDWR, PLAT_TZC_REGION_ACCESS_NS_UNPRIV);

	if (tzc_regions == NULL)
		p = init_tzc_regions;
	else
		p = tzc_regions;

	num_filters = tzc400_get_num_filters(tzc_base);

	for (; p->base != 0UL; p++) {
		tzc400_configure_region((1 << num_filters) - 1, region_index,
			p->base, p->end, p->sec_attr, p->nsaid_permissions);
		region_index++;
	}

	INFO("Total %u regions set.\n", region_index);

	tzc400_set_action(TZC_ACTION_ERR);

	tzc400_enable_filters();
}

#if IMAGE_BL2
static void bl2_security_setup(void)
{
	const arm_tzc_regions_info_t ddr_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{
			.base = PLAT_FW_TZC_PROT_DRAM01_BASE,
			.end  = PLAT_FW_TZC_PROT_DRAM01_END,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},

		{
			.base = PLAT_TEE_TZC_PROT_DRAM01_BASE,
			.end  = PLAT_TEE_TZC_PROT_DRAM01_END,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
#endif /* TRUSTED_BOARD_BOOT */
		{}
	};

	/* initialize TZC-400 */
	plat_tzc400_setup(RZV2H_TZC400_DDR00_BASE, &ddr_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_DDR01_BASE, &ddr_tzc_regions[0]);			//TODO: KTG: Confirm
	plat_tzc400_setup(RZV2H_TZC400_DDR10_BASE, NULL);			//TODO: KTG: Confirm
	plat_tzc400_setup(RZV2H_TZC400_DDR11_BASE, NULL);			//TODO: KTG: Confirm
	plat_tzc400_setup(RZV2H_TZC400_xSPI_BASE,  NULL);
	plat_tzc400_setup(RZV2H_TZC400_PCIe_BASE,  NULL);

	/* setup Master/Slave Access Control */
	plat_access_control_setup();
}
#endif

#if IMAGE_BL31
static void bl31_security_setup(void)
{
	const arm_tzc_regions_info_t msram_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{
			.base = PLAT_AP_TZC_PROT_SRAM0_BASE,
			.end  = PLAT_AP_TZC_PROT_SRAM1_END,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
#endif /* TRUSTED_BOARD_BOOT */
		{}
	};

	const arm_tzc_regions_info_t asram_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{
			.base = PLAT_AP_TZC_PROT_SRAM2_BASE,
			.end  = PLAT_AP_TZC_PROT_SRAM2_END,
			.sec_attr = TZC_REGION_S_RDWR, 
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
#endif /* TRUSTED_BOARD_BOOT */
		{}
	};

	const arm_tzc_regions_info_t r8sram_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{ 
			.base = PLAT_AP_TZC_PROT_SRAM3_BASE,				//TODO: KTG: SRAM sections used by CR8 need to be established
			.end  = PLAT_AP_TZC_PROT_SRAM3_END,
			.sec_attr = TZC_REGION_S_RDWR, 
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
#endif /* TRUSTED_BOARD_BOOT */
		{}
	};

	/* Additional settings for TZC-400 SRAM */
	plat_tzc400_setup(RZV2H_TZC400_M33_BASE, &msram_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_A55_BASE, &asram_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_R8_BASE,  &r8sram_tzc_regions[0]);
}
#endif

void plat_security_setup(void)
{
#if IMAGE_BL2
	bl2_security_setup();
#endif

#if IMAGE_BL31
	bl31_security_setup();
#endif
}
