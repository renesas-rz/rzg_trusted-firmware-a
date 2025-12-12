#include <lib/mmio.h>

#include "cpg_settings.h"
#include "cpg_regs.h"

/* Set MSTOPS in the AWO power domain */
void cpg_awo_pd_mstop_setup(void)
{
	/* Remove all MSTOPS apart from reserved and those already removed at TF-A entry */
	mstop_remove(CPG_BUS_3_MSTOP,       CPG_BUS_3_MSTOP_ADC
										| CPG_BUS_3_MSTOP_WDT0
										| CPG_BUS_3_MSTOP_RTC_P0
										| CPG_BUS_3_MSTOP_RTC_P1
										| CPG_BUS_3_MSTOP_RIIC8
										| CPG_BUS_3_MSTOP_SCIF
										| CPG_BUS_3_MSTOP_CMTW0);

	mstop_remove(CPG_BUS_4_MSTOP,      CPG_BUS_4_MSTOP_CMTW1
										| CPG_BUS_4_MSTOP_CMTW2
										| CPG_BUS_4_MSTOP_CMTW3
										| CPG_BUS_4_MSTOP_XSPI
										| CPG_BUS_4_MSTOP_SECURE_IP_P0
										| CPG_BUS_4_MSTOP_SECURE_IP_P1
										| CPG_BUS_4_MSTOP_SRAM0
										| CPG_BUS_4_MSTOP_SRAM1
										| CPG_BUS_4_MSTOP_MHU);

	mstop_remove(CPG_BUS_5_MSTOP,      CPG_BUS_5_MSTOP_TSU0
										| CPG_BUS_5_MSTOP_XSPI_REG
										| CPG_BUS_5_MSTOP_PDM0
										| CPG_BUS_5_MSTOP_PDM1
										| CPG_BUS_5_MSTOP_DMAC0
										| CPG_BUS_5_MSTOP_GTM0
										| CPG_BUS_5_MSTOP_GTM1);

	mstop_remove(CPG_BUS_12_MSTOP,     CPG_BUS_12_MSTOP_GTM7
										| CPG_BUS_12_MSTOP_ACPU_TO_MCPU
										| CPG_BUS_12_MSTOP_MCPU_TO_ACPU);
}

/* Set MSTOPS in the OTHERS power domain */
void cpg_others_pd_mstop_setup(void)
{
	/* Remove all MSTOPS apart from reserved and those already removed at TF-A entry */
	mstop_remove(CPG_BUS_1_MSTOP,      CPG_BUS_1_MSTOP_WDT1
										| CPG_BUS_1_MSTOP_RIIC0
										| CPG_BUS_1_MSTOP_RIIC1
										| CPG_BUS_1_MSTOP_RIIC2
										| CPG_BUS_1_MSTOP_RIIC3
										| CPG_BUS_1_MSTOP_RIIC4
										| CPG_BUS_1_MSTOP_RIIC5
										| CPG_BUS_1_MSTOP_RIIC6
										| CPG_BUS_1_MSTOP_RIIC7
										| CPG_BUS_1_MSTOP_SPDIF0
										| CPG_BUS_1_MSTOP_SPDIF1
										| CPG_BUS_1_MSTOP_SPDIF2
										| CPG_BUS_1_MSTOP_TZC400_PCIE);

	mstop_remove(CPG_BUS_2_MSTOP,      CPG_BUS_2_MSTOP_SCU
										| CPG_BUS_2_MSTOP_SCU_DMAC
										| CPG_BUS_2_MSTOP_ADG
										| CPG_BUS_2_MSTOP_SSIU
										| CPG_BUS_2_MSTOP_SSIU_DMAC
										| CPG_BUS_2_MSTOP_ADMAC
										| CPG_BUS_2_MSTOP_GTM2
										| CPG_BUS_2_MSTOP_GTM3
										| CPG_BUS_2_MSTOP_TSU1);

	mstop_remove(CPG_BUS_3_MSTOP,      CPG_BUS_3_MSTOP_DMAC1
										| CPG_BUS_3_MSTOP_DMAC2
										| CPG_BUS_3_MSTOP_GE3D);

	mstop_remove(CPG_BUS_4_MSTOP,      CPG_BUS_4_MSTOP_CMTW1
										| CPG_BUS_4_MSTOP_CMTW2
										| CPG_BUS_4_MSTOP_CMTW3
										| CPG_BUS_4_MSTOP_XSPI
										| CPG_BUS_4_MSTOP_SECURE_IP_P0
										| CPG_BUS_4_MSTOP_SECURE_IP_P1);

	mstop_remove(CPG_BUS_5_MSTOP,      CPG_BUS_5_MSTOP_WDT2
										| CPG_BUS_5_MSTOP_WDT3
										| CPG_BUS_5_MSTOP_CRC
										| CPG_BUS_5_MSTOP_CMTW4);

	mstop_remove(CPG_BUS_6_MSTOP,      CPG_BUS_6_MSTOP_CMTW5
										| CPG_BUS_6_MSTOP_CMTW6
										| CPG_BUS_6_MSTOP_CMTW7
										| CPG_BUS_6_MSTOP_POEG0A
										| CPG_BUS_6_MSTOP_POEG0B
										| CPG_BUS_6_MSTOP_POEG0C
										| CPG_BUS_6_MSTOP_POEG0D
										| CPG_BUS_6_MSTOP_POEG1A
										| CPG_BUS_6_MSTOP_POEG1B
										| CPG_BUS_6_MSTOP_POEG1C
										| CPG_BUS_6_MSTOP_POEG1D
										| CPG_BUS_6_MSTOP_GPT0
										| CPG_BUS_6_MSTOP_GPT1
										| CPG_BUS_6_MSTOP_DDR0_P0
										| CPG_BUS_6_MSTOP_DDR0_P1
										| CPG_BUS_6_MSTOP_DDR0_P2);

	mstop_remove(CPG_BUS_7_MSTOP,      CPG_BUS_7_MSTOP_DDR_0_P3
										| CPG_BUS_7_MSTOP_DDR_0_P4
										| CPG_BUS_7_MSTOP_USB20_HOST
										| CPG_BUS_7_MSTOP_USB2_FUNC
										| CPG_BUS_7_MSTOP_USB20_PHY
										| CPG_BUS_7_MSTOP_USB30_HOST
										| CPG_BUS_7_MSTOP_USB30_PHY);

	mstop_remove(CPG_BUS_8_MSTOP,      CPG_BUS_8_MSTOP_SD0
										| CPG_BUS_8_MSTOP_SD1
										| CPG_BUS_8_MSTOP_SD2
										| CPG_BUS_8_MSTOP_GBETH0
										| CPG_BUS_8_MSTOP_GBETH1
										| CPG_BUS_8_MSTOP_DRP_AI_MAC
										| CPG_BUS_8_MSTOP_DRP_AP_DRP0);

	mstop_remove(CPG_BUS_9_MSTOP,      CPG_BUS_9_MSTOP_CRU0
										| CPG_BUS_9_MSTOP_CRU1
										| CPG_BUS_9_MSTOP_ISP_APB
										| CPG_BUS_9_MSTOP_ISP_AXI
										| CPG_BUS_9_MSTOP_VCD_P0
										| CPG_BUS_9_MSTOP_VCD_P1
										| CPG_BUS_9_MSTOP_VCD_P2
										| CPG_BUS_9_MSTOP_DSI_LINK
										| CPG_BUS_9_MSTOP_DSI_PHY);

	mstop_remove(CPG_BUS_10_MSTOP,     CPG_BUS_10_MSTOP_ISU
										| CPG_BUS_10_MSTOP_LCDC_DU
										| CPG_BUS_10_MSTOP_LCDC_FCPVD
										| CPG_BUS_10_MSTOP_LCDC_VSPD
										| CPG_BUS_10_MSTOP_DDR0_PHY
										| CPG_BUS_10_MSTOP_DDR0_CTRL
										| CPG_BUS_10_MSTOP_DMAC3
										| CPG_BUS_10_MSTOP_DMAC4
										| CPG_BUS_10_MSTOP_CANFD
										| CPG_BUS_10_MSTOP_I3C0);

	mstop_remove(CPG_BUS_11_MSTOP,     CPG_BUS_11_MSTOP_RSPI0
										| CPG_BUS_11_MSTOP_RSPI1
										| CPG_BUS_11_MSTOP_RSPI2
										| CPG_BUS_11_MSTOP_RSCI0
										| CPG_BUS_11_MSTOP_RSCI1
										| CPG_BUS_11_MSTOP_RSCI2
										| CPG_BUS_11_MSTOP_RSCI3
										| CPG_BUS_11_MSTOP_RSCI4
										| CPG_BUS_11_MSTOP_RSCI5
										| CPG_BUS_11_MSTOP_RSCI6
										| CPG_BUS_11_MSTOP_RSCI7
										| CPG_BUS_11_MSTOP_RSCI8
										| CPG_BUS_11_MSTOP_RSCI9
										| CPG_BUS_11_MSTOP_GTM4
										| CPG_BUS_11_MSTOP_GTM5
										| CPG_BUS_11_MSTOP_GTM6);
}

#if PLAT_SYSTEM_SUSPEND
/* Re-init required mstops, that might have been deinit by other software. */
void cpg_prepare_s2r_mstop(void)
{
	mstop_remove(CPG_BUS_3_MSTOP,      CPG_BUS_3_MSTOP_RIIC8);
}

/* Remove mstops for devices in ISO domain that are no longer required. */
void cpg_enter_s2r_mstop(void)
{
	mstop_apply(CPG_BUS_1_MSTOP,      CPG_BUS_1_MSTOP_WDT1
										| CPG_BUS_1_MSTOP_RIIC0
										| CPG_BUS_1_MSTOP_RIIC1
										| CPG_BUS_1_MSTOP_RIIC2
										| CPG_BUS_1_MSTOP_RIIC3
										| CPG_BUS_1_MSTOP_RIIC4
										| CPG_BUS_1_MSTOP_RIIC5
										| CPG_BUS_1_MSTOP_RIIC6
										| CPG_BUS_1_MSTOP_RIIC7
										| CPG_BUS_1_MSTOP_SPDIF0
										| CPG_BUS_1_MSTOP_SPDIF1
										| CPG_BUS_1_MSTOP_SPDIF2
										| CPG_BUS_1_MSTOP_TZC400_PCIE);

	mstop_apply(CPG_BUS_2_MSTOP,      CPG_BUS_2_MSTOP_SCU
										| CPG_BUS_2_MSTOP_SCU_DMAC
										| CPG_BUS_2_MSTOP_ADG
										| CPG_BUS_2_MSTOP_SSIU
										| CPG_BUS_2_MSTOP_SSIU_DMAC
										| CPG_BUS_2_MSTOP_ADMAC
										| CPG_BUS_2_MSTOP_GTM2
										| CPG_BUS_2_MSTOP_GTM3
										| CPG_BUS_2_MSTOP_TSU1);

	mstop_apply(CPG_BUS_3_MSTOP,      CPG_BUS_3_MSTOP_DMAC1
										| CPG_BUS_3_MSTOP_DMAC2
										| CPG_BUS_3_MSTOP_GE3D);

	mstop_apply(CPG_BUS_4_MSTOP,      CPG_BUS_4_MSTOP_CMTW1
										| CPG_BUS_4_MSTOP_CMTW2
										| CPG_BUS_4_MSTOP_CMTW3
										| CPG_BUS_4_MSTOP_XSPI
										| CPG_BUS_4_MSTOP_SECURE_IP_P0
										| CPG_BUS_4_MSTOP_SECURE_IP_P1);

	mstop_apply(CPG_BUS_5_MSTOP,      CPG_BUS_5_MSTOP_WDT2
										| CPG_BUS_5_MSTOP_WDT3
										| CPG_BUS_5_MSTOP_CRC
										| CPG_BUS_5_MSTOP_CMTW4);

	mstop_apply(CPG_BUS_6_MSTOP,      CPG_BUS_6_MSTOP_CMTW5
										| CPG_BUS_6_MSTOP_CMTW6
										| CPG_BUS_6_MSTOP_CMTW7
										| CPG_BUS_6_MSTOP_POEG0A
										| CPG_BUS_6_MSTOP_POEG0B
										| CPG_BUS_6_MSTOP_POEG0C
										| CPG_BUS_6_MSTOP_POEG0D
										| CPG_BUS_6_MSTOP_POEG1A
										| CPG_BUS_6_MSTOP_POEG1B
										| CPG_BUS_6_MSTOP_POEG1C
										| CPG_BUS_6_MSTOP_POEG1D
										| CPG_BUS_6_MSTOP_GPT0
										| CPG_BUS_6_MSTOP_GPT1
										| CPG_BUS_6_MSTOP_DDR0_P0
										| CPG_BUS_6_MSTOP_DDR0_P1
										| CPG_BUS_6_MSTOP_DDR0_P2);

	mstop_apply(CPG_BUS_7_MSTOP,      CPG_BUS_7_MSTOP_DDR_0_P3
										| CPG_BUS_7_MSTOP_DDR_0_P4
										| CPG_BUS_7_MSTOP_USB20_HOST
										| CPG_BUS_7_MSTOP_USB2_FUNC
										| CPG_BUS_7_MSTOP_USB20_PHY
										| CPG_BUS_7_MSTOP_USB30_HOST
										| CPG_BUS_7_MSTOP_USB30_PHY);

	mstop_apply(CPG_BUS_8_MSTOP,      CPG_BUS_8_MSTOP_SD0
										| CPG_BUS_8_MSTOP_SD1
										| CPG_BUS_8_MSTOP_SD2
										| CPG_BUS_8_MSTOP_GBETH0
										| CPG_BUS_8_MSTOP_GBETH1
										| CPG_BUS_8_MSTOP_DRP_AI_MAC
										| CPG_BUS_8_MSTOP_DRP_AP_DRP0);

	mstop_apply(CPG_BUS_9_MSTOP,      CPG_BUS_9_MSTOP_CRU0
										| CPG_BUS_9_MSTOP_CRU1
										| CPG_BUS_9_MSTOP_ISP_APB
										| CPG_BUS_9_MSTOP_ISP_AXI
										| CPG_BUS_9_MSTOP_VCD_P0
										| CPG_BUS_9_MSTOP_VCD_P1
										| CPG_BUS_9_MSTOP_VCD_P2
										| CPG_BUS_9_MSTOP_DSI_LINK
										| CPG_BUS_9_MSTOP_DSI_PHY);

	mstop_apply(CPG_BUS_10_MSTOP,     CPG_BUS_10_MSTOP_ISU
										| CPG_BUS_10_MSTOP_LCDC_DU
										| CPG_BUS_10_MSTOP_LCDC_FCPVD
										| CPG_BUS_10_MSTOP_LCDC_VSPD
										| CPG_BUS_10_MSTOP_DDR0_PHY
										| CPG_BUS_10_MSTOP_DDR0_CTRL
										| CPG_BUS_10_MSTOP_DMAC3
										| CPG_BUS_10_MSTOP_DMAC4
										| CPG_BUS_10_MSTOP_CANFD
										| CPG_BUS_10_MSTOP_I3C0);

	mstop_apply(CPG_BUS_11_MSTOP,     CPG_BUS_11_MSTOP_RSPI0
										| CPG_BUS_11_MSTOP_RSPI1
										| CPG_BUS_11_MSTOP_RSPI2
										| CPG_BUS_11_MSTOP_RSCI0
										| CPG_BUS_11_MSTOP_RSCI1
										| CPG_BUS_11_MSTOP_RSCI2
										| CPG_BUS_11_MSTOP_RSCI3
										| CPG_BUS_11_MSTOP_RSCI4
										| CPG_BUS_11_MSTOP_RSCI5
										| CPG_BUS_11_MSTOP_RSCI6
										| CPG_BUS_11_MSTOP_RSCI7
										| CPG_BUS_11_MSTOP_RSCI8
										| CPG_BUS_11_MSTOP_RSCI9
										| CPG_BUS_11_MSTOP_GTM4
										| CPG_BUS_11_MSTOP_GTM5
										| CPG_BUS_11_MSTOP_GTM6);

}
#endif /* PLAT_SYSTEM_SUSPEND */
