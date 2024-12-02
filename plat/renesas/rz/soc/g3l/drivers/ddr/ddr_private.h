/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DDR_PRIVATE_H__
#define __DDR_PRIVATE_H__

#include <lib/mmio.h>

#include "ddr_regs.h"

#define DDR4_VERSION	"5.23"
#define LPDDR4_VERSION	"5.23"

#if !LPDDR4
#define DDR_VERSION		DDR4_VERSION
#else
#define DDR_VERSION		LPDDR4_VERSION
#endif

/* DDR misc function */
static inline void DDRTOP_mc_apb_wr(uint32_t addr, uint32_t data)
{
	mmio_write_32(DDR_MC_BASE + (addr << 2), data);
}
static inline uint32_t DDRTOP_mc_apb_rd(uint32_t addr)
{
	return mmio_read_32(DDR_MC_BASE + (addr << 2));
}
static inline void dwc_ddrphy_apb_wr(uint32_t addr, uint32_t data)
{
	mmio_write_32(DDR_PHY_BASE + (addr << 2), data);
}
static inline uint32_t dwc_ddrphy_apb_rd(uint32_t addr)
{
	return mmio_read_32(DDR_PHY_BASE + (addr << 2));
}

extern void DDRTOP_mc_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
extern void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
extern void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
extern uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width);
extern void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);

extern void dwc_ddrphy_phyinit_userCustom_G_waitDone(uint8_t sel_train);
extern void dwc_ddrphy_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
extern void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);

extern uint32_t get_mail(uint8_t mode_32bits);
extern void wait_pclk(uint32_t cycles);
extern void wait_dficlk(uint32_t cycles);

/* DDR setup MC funcion */
extern void setup_mc(void);
extern void update_mc(void);

/* DDR phy init function */
extern void phyinit_configuration(void);
extern void phyinit_pin_swizzling(void);
extern void phyinit_load_1d_image(void);
extern void phyinit_exec_1d_image(void);
extern void phyinit_load_2d_image(void);
extern void phyinit_exec_2d_image(void);
extern void phyinit_load_eng_image(void);

/* DDR Retention Register IO function */
extern void retcsr_read_registers(uint32_t *buffer, size_t size);
extern void retcsr_write_registers(uint32_t *buffer, size_t size);

/* DDR retention ctrl function */
extern void self_refresh_entry(void);

#endif /* __DDR_PRIVATE_H__ */
