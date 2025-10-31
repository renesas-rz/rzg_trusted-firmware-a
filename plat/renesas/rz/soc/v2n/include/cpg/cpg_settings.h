#ifndef __CPG_SETTINGS_H__
#define __CPG_SETTINGS_H__

#include "cpg_defs.h"

void cpg_pll_setup(void);
void cpg_div_sel_static_setup(void);
void cpg_div_sel_dynamic_setup(void);

void cpg_awo_set_clk(cpg_clk_state_t clk_state);
void cpg_iso_set_clk(cpg_clk_state_t clk_state);

void cpg_wdt1_set_clk(cpg_clk_state_t clk_state);
#if PLAT_M33_BOOT_SUPPORT
void cpg_cm33_set_clk(cpg_clk_state_t clk_state);
#endif /* PLAT_M33_BOOT_SUPPORT */

void cpg_awo_set_rst(cpg_rst_state_t rst_state);
void cpg_iso_set_rst(cpg_rst_state_t rst_state);
void cpg_wdt1_set_rst(cpg_rst_state_t rst_state);

void cpg_awo_mstop_setup(void);
void cpg_iso_mstop_setup(void);

#if PLAT_SYSTEM_SUSPEND
void cpg_prepare_s2r_mstop(void);
void cpg_enter_s2r_mstop(void);
#endif /* PLAT_SYSTEM_SUSPEND */

#endif /* __CPG_SETTINGS_H__ */
