/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_main.c
 * Version      : 1.0
 * Description  : SD Driver main.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 07.22.2022 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <assert.h>
#include <esdif.h>
#include <boot_esd_if.h>
#include <ut_define.h>
#include <sd.h>
#include <boot_esd_if.h>
#include <boot_common.h>
#include <cpg_iodefine.h>

uint64_t gl_uwWorkBuffer[ ESD_SIZE_OF_INIT / sizeof(uint64_t) ];         /* WorkSpace for eSD driver Library */

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
static void esd_init_param( stESD_BOOT_OUTPUT* pstOutput );

/**********************************************************************************************************************
 * Function Name: esd_main
 * Description  : Initializes and mounts the working memory of the SD driver.
 * Arguments    : void
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_main(void)
{
	int32_t subret;
	uint16_t ubcardtype;
	uint8_t  ubcardspeed;
	uint8_t  ubcardcapacity;
	stESD_BOOT_OUTPUT stesdboot_output;

	subret=0;
	esd_init_param( (stESD_BOOT_OUTPUT* )&stesdboot_output );

	subret=0;
	subret = esd_init(_SDHI0_BASE_,gl_uwWorkBuffer,SD_CD_SOCKET);
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_init.\n");
		return SD_ERR;
    }

	subret=0;
	subret = esd_set_buffer((void *)RAM_LSBLOCK_ADDR,   /* Specify format buffer that has 512byte */
									SD_SECTOR_SIZE);    /* format buffer size is specify */
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_set_buffer.\n");
		return SD_ERR;
    }

	subret=0;
	subret = esd_check_media();
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_check_media.\n");
		return SD_ERR;
	}

	subret=0;
	subret = esd_mount((SD_MODE_POLL |  SD_MODE_SW  |  SD_MODE_DS | SD_MODE_VER2X), SD_VOLT_3_3);
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_mount.\n");
		return SD_ERR;
	}

	subret=0;
	ubcardtype=0;
	ubcardspeed=0;
	ubcardcapacity=0;
	subret = esd_get_type((uint16_t *)&ubcardtype,
							(uint8_t *) &ubcardspeed,
							(uint8_t *) &ubcardcapacity);
	if( SD_OK != subret ) {
		NOTICE("BL2: Failed to esd_get_type.\n");
		return SD_ERR;
	}
	else if( ( ubcardtype & SD_MEDIA_SD ) == SD_MEDIA_SD ) {
            /* none */
    }
    else if( ( ubcardtype & SD_MEDIA_EMBEDDED ) == SD_MEDIA_EMBEDDED ) {
            /* No operation */
    }
    else {
        /* Error break with invalid card type */
		NOTICE("BL2: Failed to esd_get_type.\n");
		return SD_ERR;
    }

	subret=0;
    subret = esd_get_partition_id(
		 (int32_t *)&stesdboot_output.ipartition_number );
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_get_partition_id.\n");
		return SD_ERR;
	}

	subret=0;
    subret = esd_read_sect(( uint8_t *)0x44000000, 1, 1);
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_read_sect.\n");
		return SD_ERR;
	}

    return SD_OK;
}

static void esd_init_param( stESD_BOOT_OUTPUT* pstOutput )
{
    pstOutput->ipartition_number      = -1;
    pstOutput->ierror_code            = -1;
    pstOutput->iloadersize_blkno      = -1;
    pstOutput->iloadersize_blknum     = -1;
    pstOutput->iloaderprog_blkno      = -1;
    pstOutput->iloaderprog_blknum     = -1;
}

/* End of File */
