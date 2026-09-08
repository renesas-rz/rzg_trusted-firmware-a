/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZT2N_DDRTOP_ADDR_H__
#define __RZT2N_DDRTOP_ADDR_H__

#include "rz_soc_def.h"

/* Controller */
#define START_ADDR                     0
#define START_OFFSET                   0
#define START_WIDTH                    1
#define PHY_INDEP_INIT_MODE_ADDR       22
#define PHY_INDEP_INIT_MODE_OFFSET     8
#define PHY_INDEP_INIT_MODE_WIDTH      1
#define CASLAT_LIN_F1_ADDR             38
#define CASLAT_LIN_F1_OFFSET           16
#define CASLAT_LIN_F1_WIDTH            7
#define WRLAT_F1_ADDR                  38
#define WRLAT_F1_OFFSET                24
#define WRLAT_F1_WIDTH                 7
#define NO_MEMORY_DM_ADDR              61
#define NO_MEMORY_DM_OFFSET            8
#define NO_MEMORY_DM_WIDTH             1
#define AREFRESH_ADDR                  62
#define AREFRESH_OFFSET                8
#define AREFRESH_WIDTH                 1
#define TREF_ENABLE_ADDR               63
#define TREF_ENABLE_OFFSET             0
#define TREF_ENABLE_WIDTH              1
#define LP_AUTO_ENTRY_EN_ADDR          154
#define LP_AUTO_ENTRY_EN_OFFSET        24
#define LP_AUTO_ENTRY_EN_WIDTH         4
#define CURRENT_REG_COPY_ADDR          165
#define CURRENT_REG_COPY_OFFSET        24
#define CURRENT_REG_COPY_WIDTH         2
#define WRITE_MODEREG_ADDR             176
#define WRITE_MODEREG_OFFSET           0
#define WRITE_MODEREG_WIDTH            27
#define MRSINGLE_DATA_0_ADDR           195
#define MRSINGLE_DATA_0_OFFSET         0
#define MRSINGLE_DATA_0_WIDTH          8
#define MRSINGLE_DATA_1_ADDR           195
#define MRSINGLE_DATA_1_OFFSET         8
#define MRSINGLE_DATA_1_WIDTH          8
#define MR2_DATA_F1_0_ADDR             192
#define MR2_DATA_F1_0_OFFSET           24
#define MR2_DATA_F1_0_WIDTH            8
#define MR2_DATA_F1_1_ADDR             194
#define MR2_DATA_F1_1_OFFSET           8
#define MR2_DATA_F1_1_WIDTH            8
#define MR3_DATA_F0_0_ADDR             195
#define MR3_DATA_F0_0_OFFSET           16
#define MR3_DATA_F0_0_WIDTH            8
#define MR3_DATA_F1_0_ADDR             195
#define MR3_DATA_F1_0_OFFSET           24
#define MR3_DATA_F1_0_WIDTH            8
#define MR3_DATA_F0_1_ADDR             196
#define MR3_DATA_F0_1_OFFSET           8
#define MR3_DATA_F0_1_WIDTH            8
#define MR3_DATA_F1_1_ADDR             196
#define MR3_DATA_F1_1_OFFSET           16
#define MR3_DATA_F1_1_WIDTH            8
#define MR11_DATA_F1_0_ADDR            200
#define MR11_DATA_F1_0_OFFSET          24
#define MR11_DATA_F1_0_WIDTH           8
#define MR11_DATA_F1_1_ADDR            201
#define MR11_DATA_F1_1_OFFSET          16
#define MR11_DATA_F1_1_WIDTH           8
#define MR12_DATA_F1_0_ADDR            202
#define MR12_DATA_F1_0_OFFSET          8
#define MR12_DATA_F1_0_WIDTH           8
#define MR12_DATA_F1_1_ADDR            203
#define MR12_DATA_F1_1_OFFSET          0
#define MR12_DATA_F1_1_WIDTH           8
#define MR13_DATA_0_ADDR               203
#define MR13_DATA_0_OFFSET             16
#define MR13_DATA_0_WIDTH              8
#define FSP_OP_CURRENT_ADDR            210
#define FSP_OP_CURRENT_OFFSET          24
#define FSP_OP_CURRENT_WIDTH           1
#define BIST_GO_ADDR                   212
#define BIST_GO_OFFSET                 8
#define BIST_GO_WIDTH                  1
#define ADDR_SPACE_ADDR                212
#define ADDR_SPACE_OFFSET              24
#define ADDR_SPACE_WIDTH               6
#define BIST_DATA_CHECK_ADDR           213
#define BIST_DATA_CHECK_OFFSET         0
#define BIST_DATA_CHECK_WIDTH          1
#define BIST_START_ADDRESS_ADDR        214
#define BIST_START_ADDRESS_OFFSET      0
#define BIST_START_ADDRESS_WIDTH       32
#define BIST_TEST_MODE_ADDR            216
#define BIST_TEST_MODE_OFFSET          0
#define BIST_TEST_MODE_WIDTH           3
#define BIST_DATA_PATTERN_ADDR         217
#define BIST_DATA_PATTERN_OFFSET       0
#define BIST_DATA_PATTERN_WIDTH        64
#define ECC_ENABLE_ADDR                220
#define ECC_ENABLE_OFFSET              24
#define ECC_ENABLE_WIDTH               2
#define ECC_DISABLE_W_UC_ERR_ADDR      221
#define ECC_DISABLE_W_UC_ERR_OFFSET    16
#define ECC_DISABLE_W_UC_ERR_WIDTH     1
#define CS_MAP_ADDR                    301
#define CS_MAP_OFFSET                  24
#define CS_MAP_WIDTH                   2
#define CS_VAL_UPPER_0_ADDR            293
#define CS_VAL_UPPER_0_OFFSET          16
#define CS_VAL_UPPER_0_WIDTH           16
#define CS_VAL_UPPER_1_ADDR            295
#define CS_VAL_UPPER_1_OFFSET          16
#define CS_VAL_UPPER_1_WIDTH           16
#define CONTROLLER_BUSY_ADDR           308
#define CONTROLLER_BUSY_OFFSET         24
#define CONTROLLER_BUSY_WIDTH          1
#define WR_DBI_EN_ADDR                 311
#define WR_DBI_EN_OFFSET               0
#define WR_DBI_EN_WIDTH                1
#define INT_MASK_MASTER_ADDR           314
#define INT_MASK_MASTER_OFFSET         0
#define INT_MASK_MASTER_WIDTH          32
#define INT_STATUS_ECC_ADDR            316
#define INT_STATUS_ECC_OFFSET          0
#define INT_STATUS_ECC_WIDTH           32
#define INT_STATUS_BIST_ADDR           322
#define INT_STATUS_BIST_OFFSET         0
#define INT_STATUS_BIST_WIDTH          8
#define INT_STATUS_INIT_ADDR           323
#define INT_STATUS_INIT_OFFSET         8
#define INT_STATUS_INIT_WIDTH          8
#define INT_ACK_ECC_ADDR               325
#define INT_ACK_ECC_OFFSET             0
#define INT_ACK_ECC_WIDTH              32
#define INT_ACK_BIST_ADDR              331
#define INT_ACK_BIST_OFFSET            0
#define INT_ACK_BIST_WIDTH             8
#define TDFI_PHY_RDLAT_F1_ADDR         515
#define TDFI_PHY_RDLAT_F1_OFFSET       24
#define TDFI_PHY_RDLAT_F1_WIDTH        7

/* Controller additional register */
#define MCAR_CTL        (RZT2N_DDR_MEMC_BASE + 0x00001000)
#define MCAR_MON        (RZT2N_DDR_MEMC_BASE + 0x00001004)
#define MCAR_AXCTL0     (RZT2N_DDR_MEMC_BASE + 0x00001008)
#define MCAR_AXCTL1     (RZT2N_DDR_MEMC_BASE + 0x0000100C)
#define MCAR_AXCTL2     (RZT2N_DDR_MEMC_BASE + 0x00001010)
#define MC_HWIF0        (RZT2N_DDR_MEMC_BASE + 0x00001080)
#define MC_HWIF1        (RZT2N_DDR_MEMC_BASE + 0x00001084)
#define MC_HWDFS0       (RZT2N_DDR_MEMC_BASE + 0x00001088)
#define MC_HWDFS1       (RZT2N_DDR_MEMC_BASE + 0x0000108C)
#define SWIZZLE_SEL_0   (RZT2N_DDR_MEMC_BASE + 0x000010C0)
#define SWIZZLE_SEL_1   (RZT2N_DDR_MEMC_BASE + 0x000010C4)
#define SWIZZLE_SEL_2   (RZT2N_DDR_MEMC_BASE + 0x000010C8)

/* PHY */
#define PHY_OP_DQ_DQS_OEN_TRIM              (RZT2N_DDR_PHY_REG_BASE +  0x18) /* 7'h6 */
#define PHY_SCL_DM_DATA                     (RZT2N_DDR_PHY_REG_BASE +  0x28) /* 7'hA */
#define PHY_PDA_CTRL                        (RZT2N_DDR_PHY_REG_BASE +  0x2C) /* 7'hB */
#define PHY_SCL_DATA_CTRL                   (RZT2N_DDR_PHY_REG_BASE +  0x30) /* 7'hC */
#define PHY_SCL_MASK_0                      (RZT2N_DDR_PHY_REG_BASE +  0x34) /* 7'hD */
#define PHY_PHY_DLL_TRIM_4                  (RZT2N_DDR_PHY_REG_BASE +  0x38) /* 7'hE */
#define PHY_INT_WRLVL_CTRL                  (RZT2N_DDR_PHY_REG_BASE +  0x3C) /* 7'hF */
#define PHY_BIT_LVL_TRIM_OFFSET             (RZT2N_DDR_PHY_REG_BASE +  0x40) /* 7'h10 */
#define PHY_DLL_TEST                        (RZT2N_DDR_PHY_REG_BASE +  0x44) /* 7'h11 */
#define PHY_DLL_TEST_START                  (RZT2N_DDR_PHY_REG_BASE +  0x48) /* 7'h12 */
#define PHY_DLL_TEST_RANGE                  (RZT2N_DDR_PHY_REG_BASE +  0x4C) /* 7'h13 */
#define PHY_DLL_TEST_RESULT                 (RZT2N_DDR_PHY_REG_BASE +  0x50) /* 7'h14 */
#define PHY_A2D_D2A_CONFIG                  (RZT2N_DDR_PHY_REG_BASE +  0x54) /* 7'h15 */
#define PHY_DLLM_WINDOW_SIZE                (RZT2N_DDR_PHY_REG_BASE +  0x58) /* 7'h16 */
#define PHY_CA_TRAINING                     (RZT2N_DDR_PHY_REG_BASE +  0x5C) /* 7'h17 */
#define PHY_BIT_LVL_CONFIG                  (RZT2N_DDR_PHY_REG_BASE +  0x60) /* 7'h18 */
#define PHY_WRLVL_STEP_SIZE                 (RZT2N_DDR_PHY_REG_BASE +  0x64) /* 7'h19 */
#define PHY_MRW_CTRL_DDR4                   (RZT2N_DDR_PHY_REG_BASE +  0x68) /* 7'h1A */
#define PHY_ADRCTRL_PER_BIT_TRIM            (RZT2N_DDR_PHY_REG_BASE +  0x6C) /* 7'h1B */
#define PHY_BIT_LVL_WINDOWS                 (RZT2N_DDR_PHY_REG_BASE +  0x70) /* 7'h1C */
#define PHY_OEN_DLL_OVERRIDE                (RZT2N_DDR_PHY_REG_BASE +  0x74) /* 7'h1D */
#define PHY_MA_DLL_OVERRIDE                 (RZT2N_DDR_PHY_REG_BASE +  0x78) /* 7'h1E */
#define PHY_MCLK_DLL_OVERRIDE               (RZT2N_DDR_PHY_REG_BASE +  0x7C) /* 7'h1F */
#define PHY_ADRCTRL_DLL_OVERRIDE            (RZT2N_DDR_PHY_REG_BASE +  0x80) /* 7'h20 */
#define PHY_DQ_DLL_OVERRIDE                 (RZT2N_DDR_PHY_REG_BASE +  0x84) /* 7'h21 */
#define PHY_DQS_DLL_OVERRIDE                (RZT2N_DDR_PHY_REG_BASE +  0x88) /* 7'h22 */
#define PHY_DM_DLL_OVERRIDE                 (RZT2N_DDR_PHY_REG_BASE +  0x8C) /* 7'h23 */
#define PHY_CA_PER_BIT_TRIM                 (RZT2N_DDR_PHY_REG_BASE +  0x90) /* 7'h24 */
#define PHY_RANK_SEL                        (RZT2N_DDR_PHY_REG_BASE +  0x94) /* 7'h25 */
#define PHY_PHY_DLL_TRIM_OP_DQS             (RZT2N_DDR_PHY_REG_BASE +  0x98) /* 7'h26 */
#define PHY_PHY_PAD_CTRL_3                  (RZT2N_DDR_PHY_REG_BASE +  0x9C) /* 7'h27 */
#define PHY_DPHY_DLL_FORCE_CALIB            (RZT2N_DDR_PHY_REG_BASE +  0xA0) /* 7'h28 */
#define PHY_PHY_DLL_FORCE_CALIB             (RZT2N_DDR_PHY_REG_BASE +  0xA4) /* 7'h29 */
#define PHY_CA_SWIZZLE_POSITION             (RZT2N_DDR_PHY_REG_BASE +  0xA8) /* 7'h2A */
#define PHY_VREF_CA_TRAINING                (RZT2N_DDR_PHY_REG_BASE +  0xC0) /* 7'h30 */
#define PHY_PHY_PAD_CTRL_1                  (RZT2N_DDR_PHY_REG_BASE +  0xC4) /* 7'h31 */
#define PHY_PHY_PAD_CTRL_2                  (RZT2N_DDR_PHY_REG_BASE +  0xC8) /* 7'h32 */
#define PHY_CBT_CTRL                        (RZT2N_DDR_PHY_REG_BASE +  0xCC) /* 7'h33 */
#define PHY_MRW_CTRL                        (RZT2N_DDR_PHY_REG_BASE +  0xD0) /* 7'h34 */
#define PHY_CBT_HARDWARE_CTRL_1             (RZT2N_DDR_PHY_REG_BASE +  0xD4) /* 7'h35 */
#define PHY_CBT_HARDWARE_CTRL_2             (RZT2N_DDR_PHY_REG_BASE +  0xD8) /* 7'h36 */
#define PHY_CBT_HARDWARE_CTRL_3             (RZT2N_DDR_PHY_REG_BASE +  0xDC) /* 7'h37 */
#define PHY_OP_DM_TDQS2DQ                   (RZT2N_DDR_PHY_REG_BASE +  0xE0) /* 7'h38 */
#define PHY_OP_DQ_TDQS2DQ                   (RZT2N_DDR_PHY_REG_BASE +  0xE4) /* 7'h39 */
#define PHY_MRR_DATA                        (RZT2N_DDR_PHY_REG_BASE +  0xE8) /* 7'h3A */
#define PHY_SCL_START                       (RZT2N_DDR_PHY_REG_BASE + 0x100) /* 7'h40 */
#define PHY_SCL_DATA_0                      (RZT2N_DDR_PHY_REG_BASE + 0x104) /* 7'h41 */
#define PHY_SCL_DATA_1                      (RZT2N_DDR_PHY_REG_BASE + 0x108) /* 7'h42 */
#define PHY_SCL_LATENCY                     (RZT2N_DDR_PHY_REG_BASE + 0x10C) /* 7'h43 */
#define PHY_SCL_RD_ADDR                     (RZT2N_DDR_PHY_REG_BASE + 0x110) /* 7'h44 */
#define PHY_SCL_RD_DATA                     (RZT2N_DDR_PHY_REG_BASE + 0x114) /* 7'h45 */
#define PHY_SCL_CONFIG_1                    (RZT2N_DDR_PHY_REG_BASE + 0x118) /* 7'h46 */
#define PHY_SCL_CONFIG_2                    (RZT2N_DDR_PHY_REG_BASE + 0x11C) /* 7'h47 */
#define PHY_PHY_PAD_CTRL                    (RZT2N_DDR_PHY_REG_BASE + 0x120) /* 7'h48 */
#define PHY_PHY_DLL_RECALIB                 (RZT2N_DDR_PHY_REG_BASE + 0x124) /* 7'h49 */
#define PHY_PHY_DLL_ADRCTRL                 (RZT2N_DDR_PHY_REG_BASE + 0x128) /* 7'h4A */
#define PHY_PHY_LANE_SEL                    (RZT2N_DDR_PHY_REG_BASE + 0x12C) /* 7'h4B */
#define PHY_PHY_DLL_TRIM_2                  (RZT2N_DDR_PHY_REG_BASE + 0x134) /* 7'h4D */
#define PHY_PHY_DLL_TRIM_3                  (RZT2N_DDR_PHY_REG_BASE + 0x138) /* 7'h4E */
#define PHY_SCL_DCAPCLK_DLY                 (RZT2N_DDR_PHY_REG_BASE + 0x13C) /* 7'h4F */
#define PHY_SCL_MAIN_CLK_DELTA              (RZT2N_DDR_PHY_REG_BASE + 0x140) /* 7'h50 */
#define PHY_SCL_WINDOW_TRIM                 (RZT2N_DDR_PHY_REG_BASE + 0x1B4) /* 7'h6D */
#define PHY_DISABLE_GATING_FOR_SCL          (RZT2N_DDR_PHY_REG_BASE + 0x1B8) /* 7'h6E */
#define PHY_SCL_GATE_TIMING                 (RZT2N_DDR_PHY_REG_BASE + 0x1E0) /* 7'h78 */
#define PHY_WRLVL_CTRL                      (RZT2N_DDR_PHY_REG_BASE + 0x148) /* 7'h52 */
#define PHY_WRLVL_AUTOINC_TRIM              (RZT2N_DDR_PHY_REG_BASE + 0x14C) /* 7'h53 */
#define PHY_WRLVL_DYN_ODT                   (RZT2N_DDR_PHY_REG_BASE + 0x150) /* 7'h54 */
#define PHY_WRLVL_ON_OFF                    (RZT2N_DDR_PHY_REG_BASE + 0x154) /* 7'h55 */
#define PHY_UNQ_ANALOG_DLL_1                (RZT2N_DDR_PHY_REG_BASE + 0x15C) /* 7'h57 */
#define PHY_UNQ_ANALOG_DLL_2                (RZT2N_DDR_PHY_REG_BASE + 0x160) /* 7'h58 */
#define PHY_UNQ_ANALOG_DLL_3                (RZT2N_DDR_PHY_REG_BASE + 0x190) /* 7'h64 */
#define PHY_PHY_DLL_INCR_TRIM_3             (RZT2N_DDR_PHY_REG_BASE + 0x168) /* 7'h5A */
#define PHY_SCL_CONFIG_3                    (RZT2N_DDR_PHY_REG_BASE + 0x16C) /* 7'h5B */
#define PHY_UNIQUIFY_IO_1                   (RZT2N_DDR_PHY_REG_BASE + 0x170) /* 7'h5C */
#define PHY_UNIQUIFY_IO_2                   (RZT2N_DDR_PHY_REG_BASE + 0x174) /* 7'h5D */
#define PHY_UNIQUIFY_IO_3                   (RZT2N_DDR_PHY_REG_BASE + 0x1CC) /* 7'h73 */
#define PHY_UNIQUIFY_IO_4                   (RZT2N_DDR_PHY_REG_BASE + 0x1D0) /* 7'h74 */
#define PHY_UNIQUIFY_IO_5                   (RZT2N_DDR_PHY_REG_BASE + 0x1F0) /* 7'h7C */
#define PHY_PHY_SCL_START_ADDR              (RZT2N_DDR_PHY_REG_BASE + 0x188) /* 7'h62 */
#define PHY_PHY_DLL_RISE_FALL               (RZT2N_DDR_PHY_REG_BASE + 0x18C) /* 7'h63 */
#define PHY_UNIQUIFY_ADDR_CTRL_LOOPBACK_1   (RZT2N_DDR_PHY_REG_BASE + 0x178) /* 7'h5E */
#define PHY_UNIQUIFY_ADDR_CTRL_LOOPBACK_2   (RZT2N_DDR_PHY_REG_BASE + 0x17C) /* 7'h5F */
#define PHY_IP_DQ_DM_DQS_BITWISE_TRIM       (RZT2N_DDR_PHY_REG_BASE + 0x194) /* 7'h65 */
#define PHY_BIT_LVL_TRIM                    (RZT2N_DDR_PHY_REG_BASE + 0x198) /* 7'h66 */
#define PHY_DSCL_CNT                        (RZT2N_DDR_PHY_REG_BASE + 0x19C) /* 7'h67 */
#define PHY_OP_DQ_DM_DQS_BITWISE_TRIM       (RZT2N_DDR_PHY_REG_BASE + 0x1A0) /* 7'h68 */
#define PHY_PHY_DLL_TRIM_CLK                (RZT2N_DDR_PHY_REG_BASE + 0x1A4) /* 7'h69 */
#define PHY_DYNAMIC_BIT_LVL                 (RZT2N_DDR_PHY_REG_BASE + 0x1AC) /* 7'h6B */
#define PHY_PHY_REV_CNTRL_REG               (RZT2N_DDR_PHY_REG_BASE + 0x1B0) /* 7'h6C */
#define PHY_SCL_CONFIG_4                    (RZT2N_DDR_PHY_REG_BASE + 0x1BC) /* 7'h6F */
#define PHY_DYNAMIC_WRITE_BIT_LVL           (RZT2N_DDR_PHY_REG_BASE + 0x1C0) /* 7'h70 */
#define PHY_DDR4_CONFIG_1                   (RZT2N_DDR_PHY_REG_BASE + 0x1C4) /* 7'h71 */
#define PHY_VREF_TRAINING                   (RZT2N_DDR_PHY_REG_BASE + 0x1C8) /* 7'h72 */
#define PHY_BIT_LVL_MASK                    (RZT2N_DDR_PHY_REG_BASE + 0x1E4) /* 7'h79 */
#define PHY_DYNAMIC_IE_TIMER                (RZT2N_DDR_PHY_REG_BASE + 0x1E8) /* 7'h7A */
#define PHY_DQ_DUTY_CYCLE_FIX               (RZT2N_DDR_PHY_REG_BASE +  0xAC) /* 7'h2B */
#define PHY_DM_DUTY_CYCLE_FIX               (RZT2N_DDR_PHY_REG_BASE + 0x1F4) /* 7'h7D */
#define PHY_DQS_DUTY_CYCLE_FIX              (RZT2N_DDR_PHY_REG_BASE + 0x1F8) /* 7'h7E */
#define PHY_DQS_TRIM                        (RZT2N_DDR_PHY_REG_BASE + 0x1FC) /* 7'h7F */

/* PHY additional register */
#define PHY_MODE0       (RZT2N_DDR_PHY_REG_BASE + 0x00000200)
#define PHY_CTRL0       (RZT2N_DDR_PHY_REG_BASE + 0x00000204)
#define PHY_DMPAT       (RZT2N_DDR_PHY_REG_BASE + 0x00000208)
#define PHY_SPARE2      (RZT2N_DDR_PHY_REG_BASE + 0x0000020C)
#define PHY_CALC0       (RZT2N_DDR_PHY_REG_BASE + 0x00000210)
#define PHY_CALD0       (RZT2N_DDR_PHY_REG_BASE + 0x00000214)
#define PHY_CALD1       (RZT2N_DDR_PHY_REG_BASE + 0x00000218)
#define PHY_CALD2       (RZT2N_DDR_PHY_REG_BASE + 0x0000021C)
#define PHY_CALD3       (RZT2N_DDR_PHY_REG_BASE + 0x00000220)
#define PHY_CALD4       (RZT2N_DDR_PHY_REG_BASE + 0x00000224)
#define PHY_CALD5       (RZT2N_DDR_PHY_REG_BASE + 0x00000228)
#define PHY_CALD6       (RZT2N_DDR_PHY_REG_BASE + 0x0000022C)
#define PHY_CALD7       (RZT2N_DDR_PHY_REG_BASE + 0x00000230)
#define PHY_TDR0        (RZT2N_DDR_PHY_REG_BASE + 0x00000240)
#define PHY_TDR1        (RZT2N_DDR_PHY_REG_BASE + 0x00000244)
#define PHY_TDR2        (RZT2N_DDR_PHY_REG_BASE + 0x00000248)
#define PHY_TDR3        (RZT2N_DDR_PHY_REG_BASE + 0x0000024C)
#define PHY_TDR4        (RZT2N_DDR_PHY_REG_BASE + 0x00000250)
#define PHY_TDR5        (RZT2N_DDR_PHY_REG_BASE + 0x00000254)
#define PHY_TDR6        (RZT2N_DDR_PHY_REG_BASE + 0x00000258)
#define PHY_TDR7        (RZT2N_DDR_PHY_REG_BASE + 0x0000025C)
#define PHY_TDR8        (RZT2N_DDR_PHY_REG_BASE + 0x00000260)
#define PHY_TDR9        (RZT2N_DDR_PHY_REG_BASE + 0x00000264)
#define PHY_TDR10       (RZT2N_DDR_PHY_REG_BASE + 0x00000268)
#define PHY_WRIDLE0     (RZT2N_DDR_PHY_REG_BASE + 0x00000270)
#define PHY_WRIDLE1     (RZT2N_DDR_PHY_REG_BASE + 0x00000274)
#define PHY_WRIDLE2     (RZT2N_DDR_PHY_REG_BASE + 0x00000278)

#endif /* __RZT2N_DDRTOP_ADDR_H__ */
