/*
 * Copyright (c) 2022-2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_fip.h>
#include <io_common.h>
#include <io_emmcdrv.h>
#include <io_xspidrv.h>
#include <io_sddrv.h>
#include <lib/mmio.h>
#include <tools_share/firmware_image_package.h>

#include <rz_soc_def.h>
#include <sys.h>
#include <emmc_def.h>

static uintptr_t fip_dev_handle;
static uintptr_t xspidrv_dev_handle;
static uintptr_t emmcdrv_dev_handle;
static uintptr_t sddrv_dev_handle;

static uintptr_t boot_io_drv_id;

static const io_block_spec_t spirom_block_spec = {
	.offset = RZ_SOC_SPIROM_FIP_BASE,
	.length = RZ_SOC_SPIROM_FIP_SIZE,
};

static const io_drv_spec_t emmc_block_spec = {
	.offset = RZ_SOC_EMMC_FIP_BASE,
	.length = RZ_SOC_EMMC_FIP_SIZE,
};

static const io_drv_spec_t sd_block_spec = {
	.offset = RZ_SOC_SD_FIP_BASE,
	.length = RZ_SOC_SD_FIP_SIZE,
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
static const io_block_spec_t spirom_ddr_cfg_spec = {
	.offset = RZ_SOC_SPIROM_DDR_CFG_BASE,
	.length = RZ_SOC_SPIROM_DDR_CFG_SIZE,
};

static const io_drv_spec_t emmc_ddr_cfg_spec = {
	.offset = RZ_SOC_EMMC_DDR_CFG_BASE,
	.length = RZ_SOC_EMMC_DDR_CFG_SIZE,
};

static const io_drv_spec_t sd_ddr_cfg_spec = {
	.offset = RZ_SOC_SD_DDR_CFG_BASE,
	.length = RZ_SOC_SD_DDR_CFG_SIZE,
};
#endif /* PLAT_SYSTEM_SUSPEND */

#if PLAT_M33_BOOT_SUPPORT
#if TRUSTED_BOARD_BOOT
static const io_block_spec_t spirom_bl22_key_cert_file_spec = {
	.offset = RZG3S_SPIROM_M33_FW_BASE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t spirom_bl22_content_cert_file_spec = {
	.offset = spirom_bl22_key_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t spirom_bl22_image_spec = {
	.offset = spirom_bl22_content_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZG3S_M33_FW_SIZE,
};
#else /* TRUSTED_BOARD_BOOT */
static const io_block_spec_t spirom_bl22_image_spec = {
	.offset = RZG3S_SPIROM_M33_FW_BASE,
	.length = RZG3S_M33_FW_SIZE,
};
#endif /* TRUSTED_BOARD_BOOT */

#if TRUSTED_BOARD_BOOT
static const io_block_spec_t emmc_bl22_key_cert_file_spec = {
	.offset = RZG3S_EMMC_M33_FW_BASE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t emmc_bl22_content_cert_file_spec = {
	.offset = emmc_bl22_key_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t emmc_bl22_image_spec = {
	.offset = emmc_bl22_content_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZG3S_M33_FW_SIZE,
};
#else /* TRUSTED_BOARD_BOOT */
static const io_block_spec_t emmc_bl22_image_spec = {
	.offset = RZG3S_EMMC_M33_FW_BASE,
	.length = RZG3S_M33_FW_SIZE,
};
#endif /* TRUSTED_BOARD_BOOT */

#if TRUSTED_BOARD_BOOT
static const io_block_spec_t sd_bl22_key_cert_file_spec = {
	.offset = RZG3S_SD_M33_FW_BASE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t sd_bl22_content_cert_file_spec = {
	.offset = sd_bl22_key_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZ_TBB_CERT_SIZE,
};
static const io_block_spec_t sd_bl22_image_spec = {
	.offset = sd_bl22_content_cert_file_spec.offset + RZ_TBB_CERT_SIZE,
	.length = RZG3S_M33_FW_SIZE,
};
#else /* TRUSTED_BOARD_BOOT */
static const io_block_spec_t sd_bl22_image_spec = {
	.offset = RZG3S_SD_M33_FW_BASE,
	.length = RZG3S_M33_FW_SIZE,
};
#endif /* TRUSTED_BOARD_BOOT */
#endif /* PLAT_M33_BOOT_SUPPORT */

static int32_t open_emmcdrv(const uintptr_t spec);
static int32_t open_xspidrv(const uintptr_t spec);
static int32_t open_sddrv(const uintptr_t spec);
static int32_t open_fipdrv(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy emmc_fip_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_block_spec,
	&open_emmcdrv
};
static const struct plat_io_policy spirom_fip_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_block_spec,
	&open_xspidrv
};
static const struct plat_io_policy sd_fip_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_block_spec,
	&open_sddrv
};

#if PLAT_SYSTEM_SUSPEND
static const struct plat_io_policy emmc_ddr_config_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_ddr_cfg_spec,
	&open_emmcdrv
};
static const struct plat_io_policy spirom_ddr_config_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_ddr_cfg_spec,
	&open_xspidrv
};
static const struct plat_io_policy sd_ddr_config_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_ddr_cfg_spec,
	&open_sddrv
};
#endif /* PLAT_SYSTEM_SUSPEND */

#if PLAT_M33_BOOT_SUPPORT
static const struct plat_io_policy emmc_bl22_image_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_bl22_image_spec,
	&open_emmcdrv
};
static const struct plat_io_policy spirom_bl22_image_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_bl22_image_spec,
	&open_xspidrv
};
static const struct plat_io_policy sd_bl22_image_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_bl22_image_spec,
	&open_sddrv
};
#if TRUSTED_BOARD_BOOT
static const struct plat_io_policy emmc_bl22_kcert_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_bl22_key_cert_file_spec,
	&open_emmcdrv
};
static const struct plat_io_policy spirom_bl22_kcert_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_bl22_key_cert_file_spec,
	&open_xspidrv
};
static const struct plat_io_policy sd_bl22_kcert_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_bl22_key_cert_file_spec,
	&open_sddrv
};
static const struct plat_io_policy emmc_bl22_ccert_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_bl22_content_cert_file_spec,
	&open_emmcdrv
};
static const struct plat_io_policy spirom_bl22_ccert_policy = {
	&xspidrv_dev_handle,
	(uintptr_t) &spirom_bl22_content_cert_file_spec,
	&open_xspidrv
};
static const struct plat_io_policy sd_bl22_ccert_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_bl22_content_cert_file_spec,
	&open_sddrv
};
#endif /* TRUSTED_BOARD_BOOT */
#endif /* PLAT_M33_BOOT_SUPPORT */

static struct plat_io_policy policies[MAX_NUMBER_IDS] = {
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

static int32_t open_xspidrv(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(xspidrv_dev_handle, 0);
	if (result != 0)
		return result;

	result = io_open(xspidrv_dev_handle, spec, &handle);
	if (result == 0)
		io_close(handle);

	return result;
}

static int32_t open_emmcdrv(const uintptr_t spec)
{
	return io_dev_init(emmcdrv_dev_handle, 0);
}

static int32_t open_sddrv(const uintptr_t spec)
{
	return io_dev_init(sddrv_dev_handle, 0);
}

static void update_dev_policies(uint16_t boot_mode)
{
	switch (boot_mode) {
	case SYS_BOOT_MODE_SPI_1_8:
	case SYS_BOOT_MODE_SPI_3_3:
		policies[FIP_IMAGE_ID]			= spirom_fip_policy;
#if PLAT_SYSTEM_SUSPEND
		policies[DDR_CONFIG_ID]			= spirom_ddr_config_policy;
#endif
#if PLAT_M33_BOOT_SUPPORT
		policies[BL22_IMAGE_ID]			= spirom_bl22_image_policy;
#if TRUSTED_BOARD_BOOT
		policies[BL22_KEY_CERT_ID]		= spirom_bl22_kcert_policy;
		policies[BL22_CONTENT_CERT_ID]	= spirom_bl22_ccert_policy;
#endif
#endif
		break;
	case SYS_BOOT_MODE_EMMC_1_8:
	case SYS_BOOT_MODE_EMMC_3_3:
		policies[FIP_IMAGE_ID]			= emmc_fip_policy;
#if PLAT_SYSTEM_SUSPEND
		policies[DDR_CONFIG_ID]			= emmc_ddr_config_policy;
#endif
#if PLAT_M33_BOOT_SUPPORT
		policies[BL22_IMAGE_ID]			= emmc_bl22_image_policy;
#if TRUSTED_BOARD_BOOT
		policies[BL22_KEY_CERT_ID]		= emmc_bl22_kcert_policy;
		policies[BL22_CONTENT_CERT_ID]	= emmc_bl22_ccert_policy;
#endif
#endif
		break;
	case SYS_BOOT_MODE_ESD:
		policies[FIP_IMAGE_ID]			= sd_fip_policy;
#if PLAT_SYSTEM_SUSPEND
		policies[DDR_CONFIG_ID]			= sd_ddr_config_policy;
#endif
#if PLAT_M33_BOOT_SUPPORT
		policies[BL22_IMAGE_ID]			= sd_bl22_image_policy;
#if TRUSTED_BOARD_BOOT
		policies[BL22_KEY_CERT_ID]		= sd_bl22_kcert_policy;
		policies[BL22_CONTENT_CERT_ID]	= sd_bl22_ccert_policy;
#endif
#endif
		break;
	default:
		panic();
	}
}

void rz_io_setup(void)
{
	const io_dev_connector_t *xspi;
	const io_dev_connector_t *emmc;
	const io_dev_connector_t *sd;
	const io_dev_connector_t *rzsoc;
	uint16_t boot_mode;

	boot_mode = sys_get_boot_mode();

	boot_io_drv_id = FIP_IMAGE_ID;

	register_io_dev_fip(&rzsoc);

	io_dev_open(rzsoc, 0, &fip_dev_handle);

	if (boot_mode == SYS_BOOT_MODE_SPI_1_8 ||
		boot_mode == SYS_BOOT_MODE_SPI_3_3) {
		register_io_dev_xspidrv(&xspi);
		io_dev_open(xspi, 0, &xspidrv_dev_handle);
	} else if (boot_mode == SYS_BOOT_MODE_EMMC_1_8 ||
			   boot_mode == SYS_BOOT_MODE_EMMC_3_3) {
		register_io_dev_emmcdrv(&emmc);
		io_dev_open(emmc, 0, &emmcdrv_dev_handle);
	} else if (boot_mode == SYS_BOOT_MODE_ESD) {
		register_io_dev_sddrv(&sd);
		io_dev_open(sd, 0, &sddrv_dev_handle);
	} else {
		ERROR("Unsupported IO device %d.\n", boot_mode);
		panic();
	}
	update_dev_policies(boot_mode);
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
