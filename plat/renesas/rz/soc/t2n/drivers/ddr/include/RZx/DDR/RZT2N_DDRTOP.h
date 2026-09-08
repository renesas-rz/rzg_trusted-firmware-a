/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZT2N_DDRTOP_H__
#define __RZT2N_DDRTOP_H__

#include <stdint.h>
#include <inttypes.h>
#include <lib/mmio.h>

#include "rz_soc_def.h"
#include "RZT2N_DDRTOP_addr.h"

#include "param_ddrinit.h"

#define DDRTOP_OK (0)
#define DDRTOP_ERR (-1)

#define DDRTOP_PRIMARY_RANK 0
#define DDRTOP_SECONDARY_RANK 1
#define DDRTOP_SCL_LANES 3

#define DDRTOP_SCL_ROW_ADDR 0b00000000000000000 /* 17bits */
#define DDRTOP_SCL_COL_ADDR 0b0000000000        /* 10bits */
#define DDRTOP_SCL_BA_ADDR  0b000               /*  3bits */

/* Prototype */
int DDRTOP_proc_init(void);

/* Prototype: sub-functions */
uint8_t unswizzle_bit(uint8_t dq_bit, uint8_t lane);
uint8_t swizzle_bit(uint8_t dq_bit, uint8_t lane);
uint8_t bit_unswizzle(uint8_t int_ip_dq, uint8_t lane);
uint8_t bit_swizzle(uint8_t op_dq, uint8_t lane);
uint8_t ext_unswizzle_ca(unsigned int ca_bit);
uint8_t ext_unswizzle_cs(unsigned int cs_bit);
uint8_t ext_unswizzle_cke(unsigned int cke_bit);
uint8_t ext_unswizzle_bit(uint8_t dq_bit, uint8_t lane);
uint8_t ext_swizzle_bit(uint8_t dq_bit, uint8_t lane);
uint8_t ext_bit_unswizzle(uint8_t ip_dq, uint8_t lane);
uint8_t ext_bit_swizzle(uint8_t op_dq, uint8_t lane);
uint8_t ext_swizzle_ca(unsigned int ca_bit);
uint8_t ext_swizzle_cs(unsigned int cs_bit);
uint8_t sw_bit_unswizzle(uint8_t ip_dq, uint8_t lane);
uint8_t sw_bit_swizzle(uint8_t op_dq, uint8_t lane);
void DDRTOP_proc_MC_APBPOLL(uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_proc_PHY_APBPOLL(uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_mc_apb_wr(uint32_t addr, uint32_t data);
uint32_t DDRTOP_mc_apb_rd(uint32_t addr);
void DDRTOP_mc_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width);
void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
void DDRTOP_MC_MRW(uint8_t mr, uint8_t op, uint8_t cs);
void freq_change_init_lp4(unsigned int target_fsp);
void DDRTOP_proc_switch_fsp(unsigned int target_fsp);
void DDRTOP_init_reset_release(void);
void DDRTOP_init_phy_update_after_training(void);
void DDRTOP_init_mc_update_after_training(void);
int DDRTOP_init_hwdfs_start(unsigned int target_fsp);
void DDRTOP_init_hwdfs_clock_change(void);
int DDRTOP_init_hwdfs_finish(void);
void DDRTOP_init_phy_swizzle(void);
void DDRTOP_init_mc_swizzle(void);
void DDRTOP_init_mc_WRITE_REGMAP(uint32_t proc_status);
void DDRTOP_init_dram_sequence(unsigned int num_ranks);
int DDRTOP_init_read_bit_leveling(uint32_t bit_lvl_from_saved);
int DDRTOP_init_write_bit_leveling(uint32_t bit_lvl_from_saved);
int DDRTOP_init_scl(uint32_t rank, uint32_t bit_lvl_from_saved, uint32_t scl_second_try, uint32_t run_WR_BIT_LEVELING, uint32_t scl_start_addr_X, uint32_t val_NATIVE_BURST8);
void DDRTOP_init_vref_dq_training_wrapper(uint32_t rank, uint32_t num_ranks);
void DDRTOP_init_vref_dq_training(uint32_t DQ_vref_mid_level, uint8_t DQ_vref_mid_level_code, uint8_t DQ_sweep_range, uint8_t DQ_vref_step_size, uint8_t DQ_max_vref_trim_phy, uint32_t DRAM_vref_mid_level, uint8_t DRAM_vref_mid_level_code, uint8_t DRAM_sweep_range, uint8_t DRAM_vref_step_size, uint8_t DRAM_max_vref_trim_phy, uint8_t DRAM_sweep_limit, uint8_t DRAM_mem_chip_selects, uint32_t RUN_PHY_VREF, uint32_t RUN_DRAM_VREF, uint32_t bit_lvl_sample_qty);
void DDRTOP_init_phy_vref_dq_training(uint32_t vref_mid_level, uint32_t vref_mid_level_code, uint32_t sweep_range, uint32_t vref_step_size, uint32_t max_vref_trim_phy, uint32_t best_vref_setting_lane[2]);
void DDRTOP_init_find_vref_window(uint8_t window_diff[2], uint32_t current_vref);
void DDRTOP_init_dram_vref_dq_training(uint32_t vref_mid_level, uint8_t vref_mid_level_code, uint8_t sweep_range, uint8_t vref_step_size, uint8_t max_vref_trim_phy, uint8_t sweep_limit, uint8_t mem_chip_selects, uint32_t best_vref_setting_lane[2]);
void DDRTOP_init_setup_vref_training_registers(uint8_t vref_value, uint8_t mpr_mode, uint8_t turn_on_off_training);
int DDRTOP_init_write_leveling(uint32_t rank, uint32_t bit_lvl_from_saved);
void DDRTOP_phy_init_routine(unsigned int num_ranks);

#endif /* __RZT2N_DDRTOP_H__ */
