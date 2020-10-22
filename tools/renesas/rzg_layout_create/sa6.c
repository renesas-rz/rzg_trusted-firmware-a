/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#define RZG_SA6_TYPE_HYPERFLASH	(0)
#define RZG_SA6_TYPE_EMMC		(1)

#if (RZG_SA6_TYPE == RZG_SA6_TYPE_HYPERFLASH)

/* Number of content cert for Non-secure Target Program(BL33x) */
#define RZG_IMAGE_NUM			(0x00000001U)
/* Source address on flash for BL31 */
#define RZG_BL31SRC_ADDRESS		(0x001C0000U)
/* Reserved */
#define RZG_BL31_PARTITION		(0x00000000U)
/* Source address on flash for BL32 */
#define RZG_BL32SRC_ADDRESS		(0x00200000U)
/* Reserved */
#define RZG_BL32_PARTITION		(0x00000000U)
/* Source address on flash for BL33 */
#define RZG_BL33SRC_ADDRESS		(0x00300000U)
/* Reserved */
#define RZG_BL33_PARTITION		(0x00000000U)
#define RZG_BL332SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL332_PARTITION		(0x00000000U)
#define RZG_BL333SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL333_PARTITION		(0x00000000U)
#define RZG_BL334SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL334_PARTITION		(0x00000000U)
#define RZG_BL335SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL335_PARTITION		(0x00000000U)
#define RZG_BL336SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL336_PARTITION		(0x00000000U)
#define RZG_BL337SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL337_PARTITION		(0x00000000U)
#define RZG_BL338SRC_ADDRESS		(0x00000000U)
/* Reserved */
#define RZG_BL338_PARTITION		(0x00000000U)

#else /* RZG_SA6_TYPE == RZG_SA6_TYPE_HYPERFLASH */

/* Number of content cert for Non-secure Target Program(BL33x) */
#define RZG_IMAGE_NUM			(0x00000001U)
/* Source address on eMMC for BL31 */
#define RZG_BL31SRC_ADDRESS		(0x00040000U)
/* Source partition on eMMC for BL31 */
#define RZG_BL31_PARTITION		(0x00000001U)
/* Source address on eMMC for BL32 */
#define RZG_BL32SRC_ADDRESS		(0x00200000U)
/* Source partition on eMMC for BL32 */
#define RZG_BL32_PARTITION		(0x00000001U)
/* Source address on eMMC for BL33 */
#define RZG_BL33SRC_ADDRESS		(0x00000000U)
/* Source partition on eMMC for BL33 */
#define RZG_BL33_PARTITION		(0x00000002U)
/* Reserved */
#define RZG_BL332SRC_ADDRESS		(0x00000000U)
#define RZG_BL332_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL333SRC_ADDRESS		(0x00000000U)
#define RZG_BL333_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL334SRC_ADDRESS		(0x00000000U)
#define RZG_BL334_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL335SRC_ADDRESS		(0x00000000U)
#define RZG_BL335_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL336SRC_ADDRESS		(0x00000000U)
#define RZG_BL336_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL337SRC_ADDRESS		(0x00000000U)
#define RZG_BL337_PARTITION		(0x00000000U)
/* Reserved */
#define RZG_BL338SRC_ADDRESS		(0x00000000U)
#define RZG_BL338_PARTITION		(0x00000000U)

#endif /* RZG_SA6_TYPE == RZG_SA6_TYPE_HYPERFLASH */

/* Destination address for BL31 */
#define RZG_BL31DST_ADDRESS		(0x44000000U)
#define RZG_BL31DST_ADDRESSH		(0x00000000U)
/* Destination size for BL31 */
#define RZG_BL31DST_SIZE		(0x00004000U)
/* Destination address for BL32 */
#define RZG_BL32DST_ADDRESS		(0x44100000U)
#define RZG_BL32DST_ADDRESSH		(0x00000000U)
/* Destination size for BL32 */
#define RZG_BL32DST_SIZE		(0x00040000U)
/* Destination address for BL33 */
#define RZG_BL33DST_ADDRESS		(0x50000000U)
#define RZG_BL33DST_ADDRESSH		(0x00000000U)
/* Destination size for BL33 */
#define RZG_BL33DST_SIZE		(0x00040000U)
/* Reserved */
#define RZG_BL332DST_ADDRESS		(0x00000000U)
#define RZG_BL332DST_ADDRESSH		(0x00000000U)
#define RZG_BL332DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL333DST_ADDRESS		(0x00000000U)
#define RZG_BL333DST_ADDRESSH		(0x00000000U)
#define RZG_BL333DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL334DST_ADDRESS		(0x00000000U)
#define RZG_BL334DST_ADDRESSH		(0x00000000U)
#define RZG_BL334DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL335DST_ADDRESS		(0x00000000U)
#define RZG_BL335DST_ADDRESSH		(0x00000000U)
#define RZG_BL335DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL336DST_ADDRESS		(0x00000000U)
#define RZG_BL336DST_ADDRESSH		(0x00000000U)
#define RZG_BL336DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL337DST_ADDRESS		(0x00000000U)
#define RZG_BL337DST_ADDRESSH		(0x00000000U)
#define RZG_BL337DST_SIZE		(0x00000000U)
/* Reserved */
#define RZG_BL338DST_ADDRESS		(0x00000000U)
#define RZG_BL338DST_ADDRESSH		(0x00000000U)
#define RZG_BL338DST_SIZE		(0x00000000U)

/* SA6 */
const uint64_t __attribute__ ((section (".sa6_image_num")))	image_num	= RZG_IMAGE_NUM;
const uint64_t __attribute__ ((section (".sa6_bl31src_addr")))	bl31src_addr	= RZG_BL31SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl31partition")))	bl31partition	= RZG_BL31_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl32src_addr")))	bl32src_addr	= RZG_BL32SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl32partition")))	bl32partition	= RZG_BL32_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl33src_addr")))	bl33src_addr	= RZG_BL33SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl33partition")))	bl33partition	= RZG_BL33_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl332src_addr")))	bl332src_addr	= RZG_BL332SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl332partition")))bl332partition	= RZG_BL332_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl333src_addr")))	bl333src_addr	= RZG_BL333SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl333partition")))bl333partition	= RZG_BL333_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl334src_addr")))	bl334src_addr	= RZG_BL334SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl334partition")))bl334partition	= RZG_BL334_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl335src_addr")))	bl335src_addr	= RZG_BL335SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl335partition")))bl335partition	= RZG_BL335_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl336src_addr")))	bl336src_addr	= RZG_BL336SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl336partition")))bl336partition	= RZG_BL336_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl337src_addr")))	bl337src_addr	= RZG_BL337SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl337partition")))bl337partition	= RZG_BL337_PARTITION;
const uint64_t __attribute__ ((section (".sa6_bl338src_addr")))	bl338src_addr	= RZG_BL338SRC_ADDRESS;
const uint64_t __attribute__ ((section (".sa6_bl338partition")))bl338partition	= RZG_BL338_PARTITION;
const uint32_t __attribute__ ((section (".sa6_bl31dst_addr")))	bl31dst_addr	= RZG_BL31DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl31dst_addrh")))	bl31dst_addrh	= RZG_BL31DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl31dst_size")))	bl31dst_size	= RZG_BL31DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl32dst_addr")))	bl32dst_addr	= RZG_BL32DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl32dst_addrh")))	bl32dst_addrh	= RZG_BL32DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl32dst_size")))	bl32dst_size	= RZG_BL32DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl33dst_addr")))	bl33dst_addr	= RZG_BL33DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl33dst_addrh")))	bl33dst_addrh	= RZG_BL33DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl33dst_size")))	bl33dst_size	= RZG_BL33DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl332dst_addr")))	bl332dst_addr	= RZG_BL332DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl332dst_addrh")))bl332dst_addrh	= RZG_BL332DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl332dst_size"))) bl332dst_size	= RZG_BL332DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl333dst_addr")))	bl333dst_addr	= RZG_BL333DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl333dst_addrh")))bl333dst_addrh	= RZG_BL333DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl333dst_size")))	bl333dst_size	= RZG_BL333DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl334dst_addr")))	bl334dst_addr	= RZG_BL334DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl334dst_addrh")))bl334dst_addrh	= RZG_BL334DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl334dst_size")))	bl334dst_size	= RZG_BL334DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl335dst_addr")))	bl335dst_addr	= RZG_BL335DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl335dst_addrh")))bl335dst_addrh	= RZG_BL335DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl335dst_size")))	bl335dst_size	= RZG_BL335DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl336dst_addr")))	bl336dst_addr	= RZG_BL336DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl336dst_addrh")))bl336dst_addrh	= RZG_BL336DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl336dst_size")))	bl336dst_size	= RZG_BL336DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl337dst_addr")))	bl337dst_addr	= RZG_BL337DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl337dst_addrh")))bl337dst_addrh	= RZG_BL337DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl337dst_size")))	bl337dst_size	= RZG_BL337DST_SIZE;
const uint32_t __attribute__ ((section (".sa6_bl338dst_addr")))	bl338dst_addr	= RZG_BL338DST_ADDRESS;
const uint32_t __attribute__ ((section (".sa6_bl338dst_addrh")))bl338dst_addrh	= RZG_BL338DST_ADDRESSH;
const uint32_t __attribute__ ((section (".sa6_bl338dst_size")))	bl338dst_size	= RZG_BL338DST_SIZE;
