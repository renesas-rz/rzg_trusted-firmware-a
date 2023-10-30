/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/tzc400.h>
#include <lib/mmio.h>
#include <common/debug.h>

#include <sys_regs.h>
#include <plat_tzc_def.h>

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
	{SYS_MSTACCCTL0,  0x00BB00BBU, 0x00AAAA88U},
	{SYS_MSTACCCTL1,  0xBBBBBBBBU, 0xAAAAAAAAU},
	{SYS_MSTACCCTL2,  0x00BBBBBBU, 0x00AAAAAAU},
	{SYS_MSTACCCTL6,  0x0000BBBBU, 0x0000AAAAU},
	/* Slave Access Control Register */
	{SYS_SLVACCCTL0,  0x0000003FU, 0x0000002AU},
	{SYS_SLVACCCTL2,  0x00003FFFU, 0x00002AAAU},
	{SYS_SLVACCCTL3,  0x0000FFFFU, 0x00000030U},
#if SECURE_RTC
	{SYS_SLVACCCTL4,  0x3FFFFFFFU, 0x20000002U},
#else
	{SYS_SLVACCCTL4,  0x3FFFFFFFU, 0x00000002U},
#endif
	{SYS_SLVACCCTL5,  0x00003FFFU, 0x00000000U},
	{SYS_SLVACCCTL6,  0x0003FFFFU, 0x00000000U},
	{SYS_SLVACCCTL7,  0x003FFFFFU, 0x00000000U},
	{SYS_SLVACCCTL8,  0x0003FFFFU, 0x00000000U},
	{SYS_SLVACCCTL9,  0x00003FFFU, 0x00000000U},
	{SYS_SLVACCCTL10, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL11, 0x00000FFFU, 0x000002AAU},
	{SYS_SLVACCCTL12, 0x0000003FU, 0x00000000U},
	{SYS_SLVACCCTL14, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL16, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL17, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL18, 0x00000003U, 0x00000000U},
	{SYS_SLVACCCTL20, 0x00000003U, 0x00000002U},
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

static uint8_t tzc400_get_num_filters(uintptr_t tzc_base)
{
	if (tzc_base != RZG3S_TZC400_DDR_BASE) {
		return (uint8_t)((mmio_read_32(tzc_base + BUILD_CONFIG_OFF) >> BUILD_CONFIG_NF_SHIFT) & BUILD_CONFIG_NF_MASK) + 1U;
	} else {
		return (uint8_t)PLAT_TZC400_DDR_FILTER_NUM;
	}
}

static void plat_tzc400_setup(uintptr_t tzc_base, const arm_tzc_regions_info_t *tzc_regions)
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
#if PLAT_M33_BOOT_SUPPORT
	const arm_tzc_regions_info_t msram0_tzc_regions[] = {
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
		{}
	};

	const arm_tzc_regions_info_t msram1_tzc_regions[] = {
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_UNPRIV
		},
		{}
	};

	plat_tzc400_setup(RZG3S_TZC400_MSRAM_0_BASE, &msram0_tzc_regions[0]);
	plat_tzc400_setup(RZG3S_TZC400_MSRAM_1_BASE, &msram1_tzc_regions[0]);
#endif
}

static void plat_tzc_ddr_setup(void)
{
	const arm_tzc_regions_info_t ddr_tzc_regions[] = {
#if TRUSTED_BOARD_BOOT
		{
			/* Default Region 0: Lock down */
			.base = 0,	/* Not Used by Region 0 */
			.end  = 0,	/* Not Used by Region 0 */
			.sec_attr = TZC_REGION_S_RDWR,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_S_PRIV
		},

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

		{
			.base = PLAT_TEE_TZC_PROT_DRAM01_END + 1,
			.end  = UL(0xFFFFFFFFF),
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
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

	plat_tzc400_setup(RZG3S_TZC400_DDR_BASE, &ddr_tzc_regions[0]);
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
			.base = RZG3S_SPIROM_BASE,
			.end  = (RZG3S_SPIROM_BASE + RZG3S_SPIROM_SIZE - 1ULL),
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},
		{
			.base = RZG3S_XSPI_BASE,
			.end  = (RZG3S_XSPI_BASE + 0x1FFFFULL),
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},
		{
			.base = RZG3S_OCTA_BASE,
			.end  = (RZG3S_OCTA_BASE + 0x0FFFFULL),
			.sec_attr = TZC_REGION_S_NONE,
			.nsaid_permissions = PLAT_TZC_REGION_ACCESS_NS_UNPRIV
		},
		{}
	};

	plat_tzc400_setup(RZG3S_TZC400_xSPI_BASE, &xspi_tzc_regions[0]);
}

static void bl2_security_setup(void)
{
	/* initialize TZC-400 */
	plat_tzc_msram_setup();
	plat_tzc_spi_setup();
	plat_tzc_ddr_setup();

	/* setup Master/Slave Access Control */
	plat_access_control_setup();
}

void plat_security_setup(void)
{
	bl2_security_setup();
}
