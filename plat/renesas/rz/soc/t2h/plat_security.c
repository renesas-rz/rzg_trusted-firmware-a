/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/tzc400.h>
#include <lib/mmio.h>
#include <common/debug.h>

#include "rz_soc_def.h"
#include "platform_def.h"
#include "sys.h"
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
} acctl[] = {
	/* Master Access Control Register */
	{MSTACCCTL0,  0xFFFFFFFFU, 0xEEEEEEEEU},
	{MSTACCCTL1,  0x0000000FU, 0x0000000EU},
	{MSTACCCTL2,  0xFFFFF0FFU, 0xEEEEE0EEU},
	{MSTACCCTL3,  0x0000000FU, 0x0000000EU},
	/* Slave Access Control Register */
	{SLVACCCTL0,  0xCFFFF3FFU, 0x00000000U},
	{SLVACCCTL1,  0xFF3FF3FFU, 0x00000000U},
	{SLVACCCTL2,  0x003F0F0FU, 0x00000000U},
	{SLVACCCTL4,  0x0F0F0303U, 0x00000000U},
	{SLVACCCTL5,  0xFFFF003FU, 0x00000000U},
	{SLVACCCTL6,  0xFF3FFCFFU, 0x00000000U},
	{SLVACCCTL7,  0xFFCC000FU, 0x00000000U},
	{SLVACCCTL8,  0x0F3FFFF3U, 0x00000000U},
	{SLVACCCTL9,  0x0FFF0003U, 0x00000000U},
};


void plat_access_control_setup(void)
{
	uint32_t i;

	/* Un-lock writing to MSTACCCTLx and SLVACCCTLx registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	for (i = 0; i < ARRAY_SIZE(acctl); i++) {
		uint32_t val = mmio_read_32(acctl[i].reg) & (~acctl[i].msk);

		val |= (acctl[i].val & acctl[i].msk);

		mmio_write_32(acctl[i].reg, val);
	}

	/* Lock writing to MSTACCCTLx and SLVACCCTLx registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
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
	const arm_tzc_regions_info_t *p = tzc_regions;

	if (p == NULL) {
		ERROR("Invalid Trust Zone Configuration\n");
		return;
	}

	INFO("Configuring TrustZone Controller\n");

	tzc400_init(tzc_base);

	tzc400_disable_filters();

	/* Region 0 configuration is always supplied */
	tzc400_configure_region0(p->sec_attr, p->nsaid_permissions);
	p++;

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

static void plat_tzc_ddr_setup(void)
{
	const arm_tzc_regions_info_t ddr_a_bus_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	const arm_tzc_regions_info_t ddr_r_bus_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	/* DDR A-BUS */
	plat_tzc400_setup(RZT2H_TZC400_0, &ddr_a_bus_tzc_regions[0]);
	plat_tzc400_setup(RZT2H_TZC400_1, &ddr_a_bus_tzc_regions[0]);
	/* DDR R-BUS */
	plat_tzc400_setup(RZT2H_TZC400_3, &ddr_r_bus_tzc_regions[0]);
	plat_tzc400_setup(RZT2H_TZC400_4, &ddr_r_bus_tzc_regions[0]);
}

static void plat_tzc_xspi_setup(void)
{
	const arm_tzc_regions_info_t xspi_tzc_regions[] = {
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0*/
			.end  = 0,	/* Not Used by Region 0*/
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},

		{
			/* Region 1: Setup for XSPI0 */
			.base = RZT2H_XSPI0_MEMORY_MAP_BASE,
			.end  = RZT2H_XSPI0_MEMORY_MAP_BASE + RZT2H_XSPI_MEMORY_MAP_SIZE - 1ULL,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{
			/* Region 2: Setup for XSPI1 */
			.base = RZT2H_XSPI1_MEMORY_MAP_BASE,
			.end  = RZT2H_XSPI1_MEMORY_MAP_BASE + RZT2H_XSPI_MEMORY_MAP_SIZE - 1ULL,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	plat_tzc400_setup(RZT2H_TZC400_7, &xspi_tzc_regions[0]);
}

static void plat_tzc_pci_setup(void)
{
	const arm_tzc_regions_info_t pci_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_PRIV
		},

		{}
	};

	plat_tzc400_setup(RZT2H_TZC400_2, &pci_tzc_regions[0]);
}

static void plat_tzc_r52_tcm_setup(void)
{
	const arm_tzc_regions_info_t r52_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},

		{}
	};

	plat_tzc400_setup(RZT2H_TZC400_8, &r52_tzc_regions[0]);
}

static void plat_tzc_bsc_setup(void)
{
	const arm_tzc_regions_info_t bsc_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_PRIV
		},

		{}
	};

	plat_tzc400_setup(RZT2H_TZC400_6, &bsc_tzc_regions[0]);
}

static void bl2_security_setup(void)
{
	/* initialize TZC-400 */
	plat_tzc_xspi_setup();
	plat_tzc_ddr_setup();
	plat_tzc_pci_setup();
	plat_tzc_r52_tcm_setup();
	plat_tzc_bsc_setup();

	/* setup Master/Slave Access Control */
	plat_access_control_setup();
}

void plat_security_setup(void)
{
	bl2_security_setup();
}
