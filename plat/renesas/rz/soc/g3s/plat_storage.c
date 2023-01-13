/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_fip.h>
#include <drivers/io/io_memmap.h>
#include <io_common.h>
#include <io_emmcdrv.h>
#include <lib/mmio.h>
#include <tools_share/firmware_image_package.h>

#include <rz_soc_def.h>
#include <sys.h>
#include <xspi.h>
#include <emmc_def.h>

static uintptr_t memdrv_dev_handle;
static uintptr_t fip_dev_handle;
static uintptr_t emmcdrv_dev_handle;

static uintptr_t boot_io_drv_id;

static const io_block_spec_t spirom_block_spec = {
	.offset = RZ_SOC_SPIROM_FIP_BASE,
	.length = RZG3S_FIP_SIZE_MAX,
};

static const io_drv_spec_t emmc_block_spec = {
	.offset = RZ_SOC_EMMC_FIP_BASE,
	.length = RZG3S_FIP_SIZE_MAX,
};

static const io_uuid_spec_t bl31_file_spec = {
	.uuid = UUID_EL3_RUNTIME_FIRMWARE_BL31,
};

static const io_uuid_spec_t bl32_file_spec = {
	.uuid = UUID_SECURE_PAYLOAD_BL32,
};

static const io_uuid_spec_t bl33_file_spec = {
	.uuid = UUID_NON_TRUSTED_FIRMWARE_BL33,
};

#if TRUSTED_BOARD_BOOT
static const io_uuid_spec_t soc_fw_key_cert_file_spec = {
	.uuid = UUID_SOC_FW_KEY_CERT,
};

static const io_uuid_spec_t soc_fw_content_cert_file_spec = {
	.uuid = UUID_SOC_FW_CONTENT_CERT,
};

static const io_uuid_spec_t tos_fw_key_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_KEY_CERT,
};

static const io_uuid_spec_t tos_fw_content_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_CONTENT_CERT,
};

static const io_uuid_spec_t nt_fw_key_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_KEY_CERT,
};

static const io_uuid_spec_t nt_fw_content_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_CONTENT_CERT,
};
#endif

#if PLAT_SYSTEM_SUSPEND
static const io_block_spec_t spirom_s2r_prm_spec = {
	.offset = RZ_SOC_SPIROM_DDR_CFG_BASE,
	.length = RZ_SOC_SPIROM_DDR_CFG_SIZE,
};

static const io_drv_spec_t emmc_s2r_prm_spec = {
	.offset = RZ_SOC_EMMC_DDR_CFG_BASE,
	.length = RZ_SOC_EMMC_DDR_CFG_SIZE,
};
#endif /* PLAT_SYSTEM_SUSPEND */

#if PLAT_SUBCORE_BOOT
static const io_uuid_spec_t bl22_file_spec = {
	.uuid = UUID_TRUSTED_FIRMWARE_BL22,
};

#if TRUSTED_BOARD_BOOT
static const io_uuid_spec_t score_fw_key_cert_file_spec = {
	.uuid = UUID_SUBCORE_KEY_CERT,
};

static const io_uuid_spec_t score_fw_content_cert_file_spec = {
	.uuid = UUID_SUBCORE_CONTENT_CERT,
};
#endif /* TRUSTED_BOARD_BOOT */
#endif /* PLAT_SUBCORE_BOOT */

static int32_t open_emmcdrv(const uintptr_t spec);
static int32_t open_memmap(const uintptr_t spec);
static int32_t open_fipdrv(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy fip_policies[] = {
	[SYS_BOOT_MODE_EMMC_1_8] = {
				&emmcdrv_dev_handle,
				(uintptr_t) &emmc_block_spec,
				&open_emmcdrv},
	[SYS_BOOT_MODE_EMMC_3_3] = {
				&emmcdrv_dev_handle,
				(uintptr_t) &emmc_block_spec,
				&open_emmcdrv},
	[SYS_BOOT_MODE_SPI_1_8] = {
				&memdrv_dev_handle,
				(uintptr_t) &spirom_block_spec,
				&open_memmap},
	[SYS_BOOT_MODE_SPI_3_3] = {
				&memdrv_dev_handle,
				(uintptr_t) &spirom_block_spec,
				&open_memmap},
};

#if PLAT_SYSTEM_SUSPEND
static const struct plat_io_policy ddr_cfg_policies[] = {
	[SYS_BOOT_MODE_EMMC_1_8] = {
				&emmcdrv_dev_handle,
				(uintptr_t) &emmc_s2r_prm_spec,
				&open_emmcdrv},
	[SYS_BOOT_MODE_EMMC_3_3] = {
				&emmcdrv_dev_handle,
				(uintptr_t) &emmc_s2r_prm_spec,
				&open_emmcdrv},
	[SYS_BOOT_MODE_SPI_1_8] = {
				&memdrv_dev_handle,
				(uintptr_t) &spirom_s2r_prm_spec,
				&open_memmap},
	[SYS_BOOT_MODE_SPI_3_3] = {
				&memdrv_dev_handle,
				(uintptr_t) &spirom_s2r_prm_spec,
				&open_memmap},
};
#endif /* PLAT_SYSTEM_SUSPEND */

static struct plat_io_policy policies[] = {
    [FIP_IMAGE_ID]  = {0, 0, 0},
	[BL31_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl31_file_spec,
				&open_fipdrv},
	[BL32_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl32_file_spec,
				&open_fipdrv},
	[BL33_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl33_file_spec,
				&open_fipdrv},
#if TRUSTED_BOARD_BOOT
	[SOC_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_key_cert_file_spec,
				&open_fipdrv},
	[SOC_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_content_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_key_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_content_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_key_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_content_cert_file_spec,
				&open_fipdrv},
#endif /* TRUSTED_BOARD_BOOT */
#if PLAT_SYSTEM_SUSPEND
    [DDR_CONFIG_ID] = {0, 0, 0},
#endif /* PLAT_SYSTEM_SUSPEND */
#if PLAT_SUBCORE_BOOT
    [BL22_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl22_file_spec,
				&open_fipdrv},
#if TRUSTED_BOARD_BOOT
	[SUBCORE_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &score_fw_key_cert_file_spec,
				&open_fipdrv},
	[SUBCORE_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &score_fw_content_cert_file_spec,
				&open_fipdrv},
#endif /* TRUSTED_BOARD_BOOT */
#endif /* PLAT_SUBCORE_BOOT */
	{0, 0, 0}
};

static int32_t open_fipdrv(const uintptr_t spec)
{
	int32_t result;

	result = io_dev_init(fip_dev_handle, boot_io_drv_id);
	if (result != 0)
		return result;

	return result;
}

static int32_t open_memmap(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(memdrv_dev_handle, 0);
	if (result != 0)
		return result;

	result = io_open(memdrv_dev_handle, spec, &handle);
	if (result == 0)
		io_close(handle);

	return result;
}

static int32_t open_emmcdrv(const uintptr_t spec)
{
	return io_dev_init(emmcdrv_dev_handle, 0);
}

void rz_io_setup(void)
{
	const io_dev_connector_t *memmap;
	const io_dev_connector_t *emmc;
	const io_dev_connector_t *rzsoc;
	uint16_t boot_mode;

	boot_mode = sys_get_boot_mode();

	boot_io_drv_id = FIP_IMAGE_ID;

	register_io_dev_fip(&rzsoc);

	io_dev_open(rzsoc, 0, &fip_dev_handle);

	if (boot_mode == SYS_BOOT_MODE_SPI_1_8 ||
		boot_mode == SYS_BOOT_MODE_SPI_3_3) {
		xspi_setup();
		register_io_dev_memmap(&memmap);
		io_dev_open(memmap, 0, &memdrv_dev_handle);
	} else if (boot_mode == SYS_BOOT_MODE_EMMC_1_8 ||
			   boot_mode == SYS_BOOT_MODE_EMMC_3_3) {
		if (emmc_init() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC driver initialize.\n");
			panic();
		}
		emmc_memcard_power(EMMC_POWER_ON);
		if (emmc_mount() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC mount operation.\n");
			panic();
		}

		register_io_dev_emmcdrv(&emmc);
		io_dev_open(emmc, 0, &emmcdrv_dev_handle);
	} else {
		panic();
	}
	policies[FIP_IMAGE_ID] = fip_policies[boot_mode];
    
#if PLAT_SYSTEM_SUSPEND
    policies[DDR_CONFIG_ID] = ddr_cfg_policies[boot_mode];
#endif /* PLAT_SYSTEM_SUSPEND */
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
				uintptr_t *image_spec)
{
	const struct plat_io_policy *policy;
	int result;

	policy = &policies[image_id];

	result = policy->check(policy->image_spec);
	if (result != 0)
		return result;

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return 0;
}

