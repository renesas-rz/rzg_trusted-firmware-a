/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/delay_timer.h>
#include <common/debug.h>

#include <ddr_internal.h>
#include <cpg.h>
#include <ddr_mc_regs.h>
#include <ddr_phy_regs.h>

#define	CEIL(a, div)	(((a) + ((div) - 1)) / (div))
#define	_MIN(a, b)		((a) < (b) ? (a) : (b))
#define	_MAX(a, b)		((a) > (b) ? (a) : (b))

#define	MAX_BYTE_LANES		(2U)
#define	MAX_BEST_VREF_SAVED	(30U)
#define	VREF_SETP			(1U)

extern const uint32_t mc_init_tbl[MC_INIT_NUM][2];
extern const uint32_t mc_odt_pins_tbl[4];
extern const uint32_t mc_mr1_tbl[2];
extern const uint32_t mc_mr2_tbl[2];
extern const uint32_t mc_mr5_tbl[2];
extern const uint32_t mc_mr6_tbl[2];
extern const uint32_t mc_phy_settings_tbl[MC_PHYSET_NUM][2];
extern const uint32_t swizzle_mc_tbl[SWIZZLE_MC_NUM][2];
extern const uint32_t swizzle_phy_tbl[SIZZLE_PHY_NUM][2];
extern const char ddr_an_version[];

void ddr_setup(void);
static void disable_phy_clk(void);
static void program_mc1(uint8_t *lp_auto_entry_en);
static void program_phy1(uint32_t sl_lanes, uint32_t byte_lanes);
static void exec_trainingWRLVL(uint32_t sl_lanes);
static void exec_trainingVREF(uint32_t sl_lanes, uint32_t byte_lanes);
static void setup_vref_training_registers(uint8_t vref_value, uint8_t cs, uint8_t turn_on_off_vref_training);
static void write_mr(uint8_t cs, uint8_t mrw_sel, uint16_t mrw_data);
static void exec_trainingBITLVL(uint32_t sl_lanes);
static void opt_delay(uint32_t sl_lanes, uint32_t byte_lanes);
static void exec_trainingSL(uint32_t sl_lanes);
static void program_phy2(void);
static void program_mc2(void);

#if (DDR_ECC_ENABLE == 1)
#define DDR_BASE_ADDRESS	0x40000000
#define TEST_BIT_NO			0
#define USER_WORD_DATA		0x5a5a5a5a5a5a5a5a
#define ECC_ERR_ADDRESS		0x60000000
static int ecc_force_ce_error(void);
static void ecc_prog_all0(uint64_t addr_start, uint64_t addr_end);
static void init_ecc(void);
#if (DDR_ECC_DETECT_CORRECT == 1)
char data_synd[] = {
			0xf4, 0xf1, 0xec, 0xea, 0xe9, 0xe6, 0xe5, 0xe3,
			0xdc, 0xda, 0xd9, 0xd6, 0xd5, 0xd3, 0xce, 0xcb,
			0xb5, 0xb0, 0xad, 0xab, 0xa8, 0xa7, 0xa4, 0xa2,
			0x9d, 0x9b, 0x98, 0x97, 0x94, 0x92, 0x8f, 0x8a,
			0x75, 0x70, 0x6d, 0x6b, 0x68, 0x67, 0x64, 0x62,
			0x5e, 0x5b, 0x58, 0x57, 0x54, 0x52, 0x4f, 0x4a,
			0x34, 0x31, 0x2c, 0x2a, 0x29, 0x26, 0x25, 0x23,
			0x1c, 0x1a, 0x19, 0x16, 0x15, 0x13, 0x0e, 0x0b
		  };
#endif
#endif

void ddr_setup(void)
{
	uint32_t	sl_lanes, byte_lanes;
	uint8_t		runBITLVL, runSL, runVREF;
	uint8_t		lp_auto_entry_en = 0;
	uint32_t	tmp;
	int i;

	INFO("BL2: setup DDR (Rev. %s)\n", ddr_an_version);
	/* Step 2-11 */
	cpg_active_ddr(disable_phy_clk);

	/* Step 12 */
	program_mc1(&lp_auto_entry_en);

	/* Step 13 */
	tmp = read_mc_field(MEM_DP_REDUCTION_ADDR, MEM_DP_REDUCTION_WIDTH, MEM_DP_REDUCTION_OFFSET);
	sl_lanes	= ((tmp & 0x1) == 0) ? 3 : 1;
	byte_lanes	= ((tmp & 0x1) == 0) ? 2 : 1;
	tmp = read_mc_reg(USER_DEF_REG_10_ADDR);
	runBITLVL	= (tmp >> 20) & 0x1;
	runSL		= (tmp >> 21) & 0x1;
	runVREF		= (tmp >> 25) & 0x1;

	/* Step 14 */
	program_phy1(sl_lanes, byte_lanes);

	/* Step 15 */
	while ((read_phy_reg(DDRPHY_R42) & 0x00000003) != sl_lanes)
		;

	/* Step 16 */
	ddr_ctrl_reten_en_n(0);

	rmw_mc_set_field(LPC_SR_ZQ_EN_ADDR, LPC_SR_ZQ_EN_WIDTH, LPC_SR_ZQ_EN_OFFSET, 0x0);
	rmw_mc_set_field(PHY_INDEP_INIT_MODE_ADDR, PHY_INDEP_INIT_MODE_WIDTH, PHY_INDEP_INIT_MODE_OFFSET, 0x1);
	rmw_mc_set_field(START_ADDR, START_WIDTH, START_OFFSET, 0x1);

	while (read_mc_field(INT_STATUS_INIT_ADDR, 0x1, INT_STATUS_INIT_OFFSET + 1) != 0x1)
		;

	rmw_phy_reg(DDRPHY_R74, 0xFFF7FFFF, 0x00080000);
	rmw_mc_set_field(TDFI_CTRLUPD_MIN_ADDR, TDFI_CTRLUPD_MIN_WIDTH, TDFI_CTRLUPD_MIN_OFFSET, 64);
	rmw_mc_set_field(TDFI_CTRLUPD_MAX_ADDR, TDFI_CTRLUPD_MAX_WIDTH, TDFI_CTRLUPD_MAX_OFFSET, 111);
	rmw_mc_set_field(CTRLUPD_REQ_ADDR, CTRLUPD_REQ_WIDTH, CTRLUPD_REQ_OFFSET, 0x1);
	udelay(1);
	rmw_phy_reg(DDRPHY_R74, 0xFFF7FFFF, 0x00000000);

	/* Step 17 */
	cpg_reset_ddr_mc();
	ddr_ctrl_reten_en_n(1);

	/* Step 18-19 */
	program_mc1(&lp_auto_entry_en);

	/* Step 20 */
	for (i = 0; i < ARRAY_SIZE(swizzle_mc_tbl); i++) {
		write_mc_reg(swizzle_mc_tbl[i][0], swizzle_mc_tbl[i][1]);
	}
	for (i = 0; i < ARRAY_SIZE(swizzle_phy_tbl); i++) {
		write_phy_reg(swizzle_phy_tbl[i][0], swizzle_phy_tbl[i][1]);
	}

	/* Step 21 */
	rmw_mc_set_field(START_ADDR, START_WIDTH, START_OFFSET, 0x1);

	/* Step 22 */
	while (read_mc_field(INT_STATUS_INIT_ADDR, 0x1, INT_STATUS_INIT_OFFSET + 1) != 0x1)
		;

	/* Step 23 */
	rmw_mc_set_field(INT_ACK_INIT_ADDR, 0x1, INT_ACK_INIT_OFFSET + 1, 0x1);

	/* Step 24 */
	exec_trainingWRLVL(sl_lanes);

	/* Step 25 */
	if (runVREF == 1)
		exec_trainingVREF(sl_lanes, byte_lanes);

	/* Step 26 */
	if (runBITLVL == 1)
		exec_trainingBITLVL(sl_lanes);

	/* Step 27 */
	opt_delay(sl_lanes, byte_lanes);

	/* Step 28 */
	if (runSL == 1)
		exec_trainingSL(sl_lanes);

	/* Step 29 */
	program_phy2();

	/* Step 30 */
	program_mc2();

	/* Step 31 */
#if (DDR_ECC_ENABLE == 1)
#if (DDR_ECC_DETECT_CORRECT == 1)
	NOTICE("BL2: ECC MODE: Error Detect and Correct\n");
#elif (DDR_ECC_DETECT == 1)
	NOTICE("BL2: ECC MODE: Error Detect\n");
#else
	NOTICE("BL2: ECC MODE: Enable\n");
#endif

	init_ecc();

	NOTICE("ECC INIT DONE\n");
#endif
	/*
	 * Step 32
	 * let the auto_exit_en to be value|0x8
	 * recommended value for "value" is 0x0
	 */
	rmw_mc_set_field(LP_AUTO_EXIT_EN_ADDR, LP_AUTO_EXIT_EN_WIDTH, LP_AUTO_EXIT_EN_OFFSET, 0x8);
	rmw_mc_set_field(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_WIDTH, LP_AUTO_ENTRY_EN_OFFSET, lp_auto_entry_en);

#if (DDR_ECC_ENABLE == 1)
	/* Extra step: test ECC CE function */
	NOTICE("BL2: ECC CE function testing ....\n");
	if (ecc_force_ce_error())
		ERROR("DDR ECC Force CE FAILED\n");
	else
		NOTICE("OK\n");
#endif
}

#if (DDR_ECC_ENABLE == 1)
#if (DDR_ECC_DETECT_CORRECT == 1)
static int ecc_force_ce_error(void)
{
	int ret;
	uint64_t err_addr;
	uint64_t *user_word;
	uint32_t tmp, synd, xor_check_code;
	uint32_t retry = 0xffffffU;
	uint32_t bak_DDRMC_R052;

	bak_DDRMC_R052 = read_mc_reg(DDRMC_R052);

	/* make checkcode */
	xor_check_code = (data_synd[0] << 8) | 1;
	user_word = (uint64_t *)ECC_ERR_ADDRESS;

	do {
		tmp = read_mc_reg(DDRMC_R020);
	} while (tmp & 1);

	rmw_mc_reg(DDRMC_R052, ~((0xff << 8) | 1), xor_check_code);

	*user_word = USER_WORD_DATA;
	flush_dcache_range((unsigned long)user_word, sizeof(uint64_t));
	(*(const volatile uint64_t *)user_word);

	rmw_mc_reg(DDRMC_R052, ~((0xff << 8) | 1), 0);

	do {
		tmp = read_mc_reg(DDRMC_R064);
	} while ((retry--) && !(tmp & 0xffffU));

	/* out of retry or not CE */
	if (!retry || !(tmp & 0x3)) {
		ERROR("DDR error: out of retry or not CE.\n");
		ret = -1;
		goto err;
	}

	/* ack ecc int */
	tmp = 0xffffU;
	write_mc_reg(DDRMC_R066, tmp);

	/* check the error address */
	err_addr = (uint64_t)(read_mc_reg(DDRMC_R054) & 0x3);
	err_addr = err_addr << 32;
	err_addr |= (uint64_t)read_mc_reg(DDRMC_R053);
	err_addr += DDR_BASE_ADDRESS;
	if (((uint64_t)user_word) != err_addr) {
		ret = -1;
		ERROR("DDR error: ECC error address mismatch.\n");
		goto err;
	}

	/* check error synd */
	synd = read_mc_reg(DDRMC_R054);
	synd = (synd >> 8) & 0xff;
	if (synd != (xor_check_code >> 8)) {
		ret = -1;
		ERROR("DDR error: ECC error synd mismatch.\n");
		goto err;
	}

	if ((*user_word ^ USER_WORD_DATA) != BIT(TEST_BIT_NO)) {
		ret = -1;
		ERROR("DDR error: ECC error data mismatch.\n");
		goto err;
	}

	write_mc_reg(DDRMC_R052, bak_DDRMC_R052);

	return 0;

err:
	write_mc_reg(DDRMC_R052, bak_DDRMC_R052);

	return ret;
}
#else
static int ecc_force_ce_error(void)
{
	return 0;
}
#endif

static void ecc_prog_all0(uint64_t addr_start, uint64_t addr_end)
{
	INFO("BL2: ECC prog all0 start\n");

	int i;
	uint32_t val;
	uint64_t addr, prog_size;
	uint32_t bak_lp_auto_entry_en, bak_in_order_accept;

	/* Step 1 */
	addr = addr_start - DDR_BASE_ADDRESS;
	prog_size = addr_end - addr_start + 1;

	/* Step 2 */
	bak_lp_auto_entry_en = read_mc_field(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_WIDTH, LP_AUTO_ENTRY_EN_OFFSET);
	rmw_mc_set_field(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_WIDTH, LP_AUTO_ENTRY_EN_OFFSET, 0x0);

	bak_in_order_accept = read_mc_field(IN_ORDER_ACCEPT_ADDR, IN_ORDER_ACCEPT_WIDTH, IN_ORDER_ACCEPT_OFFSET);
	rmw_mc_set_field(IN_ORDER_ACCEPT_ADDR, IN_ORDER_ACCEPT_WIDTH, IN_ORDER_ACCEPT_OFFSET, 0x1);

	/* Step 3 */
	rmw_mc_set_field(BIST_DATA_CHECK_ADDR, BIST_DATA_CHECK_WIDTH, BIST_DATA_CHECK_OFFSET, 0x1);
	rmw_mc_set_field(BIST_ADDR_CHECK_ADDR, BIST_ADDR_CHECK_WIDTH, BIST_ADDR_CHECK_OFFSET, 0x0);
	rmw_mc_set_field(BIST_TEST_MODE_ADDR, BIST_TEST_MODE_WIDTH, BIST_TEST_MODE_OFFSET, 0x4);

	rmw_mc_set_field(BIST_DATA_PATTERN_0_ADDR, BIST_DATA_PATTERN_0_WIDTH, BIST_DATA_PATTERN_0_OFFSET, 0x0);
	rmw_mc_set_field(BIST_DATA_PATTERN_1_ADDR, BIST_DATA_PATTERN_1_WIDTH, BIST_DATA_PATTERN_1_OFFSET, 0x0);
	rmw_mc_set_field(INT_MASK_BIST_ADDR, INT_MASK_BIST_WIDTH, INT_MASK_BIST_OFFSET, 0x1);


	/* Step 4 */
	for (i = 0 ; i < 34 ; i++) {
		if (((prog_size >> i) & 1) == 1) {
			rmw_mc_set_field(BIST_START_ADDRESS_0_ADDR, BIST_START_ADDRESS_0_WIDTH, BIST_START_ADDRESS_0_OFFSET, addr & 0xffffffff);
			rmw_mc_set_field(BIST_START_ADDRESS_1_ADDR, BIST_START_ADDRESS_1_WIDTH, BIST_START_ADDRESS_1_OFFSET, (addr >> 32) & 0x3);
			rmw_mc_set_field(ADDR_SPACE_ADDR, ADDR_SPACE_WIDTH, ADDR_SPACE_OFFSET, i);

			mdelay(10);

			rmw_mc_set_field(BIST_GO_ADDR, BIST_GO_WIDTH, BIST_GO_OFFSET, 1);
			do {

			} while (!(read_mc_field(INT_STATUS_BIST_ADDR, INT_STATUS_0_BIST_WIDTH, INT_STATUS_0_BIST_OFFSET)));

			rmw_mc_set_field(BIST_GO_ADDR, BIST_GO_WIDTH, BIST_GO_OFFSET, 0);

			rmw_mc_set_field(INT_ACK_BIST_ADDR, INT_ACK_0_BIST_WIDTH, INT_ACK_0_BIST_OFFSET, 1);
			do {

			} while (read_mc_field(INT_STATUS_BIST_ADDR, INT_STATUS_0_BIST_WIDTH, INT_STATUS_0_BIST_OFFSET));

			addr += (1 << i);
		}
	}

	/* Step 5 */
	rmw_mc_set_field(INT_ACK_ECC_ADDR, INT_ACK_ECC_WIDTH, INT_ACK_ECC_OFFSET, 0xFFFF);

	do {
		val = read_mc_field(INT_STATUS_ECC_ADDR, INT_STATUS_ECC_WIDTH, INT_STATUS_ECC_OFFSET);
	} while (val != 0);

	/* Step 6 */
	rmw_mc_set_field(INT_MASK_BIST_ADDR, INT_MASK_BIST_WIDTH, INT_MASK_BIST_OFFSET, 0x0);


	/* Step 7 */
	rmw_mc_set_field(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_WIDTH, LP_AUTO_ENTRY_EN_OFFSET, bak_lp_auto_entry_en);
	rmw_mc_set_field(IN_ORDER_ACCEPT_ADDR, IN_ORDER_ACCEPT_WIDTH, IN_ORDER_ACCEPT_OFFSET, bak_in_order_accept);
	INFO("BL2: ECC prog all0 done\n");
}

static void init_ecc(void)
{
	uint64_t addr_start, addr_end;
	uint32_t cs_val_upper, val;
	INFO("BL2: ECC init start\n");
	/* Step 1 */
	val = read_mc_reg(USER_DEF_REG_10_ADDR);
	val = (val >> 16) & 0xF;
	if (val == 2)
		addr_start = DDR_BASE_ADDRESS + 0x40;
	else
		addr_start = DDR_BASE_ADDRESS + 0x20;

	/* Step 2 */
	cs_val_upper = read_mc_reg(CS_VAL_UPPER_0_ADDR);
	cs_val_upper = (cs_val_upper >> 16) & 0xffffU;
	val = read_mc_reg(CS_MAP_ADDR);
	val = (val >> 16) & 0x3;
	if (val == 0x3) {
		val = read_mc_reg(CS_VAL_UPPER_1_ADDR);
		val = (val >> 16) & 0xffffU;
		if (val > cs_val_upper) {
			cs_val_upper = val;
		}
	}
	addr_end = (uint64_t)(((cs_val_upper + 1) << 18) - 1) + DDR_BASE_ADDRESS;

	/* Step 3 */
	rmw_mc_set_field(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_WIDTH, ECC_DISABLE_W_UC_ERR_OFFSET, 0x1);

	rmw_mc_set_field(INT_MASK_ECC_ADDR, INT_MASK_ECC_WIDTH, INT_MASK_ECC_OFFSET, 0x01CF);

	/* Step 4 */
	mdelay(10);

	/* Step 5 */
	ecc_prog_all0(addr_start, addr_end);

	/* Step 6 */
	rmw_mc_set_field(INT_MASK_ECC_ADDR, INT_MASK_ECC_WIDTH, INT_MASK_ECC_OFFSET, 0x0000);

	rmw_mc_set_field(INT_ACK_ECC_ADDR, INT_ACK_ECC_WIDTH, INT_ACK_ECC_OFFSET, 0x01CF);

	rmw_mc_set_field(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_WIDTH, ECC_DISABLE_W_UC_ERR_OFFSET, 0x0000);

	mdelay(10);
	INFO("BL2: ECC init end\n");
}

static void program_mc1_ecc_en(void)
{
	uint32_t tmp = 0;
	uint16_t addr_diff[2], cs_size[2], cs_val_lower[2], row_start_val[2];
	int i, maxrow_cs;

	tmp = read_mc_reg(ECC_ENABLE_ADDR);
	tmp &= ~(0x3 << 24);
#if (DDR_ECC_DETECT_CORRECT == 1)
	tmp |= (0x3 << ECC_ENABLE_OFFSET);
#elif (DDR_ECC_DETECT == 1)
	tmp |= (0x2 << ECC_ENABLE_OFFSET);
#else
	tmp |= (1 << ECC_ENABLE_OFFSET);
#endif

	write_mc_reg(ECC_ENABLE_ADDR, tmp);
	rmw_mc_set_field(ADDR_COLLISION_MPM_DIS_ADDR, ADDR_COLLISION_MPM_DIS_WIDTH, ADDR_COLLISION_MPM_DIS_OFFSET, 1);
	rmw_mc_set_field(DISABLE_RD_INTERLEAVE_ADDR, DISABLE_RD_INTERLEAVE_WIDTH, DISABLE_RD_INTERLEAVE_OFFSET, 0);

	rmw_mc_set_field(IN_ORDER_ACCEPT_ADDR, IN_ORDER_ACCEPT_WIDTH, IN_ORDER_ACCEPT_OFFSET, 1);
	rmw_mc_set_field(SWAP_EN_ADDR, SWAP_EN_WIDTH, SWAP_EN_OFFSET, 0);


	addr_diff[0] = read_mc_field(BANK_DIFF_0_ADDR, BANK_DIFF_0_WIDTH, BANK_DIFF_0_OFFSET);
	addr_diff[0] += read_mc_field(COL_DIFF_0_ADDR, COL_DIFF_0_WIDTH, COL_DIFF_0_OFFSET);
	addr_diff[0] += read_mc_field(ROW_DIFF_0_ADDR, ROW_DIFF_0_WIDTH, ROW_DIFF_0_OFFSET);

	addr_diff[1] = read_mc_field(BANK_DIFF_1_ADDR, BANK_DIFF_1_WIDTH, BANK_DIFF_1_OFFSET);
	addr_diff[1] += read_mc_field(COL_DIFF_1_ADDR, COL_DIFF_1_WIDTH, COL_DIFF_1_OFFSET);
	addr_diff[1] += read_mc_field(ROW_DIFF_1_ADDR, ROW_DIFF_1_WIDTH, ROW_DIFF_1_OFFSET);

	for (i = 0; i < 2; i++) {
		cs_size[i] = (0xDFFF >> 1) >> addr_diff[i];
	}

	if (cs_size[0] >= cs_size[1]) {
		maxrow_cs = 0;
	} else {
		maxrow_cs = 1;
	}

	cs_val_lower[maxrow_cs] = 0x0000;
	cs_val_lower[(maxrow_cs + 1) % 2] = cs_val_lower[maxrow_cs] + cs_size[maxrow_cs] + 1;

	rmw_mc_set_field(CS_VAL_LOWER_0_ADDR, CS_VAL_LOWER_0_WIDTH, CS_VAL_LOWER_0_OFFSET, cs_val_lower[0]);
	rmw_mc_set_field(CS_VAL_UPPER_0_ADDR, CS_VAL_UPPER_0_WIDTH, CS_VAL_UPPER_0_OFFSET, (cs_val_lower[0] + cs_size[0]));

	tmp = read_mc_field(CS_MAP_ADDR, CS_MAP_WIDTH, CS_MAP_OFFSET);
	if (tmp == 0x3) {
		rmw_mc_set_field(CS_VAL_LOWER_1_ADDR, CS_VAL_LOWER_1_WIDTH, CS_VAL_LOWER_1_OFFSET, cs_val_lower[1]);
		rmw_mc_set_field(CS_VAL_UPPER_1_ADDR, CS_VAL_UPPER_1_WIDTH, CS_VAL_UPPER_1_OFFSET, (cs_val_lower[1] + cs_size[1]));
	}

	row_start_val[maxrow_cs] = 0x0;
	i = (maxrow_cs + 1) % 2;
	if (cs_size[i] == cs_size[maxrow_cs]) {
		row_start_val[i] = 0x1;
	} else if (cs_size[i] == (cs_size[maxrow_cs] >> 1)) {
		row_start_val[i] = 0x2;
	} else if (cs_size[i] == (cs_size[maxrow_cs] >> 2)) {
		row_start_val[i] = 0x4;
	} else {
		row_start_val[i] = 0x0;
	}

	rmw_mc_set_field(ROW_START_VAL_0_ADDR, ROW_START_VAL_0_WIDTH, ROW_START_VAL_0_OFFSET, row_start_val[0]);
	rmw_mc_set_field(ROW_START_VAL_1_ADDR, ROW_START_VAL_1_WIDTH, ROW_START_VAL_1_OFFSET, row_start_val[1]);
}
#endif /* (DDR_ECC_ENABLE == 1) */

static void disable_phy_clk(void)
{
	/* Step 9 */
	write_phy_reg(DDRPHY_R77, 0x00000200);
	write_phy_reg(DDRPHY_R78, 0x00010001);
}

static void program_mc1(uint8_t *lp_auto_entry_en)
{
	int i;

	/* Step 1 */
	for (i = 0; i < ARRAY_SIZE(mc_init_tbl); i++) {
		if (mc_init_tbl[i][0] == DDRMC_R006) {
			*lp_auto_entry_en = mc_init_tbl[i][1] & 0xF;
			write_mc_reg(DDRMC_R006, mc_init_tbl[i][1] & 0xFFFFFFF0);
		} else {
			write_mc_reg(mc_init_tbl[i][0], mc_init_tbl[i][1]);
		}
	}

	/* Step 2 */
	rmw_mc_set_field(ODT_WR_MAP_CS0_ADDR, ODT_WR_MAP_CS0_WIDTH, ODT_WR_MAP_CS0_OFFSET, mc_odt_pins_tbl[0] << 24);
	rmw_mc_set_field(ODT_WR_MAP_CS1_ADDR, ODT_WR_MAP_CS1_WIDTH, ODT_WR_MAP_CS1_OFFSET, mc_odt_pins_tbl[1] << 8);
	rmw_mc_set_field(ODT_RD_MAP_CS0_ADDR, ODT_RD_MAP_CS0_WIDTH, ODT_RD_MAP_CS0_OFFSET, mc_odt_pins_tbl[2] << 16);
	rmw_mc_set_field(ODT_RD_MAP_CS1_ADDR, ODT_RD_MAP_CS1_WIDTH, ODT_RD_MAP_CS1_OFFSET, mc_odt_pins_tbl[3] << 0);

	/* Step 3 */
	rmw_mc_reg(MR1_DATA_0_ADDR, ~(mc_mr1_tbl[0]), mc_mr1_tbl[1]);
	rmw_mc_reg(MR1_DATA_1_ADDR, ~(mc_mr1_tbl[0]), mc_mr1_tbl[1]);

	/* Step 4 */
	rmw_mc_reg(MR2_DATA_0_ADDR, ~(mc_mr2_tbl[0]), mc_mr2_tbl[1]);
	rmw_mc_reg(MR2_DATA_1_ADDR, ~(mc_mr2_tbl[0]), mc_mr2_tbl[1]);

	/* Step 5 */
	rmw_mc_reg(MR5_DATA_0_ADDR, ~(mc_mr5_tbl[0]), mc_mr5_tbl[1]);
	rmw_mc_reg(MR5_DATA_1_ADDR, ~(mc_mr5_tbl[0]), mc_mr5_tbl[1]);

	/* Step 6 */
	rmw_mc_reg(MR6_DATA_0_ADDR, ~(mc_mr6_tbl[0]), mc_mr6_tbl[1]);
	rmw_mc_reg(MR6_DATA_1_ADDR, ~(mc_mr6_tbl[0]), mc_mr6_tbl[1]);

	/* Step 7 */
	for (i = 0; i < ARRAY_SIZE(mc_phy_settings_tbl); i++) {
		write_mc_reg(mc_phy_settings_tbl[i][0], mc_phy_settings_tbl[i][1]);
	}

	/* Step 8 */
#if (DDR_ECC_ENABLE == 1)
	program_mc1_ecc_en();
#endif
}

static void lpddr4_combo_io_cal(void)
{
	/* Step 37.1 */
	uint32_t tmp;
	uint8_t dram = (read_mc_reg(USER_DEF_REG_10_ADDR) >> 16) & 0xF;

	/* Step 37.2 */
	rmw_phy_reg(UNIQUIFY_IO_2, 0xFF800000, 0x00000000);

	/* Step 37.3 */
	switch (dram) {
	case 0:
		tmp = 0x00003200;
		break;
	case 1:
		tmp = 0x00005200;
		break;
	case 2:
		tmp = 0x08009200;
		break;
	default:
		tmp = 0x00000000;
		break;
	}
	write_phy_reg(UNIQUIFY_IO_3, tmp);
	write_phy_reg(UNIQUIFY_IO_1, 0x00000002);

	while ((read_phy_reg(UNIQUIFY_IO_1) & 0x00000008) != 0x00000008)
		;

	write_phy_reg(UNIQUIFY_IO_1, 0x00000000);
	udelay(100);

	/* Step 37.4 */
	switch (dram) {
	case 0:
	case 1:
		tmp = 0x00041200;
		break;
	case 2:
		tmp = 0x08101300;
		break;
	default:
		tmp = 0x00000000;
		break;
	}
	write_phy_reg(UNIQUIFY_IO_3, tmp);
	write_phy_reg(UNIQUIFY_IO_1, 0x00000001);
	while ((read_phy_reg(UNIQUIFY_IO_1) & 0x00000004) != 0x00000004)
		;

	rmw_phy_reg(UNIQUIFY_IO_1, 0xFFFFFFEF, 0x00000010);
	rmw_phy_reg(UNIQUIFY_IO_1, 0xFFFFFFEF, 0x00000000);
	udelay(1);
}

static void program_phy1(uint32_t sl_lanes, uint32_t byte_lanes)
{
	uint32_t i;
	uint16_t dram_clk_period;
	int8_t extra_addrctrl_dly;
	int8_t ofs_dlls_trim_1, ofs_dlls_trim_3;
	uint8_t dram;
	uint8_t odt_wr_map_cs0, odt_rd_map_cs0;
	uint8_t CL, CWL, AL, PL, RL, WL;
	uint32_t mr1, mr1_wl, mr1_wl_mask;
	uint32_t mr2, mr2_wl, mr2_wl_mask;
	uint8_t clk_drive, dq_dqs_drive, dq_dqs_term, vref_value, vref_ca_value, adrctrl_drive;
	uint8_t read_lat, trim_lat;

	/*
	 * Unused
	 * int8_t dll_mas_dly
	 * uint8_t core_clk_margin_win_a, core_clk_margin_win_b;
	 * uint8_t incr_winA_val, incr_winB_val;
	 * uint8_t soc_dq_drive, soc_dq_odt;
	 * uint8_t dram_dq_drive, dram_dq_odt_wr1, dram_dq_odt_rd2, dram_dq_odt_wr2;
	 */

	uint8_t swap_phase;
	uint32_t tmp;

	/* Step 1
	 *
	 * Calculated by calling function:
	 * mem_dp_reduction
	 * sl_lanes
	 * byte_lanes
	 */

	tmp = read_mc_reg(USER_DEF_REG_10_ADDR);
	dram_clk_period = (tmp >> 0) & 0xFFFF;
	dram = (tmp >> 16) & 0xF;

	odt_wr_map_cs0 = read_mc_field(ODT_WR_MAP_CS0_ADDR, ODT_WR_MAP_CS0_WIDTH, ODT_WR_MAP_CS0_OFFSET);
	odt_rd_map_cs0 = read_mc_field(ODT_RD_MAP_CS0_ADDR, ODT_RD_MAP_CS0_WIDTH, ODT_RD_MAP_CS0_OFFSET);

	/* Step 2 */
	CL = read_mc_field(CASLAT_LIN_ADDR, 0x5, CASLAT_LIN_OFFSET + 1);
	CWL = read_mc_field(WRLAT_ADDR, WRLAT_WIDTH, WRLAT_OFFSET);

	AL =  read_mc_field(ADDITIVE_LAT_ADDR, ADDITIVE_LAT_WIDTH, ADDITIVE_LAT_OFFSET);
	PL =  read_mc_field(CA_PARITY_LAT_ADDR, CA_PARITY_LAT_WIDTH, CA_PARITY_LAT_OFFSET);

	RL = CL + AL + PL;
	WL = CWL + AL + PL;

	/* Step 3 */
	mr1 = read_mc_reg(MR1_DATA_0_ADDR) & 0xFFFF;
	mr2 = read_mc_reg(MR2_DATA_0_ADDR) & 0xFFFF;

	if (dram == 2) {
		/* DDR4 */
		mr1_wl_mask = (0x1 << 7) | (0x7 << 8);
		tmp = ((mr2 >> 9) & 0x7);
		switch (tmp) {
		case 0:
			mr1_wl = mr1 | (1<<7);
			break;
		case 1:
			mr1_wl = (mr1 & (0xFFFF ^ mr1_wl_mask)) | (1 << 7) | (0x2 << 8);
			break;
		case 2:
			mr1_wl = (mr1 & (0xFFFF ^ mr1_wl_mask)) | (1 << 7) | (0x4 << 8);
			break;
		case 4:
			mr1_wl = (mr1 & (0xFFFF ^ mr1_wl_mask)) | (1 << 7) | (0x6 << 8);
			break;
		default:
			panic();
		}

		mr2_wl_mask = 0x7 << 9;
		mr2_wl = (mr2 & (0xFFFF ^ mr2_wl_mask)) | (0x0 << 9);
	} else {
		/* DDR3L, DDR3 */
		mr1_wl_mask = (0x1 << 9) | (0x1 << 7) | (0x1 << 6) | (0x1 << 2);
		tmp = ((mr2>>9) & 0x3);
		switch (tmp) {
		case 0:
			mr1_wl = mr1 | (1 << 7);
			break;
		case 1:
			mr1_wl = (mr1 & (0xFFFF ^ mr1_wl_mask)) | (1 << 7) | (0 << 9) | (0 << 6) | (1 << 2);
			break;
		case 2:
			mr1_wl = (mr1 & (0xFFFF ^ mr1_wl_mask)) | (1 << 7) | (0 << 9) | (1 << 6) | (0 << 2);
			break;
		default:
			panic();
		}

		mr2_wl_mask = 0x3 << 9;
		mr2_wl = (mr2 & (0xFFFF ^ mr2_wl_mask)) | (0x0 << 9);
	}

	/* Step 4 */
	tmp = read_mc_reg(USER_DEF_REG_11_ADDR);
	clk_drive = tmp & 0xF;
	dq_dqs_drive = (tmp >> 4) & 0xF;
	dq_dqs_term = (tmp >> 8) & 0xF;
	adrctrl_drive = (tmp >> 12) & 0xF;
	vref_value = (tmp >> 16) & 0xFF;
	vref_ca_value = (tmp >> 24) & 0xFF;

	/*
	 * Unused
	 * tmp = read_mc_reg(USER_DEF_REG_12_ADDR);
	 * soc_dq_drive = (tmp >> 0) & 0xFF;
	 * soc_dq_odt = (tmp >> 8) & 0xFF;
	 *
	 * tmp = read_mc_reg(USER_DEF_REG_12_ADDR);
	 *
	 * dram_dq_drive = (tmp >> 0) & 0xFF;
	 * dram_dq_odt_wr1 = (tmp >> 8) & 0xFF;
	 * dram_dq_odt_wr2 = (tmp >> 16) & 0xFF;
	 * dram_dq_odt_rd2 = (tmp >> 24) & 0xFF;
	 */

	swap_phase = 1;

	read_lat = (dram == 2 ? 12 : 10) + (CEIL(RL, 2) * 2) - CEIL(WL, 2) + 28;
	trim_lat = (dram == 2 ? 11 : 9) + CEIL(RL, 2) - CEIL(WL, 2) + 29;

	/*
	 * Unused
	 * core_clk_margin_win_a = 15;
	 * core_clk_margin_win_b = 17;
	 * incr_winA_val = 9;
	 * incr_winB_val = 26;
	 */
	extra_addrctrl_dly = 26;
	ofs_dlls_trim_1 = -9;
	ofs_dlls_trim_3 = -10;

	/* Step 5 */
	tmp = ((WL == 5 ? 0x1 : 0x0) << 16) | 0x00100000;
	write_phy_reg(PHY_MODE0, tmp);

	/* Step 6 */
	write_phy_reg(DLLM_WINDOW_SIZE, 0x00000006);

	/* Step 7 */
	if (dram == 2) {
		/* DDR4 */
		write_phy_reg(DDR4_CONFIG_1, 0x00000009);
	}

	/* Step 8 */
	write_phy_reg(UNIQUIFY_IO_3, (dram == 2 ? 1 : 0) << 27);
	write_phy_reg(UNIQUIFY_IO_2, (dram == 2 ? 1 : 0) << 24);

	/* Step 9 */
	tmp = ((dram == 0 ? 0 : 1) << 15) | ((dram_clk_period < 1000 ? 1 : 0) << 8) | 0x10004000;
	write_phy_reg(PHY_PAD_CTRL, tmp);

	/* Step 10 */
	write_phy_reg(PHY_PAD_CTRL_1,
		clk_drive | (clk_drive << 4) | (adrctrl_drive << 8) | (adrctrl_drive<<12) |
		(dq_dqs_drive << 16) | (dq_dqs_drive << 20));

	/* Step 11 */
	tmp = dq_dqs_term | ((dram == 2) ? 0 : (dq_dqs_term << 4));
	write_phy_reg(PHY_PAD_CTRL_2, tmp);

	/* Step 12 */
	write_phy_reg(PHY_PAD_CTRL_3, 0x00000000);

	/* Step 13 */
	for (i = 0; i < byte_lanes; i++) {
		write_phy_reg(PHY_LANE_SEL, 7 * i);
		write_phy_reg(VREF_TRAINING, (vref_value << 4) | 0x00000004);
	}

	/* Step 14 */
	write_phy_reg(VREF_CA_TRAINING, vref_ca_value);

	/* Step 15 */
	write_phy_reg(SCL_WINDOW_TRIM, 0x1A09002D);

	/* Step 16 */
	write_phy_reg(UNQ_ANALOG_DLL_1, 0x00000000);

	/* Step 17 */
	write_phy_reg(DYNAMIC_IE_TIMER, 0x0000001A);

	/* Step 18 */
	tmp = ((dram == 2 ? 0 : 1) << 2) | (CEIL(CL, 2) << 4) | (CEIL(AL, 2) << 8) |
			(odt_rd_map_cs0 << 16) | (odt_wr_map_cs0 << 24) | 0x00000001;
	write_phy_reg(SCL_CONFIG_1, tmp);

	/* Step 19 */
	tmp = (CEIL(CWL, 2) << 8) | ((((WL % 2) == 0) ? 0 : 1) << 24) | 0x80000001;
	write_phy_reg(SCL_CONFIG_2, tmp);

	/* Step 20 */
	write_phy_reg(SCL_CONFIG_3, sl_lanes ^ 0x3);

	/* Step 21 */
	write_phy_reg(DYNAMIC_WRITE_BIT_LVL, (trim_lat << 4) | (read_lat << 12));

	/* Step 22 */
	tmp = ((WL % 2) == 0) & swap_phase;
	write_phy_reg(SCL_CONFIG_4, tmp);

	/* Step 23 */
	write_phy_reg(SCL_GATE_TIMING, 0x00000170);

	/* Step 24 */
	write_phy_reg(WRLVL_DYN_ODT, mr2_wl | (mr2 << 16));

	/* Step 25 */
	write_phy_reg(WRLVL_ON_OFF, mr1 | (mr1_wl << 16));

	/* Step 26 */
	write_phy_reg(PHY_DLL_RECALIB, 0xAC001000);

	/* Step 27 */
	udelay(10);

	/*
	 * Step 28 is skipped because dll_mas_dly is unused.
	 * dll_mas_dly = (*PHY_DLL_ADRCTRL>>24) & 0xFF;
	 */

	/* Step 29 */
	tmp = ((ofs_dlls_trim_3 < 0) ? 0 : sl_lanes);
	write_phy_reg(PHY_DLL_INCR_TRIM_3, tmp);

	/* Step 30 */
	tmp = ((ofs_dlls_trim_1 < 0) ? 0 : sl_lanes);
	write_phy_reg(PHY_DLL_INCR_TRIM_1, tmp);

	/* Step 31 */
	for (i = 0; i < byte_lanes; i++) {
		write_phy_reg(PHY_LANE_SEL, 6 * i);
		tmp = ((ofs_dlls_trim_1 < 0) ? -ofs_dlls_trim_1 : ofs_dlls_trim_1);
		write_phy_reg(PHY_DLL_TRIM_1, tmp);
		tmp = ((ofs_dlls_trim_3 < 0) ? -ofs_dlls_trim_3 : ofs_dlls_trim_3);
		write_phy_reg(PHY_DLL_TRIM_3, tmp);
	}

	/* Step 32 */
	write_phy_reg(PHY_DLL_ADRCTRL, ((extra_addrctrl_dly & 0x7F) << 0) | 0x00000200);

	/* Step 33 */
	write_phy_reg(PHY_LANE_SEL, 0);
	write_phy_reg(PHY_DLL_TRIM_CLK, (extra_addrctrl_dly << 0) | 0x00000080);

	/* Step 34 */
	write_phy_reg(PHY_DLL_RECALIB, ((extra_addrctrl_dly & 0x7F) << 0) | (0x10 << 8) | 0xAC000000);

	/* Step 35 */
	write_phy_reg(SCL_LATENCY, 0x00035076);

	/* Step 36 */
	write_phy_reg(BIT_LVL_CONFIG, 0x00000032);

	/* Step 37 */
	lpddr4_combo_io_cal();

	/* Step 38 */
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x00000000);

	/* Step 39 */
	rmw_phy_reg(PHY_CTRL0, 0xFFFFF0FE, (sl_lanes << 8));
}

static void exec_trainingWRLVL(uint32_t sl_lanes)
{
	uint32_t tmp;

	/* Step 2 */
	tmp = read_phy_reg(DDRPHY_R24);
	write_phy_reg(DDRPHY_R24, tmp | 0x01000000);

	/* Step 3 */
	write_phy_reg(DDRPHY_R37, sl_lanes);

	/* Step 4 */
	write_phy_reg(DDRPHY_R31, 0x00010000);

	/* Step 5 */
	write_phy_reg(DDRPHY_R18, 0x50200000);

	/* Step 6 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 7 - Step 8 */
	if (((read_phy_reg(DDRPHY_R36) & sl_lanes) != sl_lanes) ||
	   ((read_phy_reg(DDRPHY_R37) & sl_lanes) != 0)) {
		panic();
	}

	/* Step 9 */
	write_phy_reg(DDRPHY_R24, tmp);
}

static void exec_trainingVREF(uint32_t sl_lanes, uint32_t byte_lanes)
{
	uint32_t vref_mid_level_code;
	uint32_t vref_training_value;
	uint32_t sweep_range;
	uint16_t current_vref = 0;
	uint32_t best_window_diff_so_far[MAX_BYTE_LANES];
	uint32_t num_best_vref_matches[MAX_BYTE_LANES];
	uint32_t all_best_vref_matches[MAX_BYTE_LANES][MAX_BEST_VREF_SAVED];
	uint8_t window_0, window_1, window_diff;
	uint32_t highest_best_vref_val, lowest_best_vref_val;
	uint8_t orig_cs_config;
	uint32_t tmp;
	int i, j;

	/* Step 1 - skipped
	 * sl_lanes and byte_lanes are calculated in calling function
	 */

	/* Step 2 */
	for (i = 0; i < byte_lanes; i++) {
		write_phy_reg(DDRPHY_R29, i);
		rmw_phy_reg(DDRPHY_R07, 0xFFFFFFCF, 0x00000010);
	}

	/* Step 3 */
	vref_mid_level_code = (read_mc_reg(USER_DEF_REG_11_ADDR) >> 16) & 0xFF;
	sweep_range = read_mc_reg(USER_DEF_REG_14_ADDR) & 0xFF;

	/* Step 4 */
	for (i = 0; i < byte_lanes; i++) {
		best_window_diff_so_far[i] = 255;
		num_best_vref_matches[i] = 0;
	}

	/* Step 5 */
	for (vref_training_value = 0;
		 vref_training_value < (sweep_range * 2) + 1;
		 vref_training_value += VREF_SETP) {
		/* Step 5.1 */
		if (vref_training_value < sweep_range + 1) {
			if (vref_mid_level_code < vref_training_value + 2) {
				vref_training_value = sweep_range;
				continue;
			} else {
				current_vref = vref_mid_level_code - vref_training_value;
			}
		} else {
			if ((vref_mid_level_code + vref_training_value - sweep_range) > 126) {
				break;
			} else {
				current_vref = vref_mid_level_code + vref_training_value - sweep_range;
			}
		}
		for (i = 0; i < byte_lanes; i++) {
			write_phy_reg(DDRPHY_R29, 7 * i);
			write_phy_reg(DDRPHY_R66, (current_vref << 4) | 0x00000001);
		}

		/* Step 5.2 */
		write_phy_reg(DDRPHY_R18, 0x30800000);
		while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
			;

		/* Step 5.3 */
		for (i = 0; i < byte_lanes; i++) {
			if (((read_phy_reg(DDRPHY_R59) >> (14 + i)) & 0x1) == 0x0) {
				VERBOSE("BL2: PHY side VREF training passed on lane %0d, current_vref = %0d\n", i, current_vref);
				write_phy_reg(DDRPHY_R29, i * 6);
				window_0 = read_phy_reg(DDRPHY_R69) & 0x3F;
				window_1 = (read_phy_reg(DDRPHY_R69) >> 8) & 0x3F;
				window_diff = (window_0 > window_1) ?
								window_0 - window_1 : window_1 - window_0;
				VERBOSE("BL2: window_0 = %0d, window_1 = %0d, window_diff = %0d\n", window_0, window_1, window_diff);
				if (window_diff < best_window_diff_so_far[i]) {
					best_window_diff_so_far[i] = window_diff;
					all_best_vref_matches[i][0] = current_vref;
					num_best_vref_matches[i] = 1;
					VERBOSE("BL2: CURRENT BEST VREF PHY side :%d\n", current_vref);
				} else if ((window_diff == best_window_diff_so_far[i]) &&
						(num_best_vref_matches[i] < MAX_BEST_VREF_SAVED)) {
					all_best_vref_matches[i][num_best_vref_matches[i]] = current_vref;
					num_best_vref_matches[i] += 1;
				}
			} else {
				ERROR("BL2: PHY side VREF training failed lane %d, current_vref = %d\n",
					i, current_vref);
			}
		}
		/* Step 5.4 */
	}

	/* Step 6 */
	for (i = 0; i < byte_lanes; i++) {
		highest_best_vref_val = 0x0;
		lowest_best_vref_val = 0x7F;
		for (j = 0; j < num_best_vref_matches[i]; j++) {
			highest_best_vref_val =
				_MAX(all_best_vref_matches[i][j], highest_best_vref_val);
			lowest_best_vref_val  =
				_MIN(all_best_vref_matches[i][j], lowest_best_vref_val);
		}
		current_vref = (highest_best_vref_val + lowest_best_vref_val) >> 1;
		write_phy_reg(DDRPHY_R29, 7 * i);
		write_phy_reg(DDRPHY_R66, current_vref << 4);
	}

	/* Step 7 */
	write_phy_reg(DDRPHY_R19, 0xFF00FF00);
	write_phy_reg(DDRPHY_R20, 0xFF00FF00);

	/* Step 8 */
	write_phy_reg(DDRPHY_R18, 0x30800000);
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 9 */
	tmp = (read_phy_reg(DDRPHY_R59) >> 14) & sl_lanes;
	if ((tmp ^ sl_lanes) != sl_lanes) {
		panic();
	}

	/* Step 10 */
	rmw_phy_reg(DDRPHY_R54, 0xFFFFFF7F, 0x00000080);

	/* Step 11 */
	vref_mid_level_code = (read_mc_reg(DDRMC_R043) >> 8) & 0xFF;
	sweep_range = (read_mc_reg(DDRMC_R043) >> 16) & 0xFF;

	/* Step 12 */
	orig_cs_config = read_phy_reg(DDRPHY_R25) & 0x3;

	/* Step 13 */
	setup_vref_training_registers(vref_mid_level_code, sl_lanes, 1);

	/* Step 14 */
	rmw_phy_reg(DDRPHY_R66, 0xFFFFFFFE, 0x00000001);

	/* Step 15 */
	for (i = 0; i < byte_lanes; i++) {
		best_window_diff_so_far[i] = 255;
		num_best_vref_matches[i] = 0;
	}

	/* Step 16 */
	for (vref_training_value = 0;
		 vref_training_value < (sweep_range * 2) + 1;
		 vref_training_value += VREF_SETP) {
		/* Step 16.1 */
		if (vref_training_value < (sweep_range + 1)) {
			if (vref_training_value > vref_mid_level_code) {
				vref_training_value = sweep_range;
				continue;
			} else {
				current_vref = vref_mid_level_code - vref_training_value;
			}
		} else {
			if ((vref_mid_level_code + vref_training_value - sweep_range) <= 73) {
				current_vref =
					vref_mid_level_code + vref_training_value - sweep_range;
			} else {
				break;
			}
		}
		setup_vref_training_registers(current_vref, orig_cs_config, 0);

		/* Step 16.2 */
		write_phy_reg(DDRPHY_R18, 0x30500000);
		while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
			;

		/* Step 16.3 */
		tmp = (read_phy_reg(DDRPHY_R64) >> 20) & sl_lanes;
		for (i = 0; i < byte_lanes; i++) {
			if ((tmp ^ sl_lanes) == sl_lanes) {
				VERBOSE("BL2: VREF training passed during VrefDQ training DRAM side, current_vref = %d\n", current_vref);
				write_phy_reg(DDRPHY_R29, i * 6);
				window_0 = read_phy_reg(DDRPHY_R69) & 0x3F;
				window_1 = (read_phy_reg(DDRPHY_R69) >> 8) & 0x3F;
				window_diff = (window_0 > window_1) ?
								window_0 - window_1 : window_1 - window_0;
				VERBOSE("BL2: window_0 = %0d, window_1 = %0d, window_diff = %0d\n", window_0, window_1, window_diff);
				if (window_diff < best_window_diff_so_far[i]) {
					best_window_diff_so_far[i] = window_diff;
					all_best_vref_matches[i][0] = current_vref;
					num_best_vref_matches[i] = 1;
					VERBOSE("BL2: CURRENT BEST VREF DRAM side :%d\n", current_vref);
				} else if ((window_diff == best_window_diff_so_far[i]) &&
						(num_best_vref_matches[i] < MAX_BEST_VREF_SAVED)) {
					all_best_vref_matches[i][num_best_vref_matches[i]] = current_vref;
					num_best_vref_matches[i] += 1;
				}
			} else {
				ERROR("BL2: VREF training failed during VrefDQ training DRAM side, current_vref = %d\n", current_vref);
			}
		}
		/* Step 16.4 */
	}

	/* Step 17 */
	highest_best_vref_val = 0x0;
	lowest_best_vref_val = 0x7F;
	for (i = 0; i < byte_lanes; i++) {
		for (j = 0; j < num_best_vref_matches[i]; j++) {
			highest_best_vref_val =
				_MAX(all_best_vref_matches[i][j], highest_best_vref_val);
			lowest_best_vref_val  =
				_MIN(all_best_vref_matches[i][j], lowest_best_vref_val);
		}
	}
	current_vref = (highest_best_vref_val + lowest_best_vref_val) >> 1;

	/* Step 18 */
	setup_vref_training_registers(current_vref, sl_lanes, 0);

	/* Step 19 */
	rmw_mc_reg(DDRMC_R044, 0xFFFFFF00, current_vref);

	/* Step 20 */
	rmw_phy_reg(DDRPHY_R66, 0xFFFFFFFE, 0x00000000);

	/* Step 21 */
	setup_vref_training_registers(current_vref, sl_lanes, 2);

	/* Step 22 */
	rmw_phy_reg(DDRPHY_R54, 0xFFFFFF7F, 0x00000000);

	/* Step 23 */
	for (i = 0; i < byte_lanes; i++) {
		write_phy_reg(DDRPHY_R29, i);
		rmw_phy_reg(DDRPHY_R07, 0xFFFFFFCF, 0x00000030);
	}
}

static void setup_vref_training_registers(uint8_t vref_value, uint8_t cs, uint8_t turn_on_off_vref_training)
{
	uint8_t vref_op_code;
	uint16_t mr;

	/* Step 1 */
	if (vref_value > 50) {
		vref_op_code = vref_value - 23;
	} else {
		vref_op_code = vref_value | (1 << 6);
	}

	/* Step 2 */
	mr = read_mc_reg(MR6_DATA_0_ADDR) & 0xFF00;
	write_mr(cs, 6,
		mr | (((turn_on_off_vref_training == 2) ? 0 : 1) << 7) | vref_op_code);

	/* Step 3 */
	udelay(1);
}

static void write_mr(uint8_t cs, uint8_t mrw_sel, uint16_t mrw_data)
{
	uint8_t mrw_cs;
	uint8_t mrw_allcs;

	/* Step 1 */
	mrw_cs = 0;
	if (cs & 0x1) {
		rmw_mc_set_field(MRSINGLE_DATA_0_ADDR, 16, MRSINGLE_DATA_0_OFFSET, mrw_data);
		mrw_cs = 0;
	}
	if (cs & 0x2) {
		rmw_mc_set_field(MRSINGLE_DATA_1_ADDR, 16, MRSINGLE_DATA_1_OFFSET, mrw_data);
		mrw_cs = 1;
	}
	mrw_allcs = ((cs & 0x3) == 0x3) ? 1 : 0;

	/* Step 2 */
	rmw_mc_set_field(WRITE_MODEREG_ADDR, WRITE_MODEREG_WIDTH - 1, WRITE_MODEREG_OFFSET,
		0x02800000 | (mrw_allcs << 24) | (mrw_cs << 8) | mrw_sel);

	/* Step 3 */
	while ((read_mc_reg(INT_STATUS_MODE_ADDR) & (1 << 3)) != (1 << 3))
		;

	/* Step 4 */
	rmw_mc_set_field(INT_ACK_MODE_ADDR, 0x1, INT_ACK_MODE_OFFSET + 3, 0x01);
}

static void exec_trainingBITLVL(uint32_t sl_lanes)
{
	uint32_t tmp;

	/* Step 2 */
	write_phy_reg(DDRPHY_R62, 0x00000000);

	/* Step 3 */
	write_phy_reg(DDRPHY_R19, 0xFF00FF00);
	write_phy_reg(DDRPHY_R20, 0xFF00FF00);

	/* Step 4 */
	write_phy_reg(DDRPHY_R18, 0x30A00000);

	/* Step 5 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 6 */
	tmp = (read_phy_reg(DDRPHY_R59) >> 14) & sl_lanes;
	if ((tmp ^ sl_lanes) != sl_lanes) {
		panic();
	}

	/* Step 7 */
	rmw_phy_reg(DDRPHY_R54, 0xFFFFFF7F, 0x00000080);

	/* Step 8 */
	write_phy_reg(DDRPHY_R18, 0x30700000);

	/* Step 9 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 10 */
	tmp = (read_phy_reg(DDRPHY_R64) >> 20) & sl_lanes;
	if ((tmp ^ sl_lanes) != sl_lanes) {
		panic();
	}

	/* Step 11 */
	rmw_phy_reg(DDRPHY_R54, 0xFFFFFF7F, 0x00000000);

	/* Step 12 */
	write_phy_reg(DDRPHY_R51, 0x00080000);

	/* Step 13 */
	write_phy_reg(DDRPHY_R18, 0x11200000);

	/* Step 14 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 15 */
	write_phy_reg(DDRPHY_R51, 0x00000000);

	/* Step 16 */
	write_phy_reg(DDRPHY_R18, 0x30A00000);

	/* Step 17 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;
}

static void opt_delay(uint32_t sl_lanes, uint32_t byte_lanes)
{
	uint16_t dlls_trim_ca;
	uint16_t dlls_trim_2[MAX_BYTE_LANES];
	uint16_t op_dqs_trim[MAX_BYTE_LANES];
	uint16_t min_WL = 128;
	uint16_t min_WD = 128;
	uint32_t val;
	int lane, bit_sel;

	/*
	 * Step 1 - Skipped
	 * sl_lanes and byte_lanes are calculated by calling function.
	 */

	/* Step 2 */
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x04000000);

	/* Step 3 */
	rmw_mc_set_field(LP_CMD_ADDR, LP_CMD_WIDTH, LP_CMD_OFFSET, 0x11);

	while (read_mc_field(LP_STATE_ADDR, LP_STATE_WIDTH, LP_STATE_OFFSET) != 0x48)
		;

	/* Step 4 */
	write_phy_reg(PHY_LANE_SEL, 0 * 8);
	dlls_trim_ca = (read_phy_reg(PHY_DLL_TRIM_CLK) >> 0) & 0x7F;
	min_WL = dlls_trim_ca;
	for (lane = 0; lane < byte_lanes; lane++) {
		write_phy_reg(PHY_LANE_SEL, 6 * lane);
		dlls_trim_2[lane] = (read_phy_reg(PHY_DLL_TRIM_2) >> 0) & 0x3F;
		if (dlls_trim_2[lane] < min_WL) {
			min_WL = dlls_trim_2[lane];
		}

		write_phy_reg(PHY_LANE_SEL, (lane * 7) | 0x00000900);
		op_dqs_trim[lane] = (read_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM) >> 0) & 0x3F;
		if (op_dqs_trim[lane] < min_WD) {
			min_WD = op_dqs_trim[lane];
		}

		for (bit_sel = 0; bit_sel < 9; bit_sel++) {
			write_phy_reg(PHY_LANE_SEL, (lane * 7) | (bit_sel << 8));
			val = (read_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM) >> 0) & 0x7F;
			val = (val & 0x40) ? (op_dqs_trim[lane] + (val & 0x3F)) : (op_dqs_trim[lane] - (val & 0x3F));
			if (val < min_WD) {
				min_WD = val;
			}
		}
	}

	/* Step 5 */
	val = (dlls_trim_ca - min_WL) & 0x7F;
	write_phy_reg(PHY_LANE_SEL, 0 * 8);
	write_phy_reg(PHY_DLL_TRIM_CLK, (val << 0) | 0x00000080);
	write_phy_reg(PHY_DLL_ADRCTRL, (val << 0) | 0x00000200);
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFFFFFF80, (val << 0));

	for (lane = 0; lane < byte_lanes; lane++) {
		val = (dlls_trim_2[lane] - min_WL) & 0x3F;
		write_phy_reg(PHY_LANE_SEL, 6 * lane);
		rmw_phy_reg(PHY_DLL_TRIM_2, 0xFFFFFFC0, (val << 0));
		write_phy_reg(PHY_LANE_SEL, (lane * 7) | 0x00000900);
		val = (op_dqs_trim[lane] - min_WD) & 0x3F;
		rmw_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM, 0xFFFFFF80, (val << 0));
	}

	/* Step 6 */
	rmw_mc_set_field(LPC_SR_ZQ_EN_ADDR, LPC_SR_ZQ_EN_WIDTH, LPC_SR_ZQ_EN_OFFSET, 0x00000000);
	rmw_mc_set_field(LP_CMD_ADDR, LP_CMD_WIDTH, LP_CMD_OFFSET, 0x00000002);

	while (read_mc_field(LP_STATE_ADDR, LP_STATE_WIDTH, LP_STATE_OFFSET) != 0x40)
		;

	udelay(1);
	rmw_mc_set_field(LPC_SR_ZQ_EN_ADDR, LPC_SR_ZQ_EN_WIDTH, LPC_SR_ZQ_EN_OFFSET, 0x00000001);

	/* Step 7 */
	rmw_mc_set_field(INT_ACK_LOWPOWER_ADDR, INT_ACK_LOWPOWER_WIDTH, INT_ACK_LOWPOWER_OFFSET, 0x0009);

	while (read_mc_field(INT_STATUS_LOWPOWER_ADDR, INT_STATUS_LOWPOWER_WIDTH, INT_STATUS_LOWPOWER_OFFSET) != 0x0000)
		;

	/* Step 8 */
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x00000000);
	while ((read_phy_reg(UNQ_ANALOG_DLL_2) & 0x3) != sl_lanes)
		;
}

static void exec_trainingSL(uint32_t sl_lanes)
{
	/* Step 2 */
	write_phy_reg(DDRPHY_R62, 0x00000001);

	/* Step 3 */
	write_phy_reg(DDRPHY_R34, 0x00000010);

	/* Step 4 */
	write_phy_reg(DDRPHY_R19, 0x789B3DE0);
	write_phy_reg(DDRPHY_R20, 0xF10E4A56);

	/* Step 5 */
	write_phy_reg(DDRPHY_R18, 0x11200000);

	/* Step 6 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 7 */
	write_phy_reg(DDRPHY_R18, 0x11200000);

	/* Step 8 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 9 */
	write_phy_reg(DDRPHY_R18, 0x34200000);

	/* Step 10 */
	while ((read_phy_reg(DDRPHY_R18) & 0x10000000) != 0x00000000)
		;

	/* Step 11 */
	if ((read_phy_reg(DDRPHY_R18) & sl_lanes) != sl_lanes) {
		panic();
	}

	/* Step 12 */
	write_phy_reg(DDRPHY_R62, 0x00000003);
}

static void program_phy2(void)
{
	uint16_t dram_clk_period;
	uint32_t tmp, b21, b22, b23;

	/* Step 1 */
	tmp = read_mc_reg(USER_DEF_REG_10_ADDR);
	dram_clk_period = tmp & 0xFFFF;
	b21 = (tmp >> 21) & 0x1;
	b22 = (tmp >> 22) & 0x1;
	b23 = (tmp >> 23) & 0x1;

	/* Step 2 */
	rmw_phy_reg(DDRPHY_R64, 0xFFFFFFFE, b23);
	rmw_phy_reg(DDRPHY_R59, 0xFFFFFFFE, (b23 == 1 ? 0 : b22));
	write_phy_reg(DDRPHY_R55, (b21 << 24) |
		_MIN(1000000000000 / (2 * dram_clk_period * 256), 0xFFFFFF));

	/* Step 3 */
	rmw_phy_reg(DDRPHY_R27, 0xFBFFFFFF, 0x04000000);
	rmw_phy_reg(DDRPHY_R27, 0xFC0000FF,
		_MIN(1000000000000 / (dram_clk_period * 256), 0x3FFFF) << 8);
	rmw_phy_reg(DDRPHY_R27, 0xFBFFFFFF, 0x00000000);
}

static void program_mc2(void)
{
	uint8_t main_clk_dly;
	uint8_t tphy_rdlat;
	uint32_t tmp;

	/* Step 1 */
	main_clk_dly = (read_phy_reg(DDRPHY_R21) >> 4) & 0xF;
	tmp = read_mc_field(TDFI_RDDATA_EN_ADDR, TDFI_RDDATA_EN_WIDTH, TDFI_RDDATA_EN_OFFSET);
	tphy_rdlat = ((main_clk_dly + 1 + 1) * 2) + 2 + ((tmp == 1) ? 2 : 0);

	/* Step 2 */
	rmw_mc_set_field(TDFI_PHY_RDLAT_ADDR, TDFI_PHY_RDLAT_WIDTH, TDFI_PHY_RDLAT_OFFSET, tphy_rdlat);
}
