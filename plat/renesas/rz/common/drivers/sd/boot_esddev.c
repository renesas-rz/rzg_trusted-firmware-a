/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : boot_esddev.c
 * Version      : 1.0
 * Description  : Target CPU Interface for eSD driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include "esdif.h"            /* eSD driver library header */
#include <drivers/delay_timer.h>
#
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_init
 * Description  : initialze eSD H/W
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_init ( void )
{
    int32_t iret = SD_ERR;

    iret = SD_OK;   /* It's all normal, so it's done correctly. */

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_power_on
 * Description  : turn eSD H/W on
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_power_on ( void )
{
    int32_t iret = SD_ERR;

    do
    {
        /* It's all normal, so it's done correctly. */
        iret = SD_OK;
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_power_on
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_power_off
 * Description  : turn eSD H/W off
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_power_off ( void )
{
    int32_t iret = SD_ERR;

    do
    {
        iret = SD_OK;    /* It's all normal, so it's done correctly. */
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_power_off
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_write_data
 * Description  : write data to eSD
 *                for more information, refer to eSD card driver manual..
 * Arguments    : uint8_t *buff          : pointer to write buffer
 *                uintptr_t reg_addr     : data register address of host controller I/P
 *                uint32_t num           : write byte count
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_write_data ( uint8_t *buff , uintptr_t reg_addr , uint32_t num )
{
    int32_t iret = SD_ERR;

    do
    {
        iret = SD_OK;   /* It's all normal, so it's done correctly. */
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_write_data
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_get_clockdiv
 * Description  : get eSD clock
 *                for more information, refer to eSD card driver manual..
 * Arguments    : int32_t clock : clock frequency;
 *                                 SD_CLK_400kHz ... SD_CLK_50MHz
 * Return Value : SD_DIV_2    :complete setting to devide by 2
 *                SD_DIV_4    :complete setting to devide by 4
 *                SD_DIV_8    :complete setting to devide by 8
 *                SD_DIV_16   :complete setting to devide by 16
 *                SD_DIV_32   :complete setting to devide by 32
 *                SD_DIV_64   :complete setting to devide by 64
 *                SD_DIV_128  :complete setting to devide by 128
 *                SD_DIV_256  :complete setting to devide by 256
 *********************************************************************************************************************/
uint32_t esddev_get_clockdiv ( int32_t clock )
{
    uint32_t uiret_clkdiv;

    do
    {
        switch(clock)
        {

            case SD_CLK_50MHz:
            case SD_CLK_25MHz:
            case SD_CLK_20MHz:
                uiret_clkdiv = SD_DIV_8;    /* 16.66MHz */
                break;
            case SD_CLK_10MHz:
                uiret_clkdiv = SD_DIV_16;   /* 8.33MHz */
                break;
            case SD_CLK_5MHz:
                uiret_clkdiv = SD_DIV_32;   /* 4.165MHz */
                break;
            case SD_CLK_1MHz:
                uiret_clkdiv = SD_DIV_256;  /* 520KHz */
                break;
            case SD_CLK_400kHz:
                uiret_clkdiv = SD_DIV_512;  /* 260KHz */
                break;
            default:
                uiret_clkdiv = SD_DIV_512;
                break;

        }
    } while(0);

    return(uiret_clkdiv);
}
/**********************************************************************************************************************
 * End of function esddev_get_clockdiv
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: esddev_set_port
 * Description  : select eSD port
 *                for more information, refer to eSD card driver manual..
 * Arguments    : int32_t mode : selected mode
 *                               0x0000u : SD_PORT_SERIAL
 *                               0x0001u : SD_PORT_PARALLEL
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 * Note         : Not implement
 *              : Check whether selected port or mode at CALLER
 *********************************************************************************************************************/
int32_t esddev_set_port(int32_t mode)
{
    int32_t iret = SD_ERR;

    do
    {
        iret = SD_OK;    /* It's all normal, so it's done correctly. */
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_set_port
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_int_wait
 * Description  : wait eSD interrupt
 *                for more information, refer to eSD card driver manual..
 * Arguments    : int32_t time : time-out value[msec]
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_int_wait ( int32_t msec )
{
    int32_t iret = SD_ERR;
    int32_t isubret = SD_ERR;
    int32_t cnt = 0;
    do 
    {
        while( cnt < msec )
        {
            isubret = esd_check_int();
            if( SD_OK == isubret )
            {
                /* Loop break with SD host controller interrupt detection */
                break;
            }
            udelay(1000); //busy wait with 1msec.
            cnt++;
        }
        if( SD_OK != isubret)
        {
            /* Error break without SD host controller interrupt detection */
            break;
        }
        /* It's all normal, so it's done correctly. */
        iret = SD_OK;
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_int_wait
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_loc_cpu
 * Description  : lock of eSD interrupt
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_loc_cpu ( void )
{
    int32_t iret = SD_ERR;

    do
    {
        /* Lock of SD host controller interrupt */
        /* It's all normal, so it's done correctly. */
        iret = SD_OK;
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_loc_cpu
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_unl_cpu
 * Description  : unlock of eSD interrupt
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_unl_cpu ( void )
{
    int32_t iret = SD_ERR;

    do
    {
        /* It's all normal, so it's done correctly. */
        iret = SD_OK;
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_unl_cpu
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_init_dma
 * Description  : initialize eSD DMA
 *                for more information, refer to eSD card driver manual..
 * Arguments    : uintptr_t buff_addr : start address of buffer
 *                uintptr_t reg_addr  : start address of register
 *                uint32_t   cnt      : count of transfer
 *                int32_t   dir       : direction of transfer
 *                                        0 : register to buffer
 *                                        1 : buffer to register
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_init_dma(uintptr_t buffadr, uintptr_t regadr, uint32_t cnt, int32_t dir)
{
    return(SD_ERR);
}
/**********************************************************************************************************************
 * End of function esddev_init_dma
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_wait_dma_end
 * Description  : wait for completion of eSD DMA transfer
 *                for more information, refer to eSD card driver manual..
 * Arguments    : int32_t cnt : byte count of DMA transfer
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_wait_dma_end ( uint32_t cnt )
{
    return(SD_ERR);
}
/**********************************************************************************************************************
 * End of function esddev_wait_dma_end
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_disable_dma
 * Description  : disable eSD DMA
 *                for more information, refer to eSD card driver manual..
 * Arguments    : none.
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_disable_dma ( void )
{
    return(SD_ERR);
}
/**********************************************************************************************************************
 * End of function esddev_disable_dma
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esddev_read_data
 * Description  : read data from eSD
 *                for more information, refer to eSD card driver manual..
 * Arguments    : uint8_t *buff      : pointer to read buffer
 *                uintptr_t reg_addr : data register address of host controller I/P
 *                int32_t num        : read byte count
 * Return Value : SD_OK    : 0
 *              : SD_ERR   : -1.
 *********************************************************************************************************************/
int32_t esddev_read_data ( uint8_t *buff, uintptr_t reg_addr, uint32_t num )
{

    int32_t iret = SD_ERR;
    uint32_t i;
    uint32_t *p_reg = (uint32_t *)(reg_addr);
    uint32_t *p_ptr = (uint32_t *)buff;
    uint32_t tmp;

    do 
    {
        num+=3;                                             /* Size rounding adjustment for accessing */
        num /=4;
    
        if((uintptr_t)buff & 0x3)
        {                           /* Operation with 4bytes access */
            for(i=num; i > 0 ; i--)
            {
                tmp = *p_reg;                                 /* Read 4bytes data from FIFO */

                *buff++ = (uint8_t)(tmp);
                *buff++ = (uint8_t)(tmp >> 8);
                *buff++ = (uint8_t)(tmp >> 16);
                *buff++ = (uint8_t)(tmp >> 24);
            }
        }
        else
        {
            for(i=num; i > 0 ; i--)
            {                           /* Operation with 4byte access */
                *p_ptr++ = *p_reg;          /* Read and store data in buffer, every 2 bytes */
            }
        }
        iret = SD_OK;                   /* It's all normal, so it's done correctly. */
    } while(0);

    return(iret);
}
/**********************************************************************************************************************
 * End of function esddev_read_data
 *********************************************************************************************************************/

/*---------------------------------------------------------------------------*/
/*      End Of File                                                          */
/*---------------------------------------------------------------------------*/
