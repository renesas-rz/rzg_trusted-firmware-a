/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <sys_regs.h>
#include <xspi_puccini_regs.h>
#include <sys.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <drivers/delay_timer.h>


#if PLATFORM_CORE_COUNT == 2
	#define SCKCR2_CA55_MASK		UL(0x00001300)		/* Mask to set A55 DSU clock frequency to 1000MHz and to configure all A55 cores for 1200MHz */
#elif PLATFORM_CORE_COUNT == 1
	#define SCKCR2_CA55_MASK		UL(0x00001100)		/* Mask to set A55 DSU clock frequency to 1000MHz and to configure the single A55 core for 1200MHz */
#else
	#error "PLATFORM_CORE_COUNT is set to illegal value. It should either be set for 1,2 or 4 CA55 cores"
#endif

#define NUM_DUMMY_PERIPH_READS 		13

static void cpg_mstop_sdhi0(void)
{
	volatile uint32_t dummy;

	//* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Set bit to ensure SDHI0 is in Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) | (SSTPCR1_SDHI0_REQ_MSK));
	while (SSTPCR1_SDHI0_ACK_MSK != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI0_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);
	/* Clear bit to release SDHI0 from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM12)));
	/* Dummy read MSTPCRM register once */
	dummy = mmio_read_32(MSTPCRM);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Clear bit to release SDHI0 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_SDHI0_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI0_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Dummy read SD_CMD register NUM_DUMMY_PERIPH_READS times (SD_CMD chosen arbitarily) */
	uint32_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(SDHI0_SD_CMD);
	}
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;
	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_sdhi1(void)
{
	volatile uint32_t dummy;

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Set bit to ensure SDHI1 is in Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) | (SSTPCR1_SDHI1_REQ_MSK));
	while (SSTPCR1_SDHI1_ACK_MSK != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI1_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);
	/* Clear bit to release SDHI1 from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM13)));
	/* Dummy read MSTPCRM register once */
	dummy = mmio_read_32(MSTPCRM);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Clear bit to release SDHI1 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_SDHI1_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI1_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Dummy read SD_CMD register NUM_DUMMY_PERIPH_READS times (SD_CMD chosen arbitarily) */
	uint32_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(SDHI1_SD_CMD);
	}
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;
	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_xspi0(void)
{
	volatile uint32_t dummy;

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Set bit to ensure xSPI0 is in Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) | SSTPCR6_XSPI0_REQ_MSK);
	while (SSTPCR6_XSPI0_ACK_MSK != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI0_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);
	/* Clear bit to release xSPI0 from Module Stop State */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA04)));
	/* Dummy read MSTPCRA register once */
	dummy = mmio_read_32(MSTPCRA);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Clear bit to release xSPI0 from Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) & ~SSTPCR6_XSPI0_REQ_MSK);
	while (0U != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI0_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Dummy read WRAPCFG register NUM_DUMMY_PERIPH_READS times (WRAPCFG chosen arbitarily) */
	uint32_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(XSPI0_WRAPCFG);
	}
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;
	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_xspi1(void)
{
	volatile uint32_t dummy;

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Set bit to ensure xSPI1 is in Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) | SSTPCR6_XSPI1_REQ_MSK);
	while (SSTPCR6_XSPI1_ACK_MSK != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI1_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);
	/* Clear bit to release xSPI1 from Module Stop State */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA05)));
	/* Dummy read MSTPCRA register once */
	dummy = mmio_read_32(MSTPCRA);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Clear bit to release xSPI1 from Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) & ~SSTPCR6_XSPI1_REQ_MSK);
	while (0U != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI1_ACK_MSK)) {
		/* nothing */
	}
	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/* Dummy read WRAPCFG register NUM_DUMMY_PERIPH_READS times (WRAPCFG chosen arbitarily) */
	uint32_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(XSPI1_WRAPCFG);
	}
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;
	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_scif(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release SCIF1 from Module Stop State  */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA09)));
	/* Dummy read MSTPCRA register once */
	dummy = mmio_read_32(MSTPCRA);
	/* Dummy read CCR0 register NUM_DUMMY_PERIPH_READS times (CCR0 chosen arbitarily) */
	uint32_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(RZT2N_SCIF1_BASE + SCI_CCR0_OFFSET);
	}
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_gic(void)
{
	//* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release GIC from Bus Stop Request State */
	mmio_write_32(SSTPCR5, mmio_read_32(SSTPCR5) & (~SSTPCR5_GIC600_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR5) & SSTPCR5_GIC600_ACK_MSK)) {
		/* nothing */
	}

	/*GIC doesn't have a MSTPCRn bit*/

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_ca55(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_safetybase_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release CA55 Core 0 from Module Stop State */
	mmio_write_32(MSTPCRN, mmio_read_32(MSTPCRN) & (~MSTPCRN_MSTPCRN2_MSK));
	/* Dummy read MSTPCRE register once */
	dummy = mmio_read_32(MSTPCRN);
	/* Dummy read RVBALn register NUM_DUMMY_PERIPH_READS times (RVBALn chosen arbitarily) */
	uint8_t i;
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(CA55_RVBAL0);
	}

	/* Clear bit to release CA55 Core 1 from Module Stop State */
	mmio_write_32(MSTPCRN, mmio_read_32(MSTPCRN) & (~MSTPCRN_MSTPCRN3_MSK));
	/* Dummy read MSTPCRE register once */
	dummy = mmio_read_32(MSTPCRN);
	/* Dummy read RVBALn register NUM_DUMMY_PERIPH_READS times (RVBALn chosen arbitarily) */
	for (i = 0; i < NUM_DUMMY_PERIPH_READS; i++) {
		dummy = mmio_read_32(CA55_RVBAL1);
	}

	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_safetybase_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_setup(void)
{
	cpg_mstop_sdhi0();	/* eMMC */
	cpg_mstop_sdhi1();	/* eSD */
	cpg_mstop_xspi0();
	cpg_mstop_xspi1();
	cpg_mstop_scif();
	cpg_mstop_gic();
	/* For Linux only */
	cpg_mstop_ca55();
}

static void cpg_pll_setup(void)
{
	/* Enable write to clock generation circuit Registers */
	sys_safetybase_unlock(PRCRx_CLOCK_GEN);
// FIXME: Move this to DDR init
#if 0
	/* Setup PLL2. PLL0 is already set in BootROM since A55 will be the second boot CPU for TF-A. */
	if (PLL2EN_PLL2EN_ENABLE != (mmio_read_32(PLL2EN) & PLL2EN_PLL2EN_MSK)) {
		/* PLL enable */
		mmio_write_32(PLL2EN, PLL2EN_PLL2EN_ENABLE);

		/* wait for PLL Locked */
		while (PLL2MON_PLL2MON_LOCK != (mmio_read_32(PLL2MON) & PLL2MON_PLL2MON_MSK)) {
			/* nothing */
		}

		/* Wait for PLL selected */
		while (PMSEL_PMSEL2_MON_PLL != (mmio_read_32(PMSEL) & PMSEL_PMSEL2_MON_MSK)) {
			/* nothing */
		}
	}
#endif
	/* Disable write to clock generation circuit Registers */
	sys_safetybase_lock(PRCRx_CLOCK_GEN);
}

static void cpg_reset_xspi0(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop and Reset Registers */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release XSPI from Module Reset State */
	mmio_write_32(MRCTLA, mmio_read_32(MRCTLA) & (~BIT_32(MRCTLA_MRCTLA04)));
	/* Dummy read from MRCTLA */
	dummy = mmio_read_32(MRCTLA);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop and Reset Registers */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_reset_xspi1(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop and Reset Registers */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release XSPI from Module Reset State */
	mmio_write_32(MRCTLA, mmio_read_32(MRCTLA) & (~BIT_32(MRCTLA_MRCTLA05)));
	/* Dummy read from MRCTLA */
	dummy = mmio_read_32(MRCTLA);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop and Reset Registers */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_reset_setup(void)
{
	cpg_reset_xspi0();
	cpg_reset_xspi1();
}

void cpg_set_ca55_1200mhz(void)
{
	/* Enable write to SCKCR2 register */
	sys_safetybase_unlock(PRCRx_CLOCK_GEN);

	/* Enable 1200MHz clock on all CA55 cores present */
	mmio_write_32(SCKCR2, ((mmio_read_32(SCKCR2) & (~SCKCR2_CA55_MASK)) | SCKCR2_CA55_MASK));

	/* Disable write to SCKCR2 Register */
	sys_safetybase_lock(PRCRx_CLOCK_GEN);
}

void cpg_early_setup(void)
{
	cpg_set_ca55_1200mhz();
}

void cpg_disable_mpu_protect(void)
{
	/* Disable all MPU */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	/* Master module - Bus A */
	mmio_write_32(0x81291400, 0);	//MPU 0 - GMAC Unit 1
	mmio_write_32(0x81291500, 0);	//MPU 1 - GMAC Unit 2
	mmio_write_32(0x81291600, 0);	//MPU 2 - USB Host
	mmio_write_32(0x81291700, 0);	//MPU 3 - USB function
	mmio_write_32(0x81291800, 0);	//MPU 4 - SDHI Unit 0
	mmio_write_32(0x81291900, 0);	//MPU 5 - SDHI Unit 1
	mmio_write_32(0x81291b00, 0);	//MPU 7 - PCIE Unit 0
	mmio_write_32(0x81291d00, 0);	//MPU 9 - CoreSight ETR
	mmio_write_32(0x81291f00, 0);	//MPU 17 - NANDC

	/* Master module - Bus R */
	mmio_write_32(0x81290400, 0);	//MPU 10 - DMAC Unit 0
	mmio_write_32(0x81290500, 0);	//MPU 11 - DMAC Unit 1
	mmio_write_32(0x81290600, 0);	//MPU 12 - DMAC Unit 2
	mmio_write_32(0x81290700, 0);	//MPU 13 - GMAC Unit 0
	mmio_write_32(0x81290800, 0);	//MPU 14 - SHOSTIF
	mmio_write_32(0x81290900, 0);	//MPU 15 - CoreSight AXI-AP
	mmio_write_32(0x81290c00, 0);	//MPU 20 - ESWM
	mmio_write_32(0x81290d00, 0);	//MPU 21 - PHOSTIF
	mmio_write_32(0x81292000, 0);	//AMPU 0 - Cortex-A55 AXIP
	mmio_write_32(0x81292100, 0);	//AMPU 1 - Cortex-R52 CPU0
	mmio_write_32(0x81292200, 0);	//AMPU 2 - Cortex-R52 CPU1

	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

void cpg_set_sdhi_200mhz(void)
{
	/* Enable write to SCKCR5 register */
	sys_base_unlock(PRCRx_CLOCK_GEN);

	/* Enable 200MHz clock for SDHI0_IMCLK */
	mmio_write_32(SCKCR5, ((mmio_read_32(SCKCR5) & (~SCKCR5_SDHI0_CLKSEL_MSK)) | SCKCR5_SDHI0_CLKSEL_MSK));

	/* Enable 200MHz clock for SDHI1_IMCLK */
	mmio_write_32(SCKCR5, ((mmio_read_32(SCKCR5) & (~SCKCR5_SDHI1_CLKSEL_MSK)) | SCKCR5_SDHI1_CLKSEL_MSK));

	/* Disable write to SCKCR2 Register */
	sys_base_lock(PRCRx_CLOCK_GEN);
}

static void cpg_slave_stop_pcie(void)
{
	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release PCIE0 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_PCIE_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR1) & SSTPCR1_PCIE_ACK_MSK)) {
		/* nothing */
	}

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

void cpg_setup(void)
{
	cpg_pll_setup();
	cpg_mstop_setup();
	cpg_reset_setup();
	cpg_disable_mpu_protect();
	cpg_set_sdhi_200mhz();
	cpg_slave_stop_pcie();
}

void cpg_ddr_part1(void)
{
	volatile uint32_t dummy;

	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	sys_base_unlock(PRCRx_LOW_POWER);

	mmio_write_32(MRCTLM, 0x031D0000);
	/* Dummy read from MRCTLM */
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	mmio_write_32(SSTPCR4, mmio_read_32(SSTPCR4) & (~SSTPCR4_DDR0_ALL_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR4) & SSTPCR4_DDR0_ALL_ACK_MSK)) {
		/* nothing */
	}

	mmio_write_32(SSTPCR0, mmio_read_32(SSTPCR0) & (~SSTPCR0_DDR1_ALL_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR0) & SSTPCR0_DDR1_ALL_ACK_MSK)) {
		/* nothing */
	}

	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM00)));
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	udelay(1);

	mmio_write_32(MRCTLM, 0x031C0000);
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	udelay(1);

	mmio_write_32(MRCTLM, 0x02040000);
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	udelay(1);

	sys_safetybase_lock(PRCRx_SYS_CTRL);
	sys_base_lock(PRCRx_LOW_POWER);
}

void cpg_ddr_part2(void)
{
	volatile uint32_t dummy;

	sys_base_unlock(PRCRx_LOW_POWER);

	mmio_write_32(MRCTLM, 0x02000000);
	/* Dummy read from MRCTLM */
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	udelay(10);

	mmio_write_32(MRCTLM, 0x00000000);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	sys_base_lock(PRCRx_LOW_POWER);
}
