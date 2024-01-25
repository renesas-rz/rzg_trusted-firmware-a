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
	{SYS_MSTACCCTL0,  0xBBBBBBBBU, 0xAAAAAAAAU},
	{SYS_MSTACCCTL1,  0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL2,  0x00BBBBBBU, 0x00AAAAAAU},
	{SYS_MSTACCCTL3,  0x00BBBBBBU, 0x00AAAAAAU},
	{SYS_MSTACCCTL4,  0x0000BBBBU, 0x0000AAAAU},
	{SYS_MSTACCCTL5,  0x00BBBBBBU, 0x00AAAAAAU},
	{SYS_MSTACCCTL6,  0x0000BBBBU, 0x0000AAAAU},
	{SYS_MSTACCCTL7,  0x0000BBBBU, 0x0000AAAAU},
	{SYS_MSTACCCTL8,  0xBBBBBBBBU, 0xAAAAAAAAU},
	{SYS_MSTACCCTL9,  0x0000BBBBU, 0x0000AAAAU},
	{SYS_MSTACCCTL10, 0x00BBBBBBU, 0x00AAAAAAU},
	{SYS_MSTACCCTL11, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL12, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL13, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL14, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL15, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL16, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL17, 0x000000BBU, 0x000000AAU},
	{SYS_MSTACCCTL18, 0xBBBBBBBBU, 0xAAAAAAAAU},
	/* Slave Access Control Register */
	{SYS_SLVACCCTL0,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL1,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL2,  0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL3,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL4,  0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL5,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL6,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL7,  0x00000FFFU, 0x00000000U},
	{SYS_SLVACCCTL8,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL9,  0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL10, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL11, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL12, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL13, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL14, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL15, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL17, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL19, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL20, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL22, 0x000003FFU, 0x00000000U},
	{SYS_SLVACCCTL32, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL33, 0x003FFFC3U, 0x00000000U},
	{SYS_SLVACCCTL35, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL36, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL37, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL38, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL39, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL40, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL41, 0x0000000FU, 0x00000000U},
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
	{SYS_SLVACCCTL58, 0x000FFFFFU, 0x00000000U},
	{SYS_SLVACCCTL59, 0x0003FFFFU, 0x00000000U},
	{SYS_SLVACCCTL64, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL65, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL66, 0x0000FFFFU, 0x00000000U},
	{SYS_SLVACCCTL67, 0x000000FFU, 0x00000000U},
	{SYS_SLVACCCTL68, 0x0000000FU, 0x00000000U},
	{SYS_SLVACCCTL80, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL81, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL82, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL88, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL89, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL91, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL92, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL93, 0x00000003U, 0x00000000U},
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

static void plat_tzc_msram_setup(void)
{
	const arm_tzc_regions_info_t msram_tzc_regions[] = {
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0*/
			.end  = 0,	/* Not Used by Region 0*/
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},
		{}
	};

	plat_tzc400_setup(RZV2H_TZC400_M33_BASE, &msram_tzc_regions[0]);
}

static void plat_tzc_ddr_setup(void)
{
	const arm_tzc_regions_info_t ddr0_1_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0*/
			.end  = 0,	/* Not Used by Region 0*/
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},

		{
			/* Region 1: */
			.base = PLAT_FW_TZC_PROT_DRAM01_BASE,
			.end  = PLAT_FW_TZC_PROT_DRAM01_END,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},

		{
			/* Region 2: */
			.base = PLAT_TEE_TZC_PROT_DRAM01_BASE,
			.end  = PLAT_TEE_TZC_PROT_DRAM01_END,
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
#else
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},
#endif /* TRUSTED_BOARD_BOOT */
		{}
	};

	const arm_tzc_regions_info_t ddr_default_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	plat_tzc400_setup(RZV2H_TZC400_DDR00_BASE, &ddr_default_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_DDR01_BASE, &ddr0_1_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_DDR10_BASE, &ddr_default_tzc_regions[0]);
	plat_tzc400_setup(RZV2H_TZC400_DDR11_BASE, &ddr_default_tzc_regions[0]);
}

static void plat_tzc_spi_setup(void)
{
	const arm_tzc_regions_info_t xspi_tzc_regions[] = {
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},

		{
			/* Region 1: Access to region required to store */
			.base = RZV2H_XSPI_MEMORY_MAP_BASE,
			.end = (RZV2H_XSPI_MEMORY_MAP_BASE + RZV2H_XSPI_SIZE - 1ULL),
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},


		{}
	};

	plat_tzc400_setup(RZV2H_TZC400_xSPI_BASE, &xspi_tzc_regions[0]);
}

static void plat_tzc_pci_setup(void)
{
	const arm_tzc_regions_info_t pci_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	plat_tzc400_setup(RZV2H_TZC400_PCIe_BASE, &pci_tzc_regions[0]);
}

static void plat_tzc_r8_setup(void)
{
	const arm_tzc_regions_info_t r8_tzc_regions[] = {
		{
			/* Default Region 0: Complete access */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},

		{}
	};

	plat_tzc400_setup(RZV2H_TZC400_R8_BASE, &r8_tzc_regions[0]);
}


static void bl2_security_setup(void)
{
	/* initialize TZC-400 */
	plat_tzc_msram_setup();
	plat_tzc_spi_setup();
	plat_tzc_ddr_setup();
	plat_tzc_pci_setup();
	plat_tzc_r8_setup();

	/* setup Master/Slave Access Control */
	plat_access_control_setup();
}

void plat_security_setup(void)
{
	bl2_security_setup();
}
