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


#if PLATFORM_CORE_COUNT == 4
	#define SCKCR2_CA55_MASK		UL(0x00001F00)		/* Mask to set SCU and L3 clock frequency to 1000MHz and to configure all four A55 cores for 1200MHz */
#elif PLATFORM_CORE_COUNT == 2
	#define SCKCR2_CA55_MASK		UL(0x00001300)		/* Mask to set SCU and L3 clock frequency to 1000MHz and to configure all two A55 cores for 1200MHz */
#elif PLATFORM_CORE_COUNT == 1
	#define SCKCR2_CA55_MASK		UL(0x00001100)		/* Mask to set SCU and L3 clock frequency to 1000MHz and to configure the single A55 core for 1200MHz */
#else
	#error "PLATFORM_CORE_COUNT is set to illegal value. It should either be set for 1,2 or 4 CA55 cores"
#endif


static void cpg_mstop_sdhi0(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release SDHI0 from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM12)));
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release SDHI0 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_SDHI0_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI0_ACK_MSK)) {
		/* nothing */
	}

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_sdhi1(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release SDHI1 from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM13)));
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release SDHI1 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_SDHI1_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR1) & SSTPCR1_SDHI1_ACK_MSK)) {
		/* nothing */
	}

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_xspi0(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release XSPI from Module Stop State */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA04)));
	/* Dummy reads from MSTPCRA */
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release XSPI from Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) & ~SSTPCR6_XSPI0_REQ_MSK);
	while (0U != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI0_ACK_MSK)) {
		/* nothing */
	}
	/* Dummy read from a xSPI0 register*/
	dummy = mmio_read_32(RZT2H_XSPI0_BASE + XSPI_WRAPCFG_OFFSET);

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_xspi1(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release XSPI from Module Stop State */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA05)));
	/* Dummy reads from MSTPCRA */
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);

	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release XSPI from Bus Stop Request State */
	mmio_write_32(SSTPCR6, mmio_read_32(SSTPCR6) & ~SSTPCR6_XSPI1_REQ_MSK);
	while (0U != (mmio_read_32(SSTPCR6) & SSTPCR6_XSPI1_ACK_MSK)) {
		/* nothing */
	}

	/* Dummy read from a xSPI1 register */
	dummy = mmio_read_32(RZT2H_XSPI1_BASE + XSPI_WRAPCFG_OFFSET);

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_scif(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release SCIF0 from Module Stop State  */
	mmio_write_32(MSTPCRA, mmio_read_32(MSTPCRA) & (~BIT_32(MSTPCRA_MSTPCRA08)));
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	dummy = mmio_read_32(MSTPCRA);
	/* Dummy register reads required to make sure peripheral is released from module-stop */
	dummy = mmio_read_32(RZT2H_SCIF_BASE + SCI_CCR0_OFFSET);	/* dummy-read of a SCIO register */
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_usb(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release USB from Module Stop State	*/
	mmio_write_32(MSTPCRE, mmio_read_32(MSTPCRE) & (~BIT_32(MSTPCRE_MSTPCRE08)));
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_cmtw(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release CMTW from Module Stop State */
	mmio_write_32(MSTPCRD, mmio_read_32(MSTPCRD) & (~BIT_32(MSTPCRD_MSTPCRD05)));
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	dummy = mmio_read_32(MSTPCRD);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_gic(void)
{
	/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release GIC from Bus Stop Request State */
	mmio_write_32(SSTPCR5, mmio_read_32(SSTPCR5) & (~SSTPCR5_GIC600_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR5) & SSTPCR5_GIC600_ACK_MSK)) {
		/* nothing */
	}

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_pcie(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release PCIe from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM08)));
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);

		/* Enable write to System Registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Clear bit to release PCIe0 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_PCIE0_REQ_MSK));
	while (0 != (mmio_read_32(SSTPCR1) & SSTPCR1_PCIE0_ACK_MSK)) {
		/* nothing */
	}

	/* Clear bit to release PCIe1 from Bus Stop Request State */
	mmio_write_32(SSTPCR1, mmio_read_32(SSTPCR1) & (~SSTPCR1_PCIE1_REQ_MSK));
	while (0 != (mmio_read_32(SSTPCR1) & SSTPCR1_PCIE1_ACK_MSK)) {
		/* nothing */
	}

	/* Disable write to System Registers */
	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

static void cpg_mstop_rtc(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_safetybase_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release RTC from Module Stop State */
	mmio_write_32(MSTPCRG, mmio_read_32(MSTPCRG) & (~BIT_32(MSTPCRG_MSTPCRG05)));
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	dummy = mmio_read_32(MSTPCRG);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_safetybase_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_i2c0(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release I2C0 from Module Stop State */
	mmio_write_32(MSTPCRB, mmio_read_32(MSTPCRB) & (~BIT_32(MSTPCRB_MSTPCRB00)));
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_spi0(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release SPI0 from Module Stop State */
	mmio_write_32(MSTPCRB, mmio_read_32(MSTPCRB) & (~BIT_32(MSTPCRB_MSTPCRB04)));
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	dummy = mmio_read_32(MSTPCRB);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_lcdc(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release LCDC from Module Stop State */
	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM04)));
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_eth(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release Ethernet related modules from Module Stop State */
	mmio_write_32(MSTPCRE, mmio_read_32(MSTPCRE) & (~MSTPCRE_MSTPCRE_ETH_ALL));
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	dummy = mmio_read_32(MSTPCRE);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_ca55(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop */
	sys_safetybase_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release CA55 related modules from Module Stop State */
	mmio_write_32(MSTPCRN, mmio_read_32(MSTPCRN) & (~MSTPCRN_MSTPCRN_CA55_ALL));
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	dummy = mmio_read_32(MSTPCRN);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop Register */
	sys_safetybase_lock(PRCRx_LOW_POWER);
}

static void cpg_mstop_setup(void)
{
	cpg_mstop_cmtw();
	cpg_mstop_sdhi0();	/* eMMC */
	cpg_mstop_sdhi1();	/* eSD */
	cpg_mstop_xspi0();
	cpg_mstop_xspi1();
	cpg_mstop_scif();
	cpg_mstop_usb();
	cpg_mstop_gic();

	/* For Linux only */
	cpg_mstop_pcie();
	cpg_mstop_rtc();
	cpg_mstop_i2c0();
	cpg_mstop_spi0();
	cpg_mstop_lcdc();
	cpg_mstop_eth();
	cpg_mstop_ca55();
}

static void cpg_pll_setup(void)
{
	volatile uint32_t dummy;

	/* Enable write to clock generation circuit Registers */
	sys_safetybase_unlock(PRCRx_CLOCK_GEN);

	/* Setup PLL2 if not already enabled by BootROM */
	if (PLL2EN_PLL2EN_ENABLE != (PLL2EN_PLL2EN_ENABLE & mmio_read_32(PLL2EN))) {
		/* PLL enable */
		mmio_write_32(PLL2EN, PLL2EN_PLL2EN_ENABLE);
		/* wait for PLL Locked */
		while (PLL2MON_PLL2MON_LOCK != (mmio_read_32(PLL2MON) & PLL2MON_PLL2MON_MSK)) {
			/* nothing */
		}
		/* Select PLL clock */
		dummy = mmio_read_32(PMSEL) & (~(PMSEL_PMSEL2_PLL));
		mmio_write_32(PMSEL, dummy | PMSEL_PMSEL2_PLL);
		/* Wait for PLL selected */
		while (PMSEL_PMSEL2_MON_PLL != (mmio_read_32(PMSEL) & PMSEL_PMSEL2_MON_MSK)) {
			/* nothing */
		}
	}

/* Setup PLL3
 * According to TRM Table 7.12 default clock out of PLL will be 297MHz which is then divided
 * by default factor of 2 in SCKCR3.LCDCDIVSEL field therefore giving LCD clock of 148.5MHz
 * Will leave PLL3_VCO_CTR0, PLL3_VCO_CTR1 and CKCR3.LCDCDIVSEL with default values for now.
 */
	mmio_write_32(PLL3EN, PLL3EN_PLL3EN_ENABLE);
	/* wait for PLL Locked */
	while (PLL3MON_PLL3MON_LOCK != (mmio_read_32(PLL3MON) & PLL3MON_PLL3MON_MSK)) {
		__asm__ ("nop");
	}

	/* select PLL clock */
	dummy = mmio_read_32(PMSEL) & (~(PMSEL_PMSEL3_PLL));
	mmio_write_32(PMSEL, dummy | PMSEL_PMSEL3_PLL);
	/* wait for PLL selected */
	while (PMSEL_PMSEL3_MON_PLL != (mmio_read_32(PMSEL) & PMSEL_PMSEL3_MON_MSK)) {
		__asm__ ("nop");
	}

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

static void cpg_reset_eth(void)
{
	volatile uint32_t dummy;

	/* Enable write to Module Stop and Reset Registers */
	sys_base_unlock(PRCRx_LOW_POWER);

	/* Clear bit to release XSPI from Module Reset State */
	mmio_write_32(MRCTLE, mmio_read_32(MRCTLE) & (~MRCTLE_ETHERNET_ALL_MSK));
	/* Dummy read from MRCTLE */
	dummy = mmio_read_32(MRCTLE);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	/* Disable write to Module Stop and Reset Registers */
	sys_base_lock(PRCRx_LOW_POWER);
}

static void cpg_reset_setup(void)
{
	cpg_reset_xspi0();
	cpg_reset_xspi1();
	cpg_reset_eth();
}

void cpg_set_ca55_1200mhz(void)
{
	/* Enable write to SCKCR2 register */
	sys_base_unlock(PRCRx_CLOCK_GEN);

	/* Enable 1200MHz clock on all CA55 cores present */
	mmio_write_32(SCKCR2, ((mmio_read_32(SCKCR2) & (~SCKCR2_CA55_MASK)) | SCKCR2_CA55_MASK));

	/* Disable write to SCKCR2 Register */
	sys_base_lock(PRCRx_CLOCK_GEN);
}

void cpg_early_setup(void)
{
	cpg_set_ca55_1200mhz();
}

void cpg_disable_mpu_protect(void)
{
	/* Disable all MPU */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	mmio_write_32(0x81291400, 0);	//MPU 0 - GMAC Unit 1
	mmio_write_32(0x81291500, 0);	//MPU 1 - GMAC Unit 2
	mmio_write_32(0x81291600, 0);	//MPU 2 - USB Host
	mmio_write_32(0x81291700, 0);	//MPU 3 - USB function
	mmio_write_32(0x81291800, 0);	//MPU 4 - SDHI Unit 0
	mmio_write_32(0x81291900, 0);	//MPU 5 - SDHI Unit 1
	mmio_write_32(0x81291a00, 0);	//MPU 6 - LCD Controller
	mmio_write_32(0x81291b00, 0);	//MPU 7 - PCIE Unit 0
	mmio_write_32(0x81291c00, 0);	//MPU 8 - PCIE Unit 1
	mmio_write_32(0x81291d00, 0);	//MPU 9 - PCIE Unit 2

	mmio_write_32(0x81290400, 0);	//MPU 10 - DMAC Unit 0
	mmio_write_32(0x81290500, 0);	//MPU 11 - DMAC Unit 1
	mmio_write_32(0x81290600, 0);	//MPU 12 - DMAC Unit 2
	mmio_write_32(0x81290700, 0);	//MPU 13 - GMAC Unit 0
	mmio_write_32(0x81290800, 0);	//MPU 14 - SHOSTIF
	mmio_write_32(0x81290900, 0);	//MPU 15 - CoreSight AXI-AP

	sys_safetybase_lock(PRCRx_SYS_CTRL);
}

void cpg_setup(void)
{
	cpg_pll_setup();
	cpg_mstop_setup();
	cpg_reset_setup();
	cpg_disable_mpu_protect();
}

void cpg_ddr_part1(void)
{
	volatile uint32_t dummy;

	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	sys_base_unlock(PRCRx_LOW_POWER);

	mmio_write_32(MRCTLM, 0x03FF0000);
	/* Dummy read from MRCTLM */
	dummy = mmio_read_32(MRCTLM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	mmio_write_32(SSTPCR4, mmio_read_32(SSTPCR4) & (~SSTPCR4_DDRR_ALL_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR4) & SSTPCR4_DDRR_ALL_ACK_MSK)) {
		/* nothing */
	}

	mmio_write_32(SSTPCR0, mmio_read_32(SSTPCR0) & (~SSTPCR0_DDRA_ALL_REQ_MSK));
	while (0U != (mmio_read_32(SSTPCR0) & SSTPCR0_DDRA_ALL_ACK_MSK)) {
		/* nothing */
	}

	mmio_write_32(MSTPCRM, mmio_read_32(MSTPCRM) & (~BIT_32(MSTPCRM_MSTPCRM00)));
	dummy = mmio_read_32(MSTPCRM);
	/* The below is to avoid both a 'checkpatch.pl' and a compile issue "error: variable 'dummy' set but not used [-Werror=unused-but-set-variable]" */
	(void)dummy;

	udelay(1);

	mmio_write_32(MRCTLM, 0x03FC0000);
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
