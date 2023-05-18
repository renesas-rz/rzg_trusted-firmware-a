/*
 * Copyright (c) 2022-2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <assert.h>

#include <common/debug.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <r_sd_cfg.h>
#include <r_sdif.h>
#include <sd.h>

#include "io_common.h"
#include "io_sddrv.h"

#define DEV_SD0     (0)
#define DEV_SD1     (1)

typedef struct {
	uint32_t in_use;
	uintptr_t base;
	signed long long file_pos;
	uint32_t size;
	int32_t partition;
} file_state_t;

static file_state_t current_file = { 0 };

static io_type_t device_type_sddrv(void)
{
	return IO_TYPE_MEMMAP;
}

static const int32_t sd_port = DEV_SD0;
static uint8_t sd_work[SD_SIZE_OF_INIT] __aligned(8);
static uint8_t sd_rw_buff[SD_SECTOR_SIZE] __aligned(8);

static int sddrv_dev_open(const uintptr_t spec __attribute__ ((unused)),
				io_dev_info_t **dev_info);
static int sddrv_dev_close(io_dev_info_t *dev_info);


static int sddrv_block_seek(io_entity_t *entity, int32_t mode,
				  signed long long offset)
{
	if (mode != IO_SEEK_SET) {
		return -EINVAL;
	}

	((file_state_t *) entity->info)->file_pos = offset;

	return 0;
}

static int sddrv_block_read(io_entity_t *entity, uintptr_t buffer,
				  size_t length, size_t *length_read)
{
	file_state_t *fp = (file_state_t *) entity->info;
	uint32_t first_sector, last_sector, sector_count = 0;
	size_t buffer_offset = 0;

	first_sector = (fp->base + fp->file_pos) / SD_SECTOR_SIZE;
	last_sector = (fp->base + fp->file_pos + length - 1) / SD_SECTOR_SIZE;
	sector_count = last_sector - first_sector + 1;

	INFO("Load dst=0x%lx src=(p:%d)0x%llx(%d) len=0x%lx(%d)\n",
			buffer,
			fp->partition, (fp->base + fp->file_pos),
			first_sector, length, sector_count);

	assert((fp->file_pos + length) <= fp->size);

	// first sector
	uint32_t first_offset = (fp->base + fp->file_pos) % SD_SECTOR_SIZE;

	if (first_offset > 0) {
		memset(sd_rw_buff, 0x00, SD_SECTOR_SIZE);

		if (sd_read_sect(sd_port, (uint8_t *)sd_rw_buff, first_sector, 1) != SD_OK)
			return -EIO;

		buffer_offset = SD_SECTOR_SIZE - first_offset;
		buffer_offset = (length < buffer_offset) ? length : buffer_offset;

		memcpy((uint8_t *)buffer, &sd_rw_buff[first_offset], buffer_offset);

		first_sector++;
		sector_count--;
	}

	// last sector
	uint32_t last_offset = (fp->base + fp->file_pos + length) % SD_SECTOR_SIZE;

	if (0 < sector_count && 0 < last_offset) {
		memset(sd_rw_buff, 0x00, SD_SECTOR_SIZE);

		if (sd_read_sect(sd_port, (uint8_t *)sd_rw_buff, last_sector, 1) != SD_OK)
			return -EIO;

		memcpy((uint8_t *) buffer + (length - last_offset), &sd_rw_buff[0], last_offset);
		sector_count--;
	}

	// middle sector
	if (sector_count > 0) {
		if (sd_read_sect(sd_port, (uint8_t *)(buffer + buffer_offset),
				first_sector, sector_count) != SD_OK) {
			return -EIO;
		}
	}

	*length_read = length;
	fp->file_pos += (signed long long)length;

	return 0;
}

static int sddrv_block_write(io_entity_t *entity, const uintptr_t buffer,
			size_t length, size_t *length_written)
{
	file_state_t *fp = (file_state_t *) entity->info;
	uint32_t first_sector, last_sector, sector_count;
	size_t buffer_offset = 0;

	first_sector = (fp->base + fp->file_pos) / SD_SECTOR_SIZE;
	last_sector = (fp->base + fp->file_pos + length - 1) / SD_SECTOR_SIZE;
	sector_count = last_sector - first_sector + 1;

	assert((fp->file_pos + length) <= fp->size);

	// first sector
	uint32_t first_offset = (fp->base + fp->file_pos) % SD_SECTOR_SIZE;

	if (first_offset > 0) {
		memset(sd_rw_buff, 0x00, SD_SECTOR_SIZE);

		if (sd_read_sect(sd_port, sd_rw_buff, first_sector, 1) != SD_OK)
			return -EIO;

		buffer_offset = SD_SECTOR_SIZE - first_offset;
		buffer_offset = (length < buffer_offset) ? length : buffer_offset;

		memcpy((uint8_t *)&sd_rw_buff[first_offset], (uint8_t *)buffer, buffer_offset);

		if (sd_write_sect(sd_port, sd_rw_buff, first_sector, 1, SD_WRITE_WITH_PREERASE) != SD_OK)
			return -EIO;

		first_sector++;
		sector_count--;
	}

	// last sector
	uint32_t last_offset = (fp->base + fp->file_pos + length) % SD_SECTOR_SIZE;

	if ((sector_count > 0) && (last_offset > 0)) {
		memset(sd_rw_buff, 0x00, SD_SECTOR_SIZE);

		if (sd_read_sect(sd_port, sd_rw_buff, last_sector, 1) != SD_OK)
			return -EIO;

		memcpy((uint8_t *)&sd_rw_buff[0], (uint8_t *)buffer + (length - last_offset), last_offset);

		if (sd_write_sect(sd_port, sd_rw_buff, last_sector, 1, SD_WRITE_WITH_PREERASE) != SD_OK)
			return -EIO;

		sector_count--;
	}

	// middle sector
	if (sector_count > 0) {
		if (sd_write_sect(sd_port, (uint8_t *)buffer + buffer_offset, first_sector, sector_count, SD_WRITE_WITH_PREERASE) != SD_OK) {
			return -EIO;
		}
	}

	*length_written = length;
	fp->file_pos += (signed long long)length;

	return 0;
}

static int sddrv_block_len(io_entity_t *entity, size_t *length)
{
	*length = ((file_state_t *) entity->info)->size;

	INFO("%s: len: 0x%08lx\n", __func__, *length);

	return 0;
}

static int sddrv_block_open(io_dev_info_t *dev_info,
				const uintptr_t spec, io_entity_t *entity)
{
	st_sdhndl_t *p_hndl = SD_GET_HNDLS(sd_port);
	const io_drv_spec_t *block_spec = (io_drv_spec_t *) spec;

	if (current_file.in_use != 0U) {
		WARN("sd_block: Only one open spec at a time\n");
		return -EDEADLK;
	}

	current_file.base = block_spec->offset;
	current_file.size = block_spec->length;
	current_file.file_pos = 0;
	current_file.in_use = 1;
	current_file.partition = p_hndl->partition_id;

	INFO("SD boot from partition %d\n", current_file.partition);

	entity->info = (uintptr_t) &current_file;

	return 0;
}

static int sddrv_block_close(io_entity_t *entity)
{
	memset((void *)&current_file, 0, sizeof(current_file));
	entity->info = 0U;

	return 0;
}

static const io_dev_funcs_t sddrv_dev_funcs = {
	.type = &device_type_sddrv,
	.open = &sddrv_block_open,
	.seek = &sddrv_block_seek,
	.size = &sddrv_block_len,
	.read = &sddrv_block_read,
	.write = &sddrv_block_write,
	.close = &sddrv_block_close,
	.dev_init = NULL,
	.dev_close = &sddrv_dev_close
};

static const io_dev_info_t sddrv_dev_info = {
	.funcs = &sddrv_dev_funcs,
	.info = (uintptr_t) 0
};

static const io_dev_connector_t sddrv_dev_connector = {
	&sddrv_dev_open,
};

static int sddrv_dev_open(const uintptr_t spec __attribute__ ((unused)),
				io_dev_info_t **dev_info)
{
	uint16_t    type;

	*dev_info = (io_dev_info_t *) &sddrv_dev_info;

	if (sd_init(sd_port, SD_CFG_IP0_BASE, &sd_work[0], SD_CD_SOCKET) != SD_OK) {
		ERROR("Failed to sd_init.\n");
		panic();
	}

	/* Check if the card is inserted. *//* Cast to an appropriate type */
	if (sd_check_media(sd_port) != SD_OK) {
		ERROR("Failed to sd_check_media.\n");
		panic();
	}

	/* Initialize SD driver work buffer. *//* Cast to an appropriate type */
	if (sd_set_buffer(sd_port, &sd_rw_buff[0], SD_SECTOR_SIZE) != SD_OK) {
		ERROR("Failed to sd_set_buffer.\n");
		panic();
	}

	/* Mount SD card. *//* Cast to an appropriate type */
	if (sd_mount(sd_port, SD_CFG_DRIVER_MODE, SD_VOLT_3_3) != SD_OK) {
		ERROR("Failed to sd_mount.\n");
		panic();
	}

	if (sd_get_type(sd_port, &type, NULL, NULL) != SD_OK) {
		ERROR("Failed to sd_get_type.\n");
		panic();
	}

	if (((type & SD_MEDIA_SD) != SD_MEDIA_SD) &&
		((type & SD_MEDIA_EMBEDDED) != SD_MEDIA_EMBEDDED)) {
		/* Invalid card type */
		ERROR("Invalid SD media type.\n");
		panic();
	}

	return 0;
}

static int sddrv_dev_close(io_dev_info_t *dev_info)
{
	return 0;
}

int register_io_dev_sddrv(const io_dev_connector_t **dev_con)
{
	int rc;

	rc = io_register_device(&sddrv_dev_info);
	if (rc == 0) {
		*dev_con = &sddrv_dev_connector;
	}

	return rc;
}
