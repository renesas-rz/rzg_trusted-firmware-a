#ifndef __CPG_DEFS_H__
#define __CPG_DEFS_H__

/*
 * Write given MSTOP register to remove module stops of bits in given 'val'. Corrosponding MSTOP bit
 * enable in top word needs to be set and just zero to MSTOP bits, therefore **no** Read-Modify-Write is required.
 */
static inline void mstop_apply(uint32_t reg, uint32_t val)
{
	mmio_write_32(reg, (val << 16U) | val);
}

static inline void mstop_remove(uint32_t reg, uint32_t val)
{
	mmio_write_32(reg, val << 16U);
}

typedef enum {
	CPG_RST_ASSERT = 0xFFFF0000u,
	CPG_RST_DEASSERT = 0xFFFFFFFFu
} cpg_rst_state_t;

typedef enum {
	CPG_CLK_DISABLE = 0xFFFF0000u,
	CPG_CLK_ENABLE  = 0xFFFFFFFFu
} cpg_clk_state_t;

typedef struct {
	uintptr_t addr;
	uint32_t  val;
} cpg_reg_setting_t;

typedef struct {
	cpg_reg_setting_t reg;
	cpg_reg_setting_t mon;
} cpg_setup_data_t;

typedef cpg_setup_data_t cpg_rst_data_t;

typedef cpg_setup_data_t cpg_clk_data_t;

typedef struct {
	cpg_reg_setting_t stby;
	cpg_reg_setting_t clk1;
	cpg_reg_setting_t clk2;
	cpg_reg_setting_t mon;
} cpg_pll_settings_t;

#endif /* __CPG_H__ */
