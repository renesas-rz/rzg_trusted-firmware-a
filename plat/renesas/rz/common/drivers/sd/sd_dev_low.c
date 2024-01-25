/*
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/******************************************************************************
* System Name  : SDHI Driver
* File Name    : sd_dev_low.c
* Version      : 1.30
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <drivers/delay_timer.h>
#include "sdmmc_iodefine.h"
#include "r_sdif.h"
#include "r_sd_cfg.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: sddev_init
* Description  : Initialize H/W to use SDHI
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_init(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_init
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_power_on
* Description  : Power-on H/W to use SDHI
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_power_on(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_power_on
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_power_off
* Description  : Power-off H/W to use SDHI
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
*              : fail    : SD_ERR
******************************************************************************/
int32_t sddev_power_off(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_power_off
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_read_data
* Description  : read from SDHI buffer FIFO
* Arguments    : int32_t sd_port   : channel no (0 or 1)
*              : uint8_t *buff     : buffer addrees to store reading datas
*              : uint32_t reg_addr : SDIP FIFO address
*              : int32_t num       : counts to read(unit:byte)
* Return Value : success : SD_OK
*              : fail    : SD_ERR
******************************************************************************/
int32_t sddev_read_data(int32_t sd_port, uint8_t *buff, uint32_t reg_addr, int32_t num)
{
	int32_t  i;
	int32_t  cnt;
	uint64_t *p_reg;
	uint64_t *p_l;
	uint8_t  *p_c;
	volatile uint64_t tmp;

	/* Cast to an appropriate type */
	p_reg = (uint64_t *)((uintptr_t)reg_addr);

	cnt = (num / 8);

	/* Cast to an appropriate type */
	if (0uL != ((uintptr_t)buff & 0x7uL)) {
		/* Cast to an appropriate type */
		p_c = (uint8_t *)buff;
		for (i = cnt; i > 0 ; i--) {
			tmp = *p_reg;

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 8);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 16);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 24);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 32);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 40);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 48);

			/* Cast to an appropriate type */
			*p_c++ = (uint8_t)(tmp >> 56);
		}

		cnt = (num % 8);
		if (0 != cnt) {
			tmp = *p_reg;
			for (i = cnt; i > 0 ; i--) {
				/* Cast to an appropriate type */
				*p_c++ = (uint8_t)(tmp);
				tmp >>= 8;
			}
		}
	} else {
		/* Cast to an appropriate type */
		p_l = (uint64_t *)buff;
		for (i = cnt; i > 0 ; i--) {
			*p_l++ = *p_reg;
		}

		cnt = (num % 8);
		if (0 != cnt) {
			/* Cast to an appropriate type */
			p_c = (uint8_t *)p_l;
			tmp = *p_reg;
			for (i = cnt; i > 0 ; i--) {
				/* Cast to an appropriate type */
				*p_c++ = (uint8_t)(tmp);
				tmp >>= 8;
			}
		}
	}

	return SD_OK;
}
/*******************************************************************************
 End of function sddev_read_data
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_write_data
* Description  : write to SDHI buffer FIFO
* Arguments    : int32_t sd_port   : channel no (0 or 1)
*              : uint8_t *buff     : buffer addrees to store writting datas
*              : uint32_t reg_addr : SDIP FIFO address
*              : int32_t num       : counts to write(unit:byte)
* Return Value : success : SD_OK
*              : fail    : SD_ERR
******************************************************************************/
int32_t sddev_write_data(int32_t sd_port, uint8_t *buff, uint32_t reg_addr, int32_t num)
{
	int32_t  i;

	/* Cast to an appropriate type */
	uint64_t *p_reg = (uint64_t *)((uintptr_t)reg_addr);

	/* Cast to an appropriate type */
	uint64_t *p_buff = (uint64_t *)buff;
	uint64_t tmp;

	/* dont care non 8byte alignment data */
	num += 7;
	num /= 8;

	/* Cast to an appropriate type */
	if (((uintptr_t)buff & 0x7uL) != 0uL) {
		for (i = num; i > 0 ; i--) {
			/* Cast to an appropriate type */
			tmp  = (uint64_t)(*buff++);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 8);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 16);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 24);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 32);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 40);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 48);

			/* Cast to an appropriate type */
			tmp |= ((uint64_t)(*buff++) << 56);
			*p_reg = tmp;
		}
	} else {
		for (i = num; i > 0 ; i--) {
			*p_reg = *p_buff++;
		}
	}

	return SD_OK;
}
/*******************************************************************************
 End of function sddev_write_data
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_get_clockdiv
* Description  : Get clock div value.
* Arguments    : int32_t sd_port : channel no (0 or 1)
*              : int32_t clock   : request clock frequency
*              :   SD_CLK_50MHZ
*              :   SD_CLK_25MHZ
*              :   SD_CLK_20MHZ
*              :   SD_CLK_10MHZ
*              :   SD_CLK_5MHZ
*              :   SD_CLK_1MHZ
*              :   SD_CLK_400KHZ
* Return Value : clock div value
*              :   SD_DIV_4   : 1/4   clock
*              :   SD_DIV_8   : 1/8   clock
*              :   SD_DIV_16  : 1/16  clock
*              :   SD_DIV_32  : 1/32  clock
*              :   SD_DIV_256 : 1/256 clock
*              :   SD_DIV_512 : 1/512 clock
******************************************************************************/
uint32_t sddev_get_clockdiv(int32_t sd_port, int32_t clock)
{
	uint32_t div;

	switch (clock) {
	case SD_CLK_50MHZ:
		div = SD_DIV_4;        /* 133.25MHz/4 = 33.31MHz   */
		break;
	case SD_CLK_25MHZ:
	case SD_CLK_20MHZ:
		div = SD_DIV_8;        /* 133.25MHz/8 = 16.65MHz   */
		break;
	case SD_CLK_10MHZ:
		div = SD_DIV_16;       /* 133.25MHz/16 = 8.32MHz   */
		break;
	case SD_CLK_5MHZ:
		div = SD_DIV_32;       /* 133.25MHz/32 = 4.16MHz   */
		break;
	case SD_CLK_1MHZ:
		div = SD_DIV_256;      /* 133.25MHz/256 = 520.5kHz */
		break;
	case SD_CLK_400KHZ:
		div = SD_DIV_512;      /* 133.25MHz/512 = 260.2kHz */
		break;
	default:
		div = SD_DIV_512;      /* 133.25MHz/512 = 260.2kHz */
		break;
	}

	return div;
}
/*******************************************************************************
 End of function sddev_get_clockdiv
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_set_port
* Description  : setting ports to use SDHI
* Arguments    : int32_t sd_port : channel no (0 or 1)
*              : int32_t mode    : SD_PORT_PARALLEL : 4bit mode
*                                : SD_PORT_SERIAL   : 1bit mode
* Return Value : success : SD_OK
*              : fail    : SD_ERR
******************************************************************************/
int32_t sddev_set_port(int32_t sd_port, int32_t mode)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_set_port
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_int_wait
* Description  : Waitting for SDHI Interrupt
* Arguments    : int32_t sd_port : channel no (0 or 1)
*              : int32_t time    : time out value to wait interrupt
* Return Value : get interrupt : SD_OK
*              : time out      : SD_ERR
******************************************************************************/
int32_t sddev_int_wait(int32_t sd_port, int32_t time)
{
	int32_t  ret;
	uint32_t waittime;

	waittime = (uint32_t)time;

	/* interrupt generated? */
	ret = sd_check_int(sd_port);
	while ((SD_ERR == ret) && (waittime > 0uL)) {
		mdelay(1);
		waittime--;

		/* interrupt generated? */
		ret = sd_check_int(sd_port);
	}

	return ret;
}
/*******************************************************************************
 End of function sddev_int_wait
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_init_dma
* Description  : Initialize DMAC to transfer data from SDHI FIFO
* Arguments    : int32_t sd_port : channel no (0 or 1)
*              : uint32_t buff   : buffer addrees to transfer datas
*              : int32_t dir     : direction to transfer
*              :                 :   0 : FIFO -> buffer
*              :                 :   1 : buffer -> FIFO
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_init_dma(int32_t sd_port, uint32_t buff, int32_t dir)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_init_dma
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_wait_dma_end
* Description  : Wait to complete DMAC transfer
* Arguments    : int32_t sd_port : channel no (0 or 1)
*              : int32_t cnt     : counts to transfer(unit:byte)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_wait_dma_end(int32_t sd_port, int32_t cnt)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_wait_dma_end
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_disable_dma
* Description  : Disable DMAC transfer
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_disable_dma(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_disable_dma
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_reset_dma
* Description  : Reset of the SDHI module built-in DMAC.
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_reset_dma(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_reset_dma
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_finalize_dma
* Description  : Finalize of the SDHI module built-in DMAC.
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_finalize_dma(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_finalize_dma
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_loc_cpu
* Description  : lock cpu to disable interrupt
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_loc_cpu(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_loc_cpu
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_unl_cpu
* Description  : unlock cpu to enable interrupt
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_unl_cpu(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_unl_cpu
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_finalize
* Description  : finalize SDHI
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : success : SD_OK
******************************************************************************/
int32_t sddev_finalize(int32_t sd_port)
{
	return SD_OK;
}
/*******************************************************************************
 End of function sddev_finalize
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_cd_layout
* Description  : CD Terminal Support Confirmation
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : Not Support : SD_ERR
******************************************************************************/
int32_t sddev_cd_layout(int32_t sd_port)
{
	return SD_ERR;
}
/*******************************************************************************
 End of function sddev_cd_layout
 ******************************************************************************/

/******************************************************************************
* Function Name: sddev_wp_layout
* Description  : WP Terminal Support Confirmation
* Arguments    : int32_t sd_port : channel no (0 or 1)
* Return Value : Not Support : SD_ERR
******************************************************************************/
int32_t sddev_wp_layout(int32_t sd_port)
{
	return SD_ERR;
}
/*******************************************************************************
 End of function sddev_wp_layout
 ******************************************************************************/

/* End of File */
