/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <lib/mmio.h>
#include <rz_soc_def.h>
#include <riic.h>
#include <riic_regs.h>

#define RIIC_SPEED_RATE			(100000)
#define RIIC_WAIT_COUNT_MAX		(40000000U)

static void riic_clear_bit(uint8_t val, uintptr_t offset)
{
	mmio_write_8(offset, mmio_read_8(offset) & ~val);
}

static void riic_set_bit(uint8_t val, uint32_t offset)
{
	mmio_write_8(offset, mmio_read_8(offset) | val);
}

static int riic_set_clock(int clock)
{
	switch (clock) {
	case 100000:
		riic_clear_bit(ICFER_FMPE, RIIC_ICFER);
		riic_set_bit(ICFER_SCLE | ICFER_NFE, RIIC_ICFER);
		riic_clear_bit(ICMR3_NF_MASK, RIIC_ICMR3);
		riic_set_bit(ICMR3_NF_FOUR, RIIC_ICMR3);
		riic_clear_bit(ICMR1_CKS_MASK, RIIC_ICMR1);
		riic_set_bit(ICMR1_CKS(4), RIIC_ICMR1);
		mmio_write_8(RIIC_ICBRH, ICBRH_RESERVED | 24);
		mmio_write_8(RIIC_ICBRL, ICBRL_RESERVED | 27);
		break;
	case 400000:
		riic_clear_bit(ICFER_FMPE, RIIC_ICFER);
		riic_set_bit(ICFER_SCLE | ICFER_NFE, RIIC_ICFER);
		riic_clear_bit(ICMR3_NF_MASK, RIIC_ICMR3);
		riic_set_bit(ICMR3_NF_FOUR, RIIC_ICMR3);
		riic_clear_bit(ICMR1_CKS_MASK, RIIC_ICMR1);
		riic_set_bit(ICMR1_CKS(2), RIIC_ICMR1);
		mmio_write_8(RIIC_ICBRH, ICBRH_RESERVED | 25);
		mmio_write_8(RIIC_ICBRL, ICBRL_RESERVED | 26);
		break;
	case 1000000:
		riic_set_bit(ICFER_FMPE, RIIC_ICFER);
		riic_set_bit(ICFER_SCLE | ICFER_NFE, RIIC_ICFER);
		riic_clear_bit(ICMR3_NF_MASK, RIIC_ICMR3);
		riic_set_bit(ICMR3_NF_FOUR, RIIC_ICMR3);
		riic_clear_bit(ICMR1_CKS_MASK, RIIC_ICMR1);
		riic_set_bit(ICMR1_CKS(1), RIIC_ICMR1);
		mmio_write_8(RIIC_ICBRH, ICBRH_RESERVED | 14);
		mmio_write_8(RIIC_ICBRL, ICBRL_RESERVED | 24);
		break;

	default:
		ERROR("%s: unsupported clock (%dHz)\n", __func__, clock);
		return -1;
	}

	return 0;
}

static int riic_init_setting(int clock)
{
	int ret;

	riic_clear_bit(ICCR1_ICE, RIIC_ICCR1);

	riic_set_bit(ICCR1_IICRST, RIIC_ICCR1);

	riic_clear_bit(ICCR1_IICRST, RIIC_ICCR1);

	ret = (int)mmio_read_8(RIIC_ICCR1);

	mmio_write_8(RIIC_ICSER, ICSER_SAR0E);

	mmio_write_8(RIIC_ICMR1, ICMR1_BC(7));

	ret = mmio_read_8(RIIC_ICMR1);

	ret = riic_set_clock(clock);
	if (ret < 0)
		return ret;

	riic_set_bit(ICCR1_ICE, RIIC_ICCR1);	/* Enable RIIC */

	riic_set_bit(ICMR3_RDRFS | ICMR3_WAIT | ICMR3_ACKWP, RIIC_ICMR3);

	ret = mmio_read_8(RIIC_ICMR3);

	return 0;
}

static int riic_check_busy(void)
{
	uint8_t icsr2;
	uint32_t loop_cnt = 0;

	do {
		icsr2 = mmio_read_8(RIIC_ICCR2);
		if (0 == (icsr2 & ICCR2_BBSY))
			return 0;
	} while (loop_cnt++ < RIIC_WAIT_COUNT_MAX);

	ERROR("%s: i2c bus is busy.\n", __func__);
	return -1;
}

static int riic_wait_for_icsr2(uint8_t bit)
{
	uint8_t icsr2;
	uint32_t loop_cnt = 0;

	do {
		icsr2 = mmio_read_8(RIIC_ICSR2);

		if (0 != (icsr2 & ICSR2_NACKF))
			return -1;
		if (0 != (icsr2 & bit))
			return 0;
	} while (loop_cnt++ < RIIC_WAIT_COUNT_MAX);

	ERROR("%s: timeout!(bit = %x icsr2 = %x, iccr2 = %x)\n", __func__,
		bit, mmio_read_8(RIIC_ICSR2), mmio_read_8(RIIC_ICCR2));

	return -1;
}

static int riic_check_nack_receive(void)
{
	if (0 != (mmio_read_8(RIIC_ICSR2) & ICSR2_NACKF)) {
		/* received NACK */
		riic_clear_bit(ICSR2_NACKF, RIIC_ICSR2);
		riic_set_bit(ICCR2_SP, RIIC_ICCR2);
		mmio_read_8(RIIC_ICDRR);	/* dummy read */
		return -1;
	}
	return 0;
}

static void riic_set_receive_ack(int ack)
{

	if (ack)
		riic_clear_bit(ICMR3_ACKBT, RIIC_ICMR3);
	else
		riic_set_bit(ICMR3_ACKBT, RIIC_ICMR3);
}

static int riic_i2c_raw_write(uint8_t *buf, int len)
{
	int ret = 0;
	int index = 0;

	for (index = 0; index < len; index++) {
		ret = riic_check_nack_receive();
		if (ret < 0)
			return ret;

		ret = riic_wait_for_icsr2(ICSR2_TDRE);
		if (ret < 0)
			return ret;

		mmio_write_8(RIIC_ICDRT, buf[index]);
	}

	return ret;
}

static int riic_i2c_raw_read(uint8_t *buf, uint32_t len)
{
	int dummy_read = 1;
	int ret = 0;
	int index = 0;

	do {
		ret = riic_wait_for_icsr2(ICSR2_RDRF);
		if (ret < 0)
			return ret;

		buf[index] = mmio_read_8(RIIC_ICDRR);
		if (dummy_read)
			dummy_read = 0;
		else
			index++;
		riic_set_receive_ack(1);
	} while (index < (len - 1));

	ret = riic_wait_for_icsr2(ICSR2_RDRF);
	if (ret < 0)
		return ret;

	riic_clear_bit(ICSR2_STOP, RIIC_ICSR2);

	riic_set_bit(ICCR2_SP, RIIC_ICCR2);

	buf[index++] = mmio_read_8(RIIC_ICDRR);

	riic_set_receive_ack(0);

	return ret;
}

static int riic_send_mem_addr(uint32_t addr, int alen)
{
	int i;
	uint8_t buf[4];

	if (alen > 4 || alen <= 0)
		return -1;

	for (i = alen - 1; i >= 0; i--, addr >>= 8)
		buf[i] = addr & 0xff;

	return riic_i2c_raw_write(buf, alen);
}

static int riic_send_start_cond(int restart)
{
	int ret;

	if (restart)
		mmio_write_8(RIIC_ICCR2, mmio_read_8(RIIC_ICCR2) | ICCR2_RS);
	else
		mmio_write_8(RIIC_ICCR2, mmio_read_8(RIIC_ICCR2) | ICCR2_ST);

	ret = riic_wait_for_icsr2(ICSR2_START);
	if (ret < 0)
		return ret;

	riic_clear_bit(ICSR2_START, RIIC_ICSR2);

	return ret;
}

static int riic_send_stop_cond(void)
{
	int ret;

	riic_clear_bit(ICSR2_STOP | ICSR2_NACKF, RIIC_ICSR2);
	riic_set_bit(ICCR2_SP, RIIC_ICCR2);

	ret = riic_wait_for_icsr2(ICSR2_STOP);
	if (ret < 0)
		return ret;

	riic_clear_bit(ICSR2_STOP | ICSR2_NACKF, RIIC_ICSR2);
	return ret;
}

static int riic_send_dev_addr(uint8_t addr, int read)
{
	uint8_t buf = ((addr << 1) | read);

	return riic_i2c_raw_write(&buf, 1);
}

void riic_setup(void)
{
	int ret;

	ret = riic_init_setting(RIIC_SPEED_RATE);
	if (ret)
		panic();
}

int32_t riic_write(uint8_t slave, uint8_t addr, uint8_t data)
{
	int ret;

	ret = riic_check_busy();
	if (ret < 0)
		return ret;

	ret = riic_send_start_cond(0);
	if (ret < 0)
		goto force_exit;

	/* send addr */
	ret = riic_send_dev_addr(slave, 0);
	if (ret < 0)
		goto force_exit;

	ret = riic_send_mem_addr(addr, 1);
	if (ret < 0)
		goto force_exit;

	/* transmit data */
	ret = riic_i2c_raw_write(&data, 1);
	if (ret < 0)
		goto force_exit;

	ret = riic_wait_for_icsr2(ICSR2_TEND);

force_exit:
	riic_send_stop_cond();

	return ret;
}

int32_t riic_read(uint8_t slave, uint8_t addr, uint8_t *data)
{
	int ret;

	ret = riic_check_busy();
	if (ret < 0)
		return ret;

	ret = riic_send_start_cond(0);
	if (ret < 0)
		goto force_exit;

	/* send addr */
	ret = riic_send_dev_addr(slave, 0);
	if (ret < 0)
		goto force_exit;

	ret = riic_send_mem_addr(addr, 1);
	if (ret < 0)
		goto force_exit;

	ret = riic_wait_for_icsr2(ICSR2_TEND);
	if (ret < 0)
		goto force_exit;

	/* restart */
	ret = riic_send_start_cond(1);
	if (ret < 0)
		goto force_exit;

	ret = riic_send_dev_addr(slave, 1);
	if (ret < 0)
		goto force_exit;

	ret = riic_wait_for_icsr2(ICSR2_RDRF);
	if (ret < 0)
		goto force_exit;

	ret = riic_check_nack_receive();
	if (ret < 0)
		goto force_exit;

	/* receive data */
	ret = riic_i2c_raw_read(data, 1);

force_exit:
	riic_send_stop_cond();

	return ret;
}
