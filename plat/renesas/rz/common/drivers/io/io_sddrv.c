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
#include <emmc_config.h>
#include <emmc_def.h>
#include <emmc_hal.h>
#include <emmc_std.h>
#include <sd.h>
#include <esdif.h>

#include "io_common.h"
#include "io_sddrv.h"


typedef struct {
	uint32_t in_use;
	uintptr_t base;
	signed long long file_pos;
	uint32_t size;
	EMMC_PARTITION_ID partition;
} file_state_t;

static file_state_t current_file = { 0 };

static io_type_t device_type_sddrv(void)
{
	return IO_TYPE_MEMMAP;
}

static uint8_t sector_buf[EMMC_SECTOR_SIZE] __aligned(8);

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

	first_sector = (fp->base + fp->file_pos) >> EMMC_SECTOR_SIZE_SHIFT;
	last_sector = (fp->base + fp->file_pos + length - 1) >> EMMC_SECTOR_SIZE_SHIFT;
	sector_count = last_sector - first_sector + 1;

	INFO("Load dst=0x%lx src=(p:%d)0x%llx(%d) len=0x%lx(%d)\n",
			buffer,
			fp->partition, (fp->base + fp->file_pos),
			first_sector, length, sector_count);

	assert((fp->file_pos + length) <= fp->size);

	// first sector
	uint32_t first_offset = (fp->base + fp->file_pos) % EMMC_SECTOR_SIZE;

	if (first_offset > 0) {
		memset(sector_buf, 0x00, EMMC_SECTOR_SIZE);

		if (esd_read_sect((uint8_t *)sector_buf, first_sector, 1) != SD_OK)
			return -EIO;

		buffer_offset = EMMC_SECTOR_SIZE - first_offset;
		buffer_offset = (length < buffer_offset) ? length : buffer_offset;

		memcpy((uint8_t *)buffer, &sector_buf[first_offset], buffer_offset);

		first_sector++;
		sector_count--;
	}

	// last sector
	uint32_t last_offset = (fp->base + fp->file_pos + length) % EMMC_SECTOR_SIZE;

	if (0 < sector_count && 0 < last_offset) {
		memset(sector_buf, 0x00, EMMC_SECTOR_SIZE);

		if (esd_read_sect((uint8_t *)sector_buf, last_sector, 1) != SD_OK)
			return -EIO;

		memcpy((uint8_t *) buffer + (length - last_offset), &sector_buf[0], last_offset);
		sector_count--;
	}

	// middle sector
	if (sector_count > 0) {
		if (esd_read_sect((uint8_t *)(buffer + buffer_offset),
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
	return -EIO;
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
	const io_drv_spec_t *block_spec = (io_drv_spec_t *) spec;

	if (current_file.in_use != 0U) {
		WARN("sd_block: Only one open spec at a time\n");
		return -EDEADLK;
	}

	current_file.base = block_spec->offset;
	current_file.size = block_spec->length;
	current_file.file_pos = 0;
	current_file.in_use = 1;

	current_file.partition = mmc_drv_obj.boot_partition_en;
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
	*dev_info = (io_dev_info_t *) &sddrv_dev_info;

	if (esd_main() != SD_OK) {
		ERROR("Failed to eSD driver initialize.\n");
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
