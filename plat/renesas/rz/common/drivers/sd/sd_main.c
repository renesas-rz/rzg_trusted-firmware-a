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
#include <sd.h>

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
static uint32_t sd_drv_rw_buffer[SD_SECTOR_SIZE / sizeof(uint32_t)];
static uint64_t sd_drv_work_area[ESD_SIZE_OF_INIT / sizeof(uint64_t)];         /* WorkSpace for eSD driver Library */

/**********************************************************************************************************************
 * Function Name: esd_main
 * Description  : Initializes and mounts the working memory of the SD driver.
 * Arguments    : void
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
int32_t esd_main(void)
{
	int32_t subret;
	uint16_t ubcardtype;
	uint8_t  ubcardspeed;
	uint8_t  ubcardcapacity;
	int32_t  ipartition_number;

	subret=0;
	subret = esd_init(_SDHI0_BASE_, sd_drv_work_area, SD_CD_SOCKET);
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_init.\n");
		return SD_ERR;
    }

	subret=0;
	subret = esd_set_buffer((void *)sd_drv_rw_buffer,     /* Specify format buffer that has 512byte */
							sizeof(sd_drv_rw_buffer));    /* format buffer size is specify */
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
    ipartition_number = 0;
    subret = esd_get_partition_id(&ipartition_number);
	if (SD_OK != subret) {
		NOTICE("BL2: Failed to esd_get_partition_id.\n");
		return SD_ERR;
	}

    return SD_OK;
}

/* End of File */
