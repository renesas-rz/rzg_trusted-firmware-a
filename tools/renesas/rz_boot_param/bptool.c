/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SPI_LOAD_OFFSET		(0x00000200)
#define MMC_LOAD_OFFSET		(0x00000400)
#define BOOT_PARAM_SIGN		(0xAA55FFFF)

struct
{
	uint32_t size;
	uint32_t pad1[3];
	uint32_t load;
	uint32_t pad2[3];
	uint32_t dest;
	uint32_t pad3[3];
	uint32_t pad4[115];
	uint32_t sign;
} bootparam;

static off_t fsize(const char *filename)
{
	struct stat st;
	if (stat(filename, &st) == 0)
		return st.st_size;
	return -1;
}

static int get_destadr(char *mode, uint32_t *ldr)
{
	int err = 0;

	if (0 == strcmp(mode, "spi"))
	{
		*ldr = SPI_LOAD_OFFSET;
	}
	else if (0 == strcmp(mode, "mmc"))
	{
		*ldr = MMC_LOAD_OFFSET;
	}
	else
	{
		err = -1;
	}

	return err;
}

int main(int argc, char *argv[])
{
	int err = -1;
	FILE *fp = NULL;

	char *mode = "spi";
	uint32_t load_offset_adr = 0;
	off_t size = 0;

	memset(&bootparam, 0xFF, sizeof(bootparam));

	if (2 >= argc)
	{
		printf("Usage: %s <IPL_FILE> <BOOT_PARAM> [BOOT_MODE]\n", argv[0]);
		printf("\t<IPL_FILE>       IPL file path. \n");
		printf("\t<BOOT_PARAM>     Boot parameter file path. \n");
		printf("\t[BOOT_MODE]      Boot mode selection (default: spi). \n");
		printf("\t                 - \"spi\", \"mmc\" \n");
		goto exit;
	}

	if (4 <= argc)
	{
		mode = argv[3];
	}
	
	if (NULL == (fp = fopen(argv[2], "wb")))
	{
		printf("Could not open file %s.\n", argv[2]);
		goto exit;
	}

	if (0 >= (size = fsize(argv[1])))
	{
		printf("Could not get size of file %s.\n", argv[1]);
		goto exit;
	}

	if (0 != get_destadr(mode, &load_offset_adr))
	{
		printf("Could not get load addr of boot mode %s.\n", mode);
		goto exit;
	}


	bootparam.size = (uint32_t)((size + 3) & (~0x3));
	bootparam.load = (uint32_t)load_offset_adr;
	bootparam.dest = (uint32_t)DEST_OFFSET_ADR;
	bootparam.sign = (uint32_t)BOOT_PARAM_SIGN;

	fwrite(&bootparam, 1, sizeof(bootparam), fp);

	err = 0;

exit:

	if (NULL != fp)
		fclose(fp);

	return err;
}
