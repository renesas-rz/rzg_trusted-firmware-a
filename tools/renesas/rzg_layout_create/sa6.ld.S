/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

SECTIONS
{
	. = 0x00000000;
	.image_num : {
		KEEP(*(.sa6_image_num))
		. = 0x00000008;
	}

	.src_addr : {
		KEEP(*(.sa6_bl31src_addr))
		. = 0x00000008;
		KEEP(*(.sa6_bl31partition))
		. = 0x00000010;
		KEEP(*(.sa6_bl32src_addr))
		. = 0x00000018;
		KEEP(*(.sa6_bl32partition))
		. = 0x00000020;
		KEEP(*(.sa6_bl33src_addr))
		. = 0x00000028;
		KEEP(*(.sa6_bl33partition))
		. = 0x00000030;
		KEEP(*(.sa6_bl332src_addr))
		. = 0x00000038;
		KEEP(*(.sa6_bl332partition))
		. = 0x00000040;
		KEEP(*(.sa6_bl333src_addr))
		. = 0x00000048;
		KEEP(*(.sa6_bl333partition))
		. = 0x00000050;
		KEEP(*(.sa6_bl334src_addr))
		. = 0x00000058;
		KEEP(*(.sa6_bl334partition))
		. = 0x00000060;
		KEEP(*(.sa6_bl335src_addr))
		. = 0x00000068;
		KEEP(*(.sa6_bl335partition))
		. = 0x00000070;
		KEEP(*(.sa6_bl336src_addr))
		. = 0x00000078;
		KEEP(*(.sa6_bl336partition))
		. = 0x00000080;
		KEEP(*(.sa6_bl337src_addr))
		. = 0x00000088;
		KEEP(*(.sa6_bl337partition))
		. = 0x00000090;
		KEEP(*(.sa6_bl338src_addr))
		. = 0x00000098;
		KEEP(*(.sa6_bl338partition))
	}

	. = 0x00000554;

	.dst_addr : {
		KEEP(*(.sa6_bl31dst_addr))
		. = 0x00000004;
		KEEP(*(.sa6_bl31dst_addrh))
		. = 0x00000110;
		KEEP(*(.sa6_bl31dst_size))
		. = 0x00000800;
		KEEP(*(.sa6_bl32dst_addr))
		. = 0x00000804;
		KEEP(*(.sa6_bl32dst_addrh))
		. = 0x00000910;
		KEEP(*(.sa6_bl32dst_size))
		. = 0x00001000;
		KEEP(*(.sa6_bl33dst_addr))
		. = 0x00001004;
		KEEP(*(.sa6_bl33dst_addrh))
		. = 0x00001110;
		KEEP(*(.sa6_bl33dst_size))
		. = 0x00001800;
		KEEP(*(.sa6_bl332dst_addr))
		. = 0x00001804;
		KEEP(*(.sa6_bl332dst_addrh))
		. = 0x00001910;
		KEEP(*(.sa6_bl332dst_size))
		. = 0x00002000;
		KEEP(*(.sa6_bl333dst_addr))
		. = 0x00002004;
		KEEP(*(.sa6_bl333dst_addrh))
		. = 0x00002110;
		KEEP(*(.sa6_bl333dst_size))
		. = 0x00002800;
		KEEP(*(.sa6_bl334dst_addr))
		. = 0x00002804;
		KEEP(*(.sa6_bl334dst_addrh))
		. = 0x00002910;
		KEEP(*(.sa6_bl334dst_size))
		. = 0x00003000;
		KEEP(*(.sa6_bl335dst_addr))
		. = 0x00003004;
		KEEP(*(.sa6_bl335dst_addrh))
		. = 0x00003110;
		KEEP(*(.sa6_bl335dst_size))
		. = 0x00003800;
		KEEP(*(.sa6_bl336dst_addr))
		. = 0x00003804;
		KEEP(*(.sa6_bl336dst_addrh))
		. = 0x00003910;
		KEEP(*(.sa6_bl336dst_size))
		. = 0x00004000;
		KEEP(*(.sa6_bl337dst_addr))
		. = 0x00004004;
		KEEP(*(.sa6_bl337dst_addrh))
		. = 0x00004110;
		KEEP(*(.sa6_bl337dst_size))
		. = 0x00004800;
		KEEP(*(.sa6_bl338dst_addr))
		. = 0x00004804;
		KEEP(*(.sa6_bl338dst_addrh))
		. = 0x00004910;
		KEEP(*(.sa6_bl338dst_size))
	}

}
