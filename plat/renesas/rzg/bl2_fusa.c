/*
 * bl2_ecc_setup.c -
 *
 * Copyright (C) 2020 Renesas Electronics Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>

#if (RZG_DRAM_ECC == 1)
#define MB(x)			((x) << 20)

#define FUSA_DRAM_CLEAR		1

#define NUM_DFUSACR		8
#define NUM_EFUSACR		8
#define BANK_SIZE		(2048)
#define AREA_SIZE		(BANK_SIZE / NUM_DFUSACR)
#define EXTRA_SPLIT_SIZE	(AREA_SIZE / NUM_EFUSACR)
#define DRAM_ADDR_BASE		(0x400000000)
#define DRAM_XOR_ADDR		(0x200000000)

#define	DFUSAAREACR		0xE6785000	/* DRAM FuSa Area Conf */
#define	DECCAREACR		0xE6785040	/* DRAM ECC Area Conf */
#define	NUM_DAREA		16
#define	FUSACR			0xE6784020	/* FuSa Ctrl */

#define	ADSPLCR0		0xE6784008	/* Address Split Control 0 */
#define	ADSPLCR1		0xE678400C	/* Address Split Control 1 */
#define	ADSPLCR2		0xE6784010	/* Address Split Control 2 */
#define	ADSPLCR3		0xE6784014	/* Address Split Control 3 */

/* As the saddr, specify high-memory address (> 4 GB) */
#define	FUSAAREACR(en, size, saddr)     \
       (((uint32_t)en << 31) | ((uint32_t)size << 24) | (uint32_t)(((uintptr_t)saddr) >> 12))
#define ECCAREACR(ecc, saddr) \
       (((uint32_t)ecc << 31) | (uint32_t)(((uintptr_t)saddr) >> 12))

#define EFUSASEL(x)		((uint32_t)x & 0xff) << 24	/* Setting for Extra Split mode */
#define DFUSASEL(x)		((uint32_t)x & 0xff) << 16	/* Setting for DRAM */
#define SFUSASEL(x)		((uint32_t)x & 0x3f)		/* Setting for SRAM */

#define SPLITSEL(x)		((uint32_t)x & 0xff) << 16	/* Setting for Split mode */
#define AREA(x)			((uint32_t)x & 0x1f) << 8	/* Address bit devides DBSC into 8 areas */
#define SWP(x)			((uint32_t)x & 0x1f)		/* Address bit to interleave DBSCs in split mode */
#define ADRMODE(x)		((uint32_t)x & 0x1) << 31	/* Select address mapping mode */

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)		(sizeof(a) / sizeof(a[0]))
#endif

extern void rcar_swdt_init_counter(void);

struct rzg2_ecc_conf {
	uint32_t fusaareacr;
	uint32_t eccareacr;
};

#if (RCAR_LSI == RZ_G2E)
static const struct rzg2_ecc_conf rzg2_ek874_conf[] = {
#if (RZG_DRAM_ECC_FULL == 2) /* ECC Full mode single channel */
	{ FUSAAREACR(1, 7, 0x408000000),    ECCAREACR(0, 0x448000000)   }, /* 128+128 MB */
	{ FUSAAREACR(1, 8, 0x410000000),    ECCAREACR(0, 0x450000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 8, 0x420000000),    ECCAREACR(0, 0x460000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 8, 0x430000000),    ECCAREACR(0, 0x470000000)   }, /* 256+256 MB */
	{ FUSAAREACR(1, 6, 0x440000000),    ECCAREACR(0, 0x444000000)   }, /* 64+64 MB */
#else /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
#endif /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
};
#endif /* RCAR_LSI == RZ_G2E */

#if (RCAR_LSI == RZ_G2N)
static const struct rzg2_ecc_conf rzg2_hihope_rzg2n_conf[] = {
#if (RZG_DRAM_ECC_FULL == 2) /* ECC Full mode single channel */
	{ FUSAAREACR(1, 7, 0x408000000),    ECCAREACR(0, 0x498000000)   }, /* 128+128 MB */
	{ FUSAAREACR(1, 9, 0x430000000),    ECCAREACR(0, 0x4A0000000)   }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x450000000),    ECCAREACR(0, 0x4C0000000)   }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x470000000),    ECCAREACR(0, 0x4E0000000)   }, /* 512+512 MB */
	{ FUSAAREACR(1, 6, 0x490000000),    ECCAREACR(0, 0x494000000)   }, /* 64+64 MB */
#else /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
#endif /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
	{ FUSAAREACR(0, 0, 0),    ECCAREACR(0, 0)   },
};
#endif /* RCAR_LSI == RZ_G2N */

#if (RCAR_LSI == RZ_G2H)
static const struct rzg2_ecc_conf rzg2_hihope_rzg2h_conf[] = {
#if (RZG_DRAM_ECC_FULL == 2)   /* ECC Full mode single channel */
	{ FUSAAREACR(1, 6, 0x450000000), ECCAREACR(0, 0x454000000) }, /* 64+64 MB */
	{ FUSAAREACR(1, 7, 0x408000000), ECCAREACR(0, 0x458000000) }, /* 128+128 MB */
	{ FUSAAREACR(1, 9, 0x430000000), ECCAREACR(0, 0x460000000) }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x600000000), ECCAREACR(0, 0x640000000) }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x620000000), ECCAREACR(0, 0x660000000) }, /* 512+512 MB */
#else /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
#endif /* (RZG_DRAM_ECC_FULL == 2 */
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
};
#endif /* RCAR_LSI == RZ_G2H */

#if (RCAR_LSI == RZ_G2M)
static const struct rzg2_ecc_conf rzg2_hihope_rzg2m_conf[] = {
#if (RZG_DRAM_ECC_FULL == 2)   /* ECC Full mode single channel */
	{ FUSAAREACR(1, 6, 0x450000000), ECCAREACR(0, 0x454000000) }, /* 64+64 MB */
	{ FUSAAREACR(1, 7, 0x408000000), ECCAREACR(0, 0x458000000) }, /* 128+128 MB */
	{ FUSAAREACR(1, 9, 0x430000000), ECCAREACR(0, 0x460000000) }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x600000000), ECCAREACR(0, 0x640000000) }, /* 512+512 MB */
	{ FUSAAREACR(1, 9, 0x620000000), ECCAREACR(0, 0x660000000) }, /* 512+512 MB */
#else /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
#endif /* (RZG_DRAM_ECC_FULL == 2) */
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
	{ FUSAAREACR(0, 0, 0), ECCAREACR(0, 0) },
};
#endif /* RCAR_LSI == RZ_G2M */

#if (RCAR_LSI == RZ_G2M || RCAR_LSI == RZ_G2H)
#if (RZG_DRAM_ECC_FULL == 1) /* ECC Full mode dual channel */
static const uint32_t fusacr = EFUSASEL(0xF0) | DFUSASEL(0xF8)| SFUSASEL(0);
static const uint32_t adsplcr0 = ADRMODE(0) | SPLITSEL(1) | AREA(0x1C) | SWP(0);
static const uint32_t adsplcr1 = SPLITSEL(1) | AREA(0x1C) | SWP(0);
static const uint32_t adsplcr2 = 0;
static const uint32_t adsplcr3 = SPLITSEL(0) | AREA(0x19) | SWP(0);
#else /* (RZG_DRAM_ECC_FULL == 1) */
static const uint32_t fusacr = 0;
static const uint32_t adsplcr0 = 0;
static const uint32_t adsplcr1 = 0;
static const uint32_t adsplcr2 = 0;
static const uint32_t adsplcr3 = 0;
#endif /* (RZG_DRAM_ECC_FULL == 1) */
#endif /* (RCAR_LSI == RZ_G2M || RCAR_LSI == RZ_G2H) */

#if ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL != 0))
/* Write zero-valued octa-byte words */
static void bzero64(uintptr_t start, uint64_t size)
{
	/* start should be aligned on word boundary, size should be multiple of 8 */
	volatile uint64_t *ptr = (volatile uint64_t *)start;
	volatile uint64_t *end = ptr + (size / sizeof(uint64_t));

	while (ptr < end)
		*ptr++ = 0;
}
#endif /* ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL != 0)) */

#if (RCAR_LSI == RZ_G2M || RCAR_LSI == RZ_G2H)
void bl2_ecc_dual_init()
{
	int i, num_area;
	uintptr_t addr[24];
	uint64_t size[24];

	num_area = 0;
	for (i = 0; i < NUM_DFUSACR; i++) {
		if (fusacr & (EFUSASEL(0) | DFUSASEL((1 << i)))) {
			addr[num_area] = DRAM_ADDR_BASE + (i * MB(AREA_SIZE));
			size[num_area] = MB(AREA_SIZE);

			if (num_area > 0 &&
			    (addr[num_area] ==
			     (addr[num_area - 1] + size[num_area - 1])))
				size[num_area - 1] += size[num_area];
			else
				num_area++;
		} else if ((adsplcr0 & SPLITSEL((1 << i))) &&
			   (adsplcr1 & SPLITSEL((1 << i)))) {
			int j;

			for (j = 0 ; j < NUM_EFUSACR; j++) {
				if (!(fusacr & EFUSASEL((1 << j))))
					continue;

				addr[num_area] = DRAM_ADDR_BASE +
						 (i * MB(AREA_SIZE)) +
						 (j * MB(EXTRA_SPLIT_SIZE));
				size[num_area] = MB(EXTRA_SPLIT_SIZE);

				if (num_area > 0 &&
				    (addr[num_area] ==
				     (addr[num_area - 1] + size[num_area - 1])))
					size[num_area - 1] += size[num_area];
				else
					num_area++;
			}
		}
	}

#if (RZG_DRAM_ECC_FULL == 1)
	NOTICE("BL2: DRAM ECC Full Dual Channel Configured:\n");
#else /* (RZG_DRAM_ECC_FULL == 1) */
	NOTICE("BL2: DRAM ECC Partial mode\n");
#endif /* (RZG_DRAM_ECC_FULL == 1) */
	for (i = 0; i < num_area; i++) {
		NOTICE("\tECC from 0x%lx, size = %d MB\n",
		       addr[i] ^ DRAM_XOR_ADDR, (unsigned int)(size[i] >> 20));

#if ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 1))
		/* Erase memory consume too much time, it may cause
		 * overflow watchdog, so that we will reset the watchdog
		 * counter to have enough time.
		 */
		rcar_swdt_init_counter();
		VERBOSE("\t\tClearing ECC area...\n");
		bzero64((addr[i] ^ DRAM_XOR_ADDR), size[i]);
#endif /* ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 1)) */
	}

	mmio_write_32((uintptr_t)((uint32_t *)ADSPLCR0), adsplcr0);
	mmio_write_32((uintptr_t)((uint32_t *)ADSPLCR1), adsplcr1);
	mmio_write_32((uintptr_t)((uint32_t *)ADSPLCR2), adsplcr2);
	mmio_write_32((uintptr_t)((uint32_t *)ADSPLCR3), adsplcr3);
	mmio_write_32((uintptr_t)((uint32_t *)FUSACR),  fusacr);

	for (i = 0; i < num_area; i++) {
		NOTICE("\tFUSA from 0x%lx, size = %d MB\n",
		       addr[i], (unsigned int)(size[i] >> 20));

#if ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 1))
		/* Erase memory consume too much time, it may cause
		 * overflow watchdog, so that we will reset the watchdog
		 * counter to have enough time.
		 */
		rcar_swdt_init_counter();
		VERBOSE("\t\tClearing FUSA area...\n");
		bzero64(addr[i], size[i]);
#endif /* ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 1)) */
	}
}
#endif /* (RCAR_LSI == RZ_G2M || RCAR_LSI == RZ_G2H) */

void bl2_ecc_single_init(const struct rzg2_ecc_conf *conf, int nb_of_conf)
{
	int n, size_shift;
	uintptr_t data = 0x0, ecc = 0x0;
	uint32_t fusa_size = 0;

#if (RZG_DRAM_ECC_FULL == 2)
	NOTICE("BL2: DRAM ECC Full Single Channel Configured:\n");
#else /* (RZG_DRAM_ECC_FULL == 2) */
	NOTICE("BL2: DRAM ECC Partial mode\n");
#endif /* (RZG_DRAM_ECC_FULL == 2) */
	for (n = 0; n < nb_of_conf; n++, conf++) {
		if (conf->fusaareacr & ((uint32_t) 1 << 31)) {
			fusa_size = 1 << ((conf->fusaareacr >> 24) & 0xF);
			data = (uintptr_t)(conf->fusaareacr & 0xFFFFFF) << 12;
			ecc = (uintptr_t)(conf->eccareacr & 0xFFFFFF) << 12;
			/* The ECC size of ECC single follow the ECC bit mode
			 * ECC bit mode is 8, the ECC size equal to DATA size
			 * ECC bit mode is 64, the ECC size is 1/8 of DATA size
			 */
			size_shift = (conf->eccareacr >> 31) * 3;

			NOTICE("\tFUSA from 0x%lx, size = %d MB,\n",
			       data, fusa_size);
			NOTICE("\t ECC from 0x%lx, ECC %d bits mode\n",
			       ecc, (size_shift == 0 ? 8 : 64));

#if ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 2))
			/* Erase memory consume too much time, it may cause
			 * overflow watchdog, so that we will reset the watchdog
			 * counter to have enough time.
			 */
			rcar_swdt_init_counter();

			VERBOSE("\t\tClearing ECC area...\n");
			bzero64(ecc, MB((uint64_t)fusa_size >> size_shift));
#endif /* ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 2)) */
		}

		mmio_write_32((uintptr_t)((uint32_t *)DFUSAAREACR + n),
			      conf->fusaareacr);
		mmio_write_32((uintptr_t)((uint32_t *)DECCAREACR + n),
			      conf->eccareacr);

#if ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 2))
		if (conf->fusaareacr & ((uint32_t) 1 << 31)) {
			/* Erase memory consume too much time, it may cause
			 * overflow watchdog, so that we will reset the watchdog
			 * counter to have enough time.
			 */
			rcar_swdt_init_counter();

			VERBOSE("\t\tClearing FUSA area...\n");
			bzero64(data, MB((uint64_t)fusa_size));
		}
#endif /* ((FUSA_DRAM_CLEAR == 1) && (RZG_DRAM_ECC_FULL == 2)) */
	}
}

#endif /* (RZG_DRAM_ECC == 1) */

void bl2_ecc_init(uint32_t major, uint32_t minor)
{
#if (RZG_DRAM_ECC == 1)
	int nb_of_conf = 0;

#if (RCAR_LSI == RZ_G2E)
	nb_of_conf = ARRAY_SIZE(rzg2_ek874_conf);
	bl2_ecc_single_init(rzg2_ek874_conf, nb_of_conf);
#elif (RCAR_LSI == RZ_G2M)
	bl2_ecc_dual_init();

	if (major == 3)
	{
		nb_of_conf = ARRAY_SIZE(rzg2_hihope_rzg2m_conf);
		bl2_ecc_single_init(rzg2_hihope_rzg2m_conf, nb_of_conf);
	}
#elif (RCAR_LSI == RZ_G2N)
	nb_of_conf = ARRAY_SIZE(rzg2_hihope_rzg2n_conf);
	bl2_ecc_single_init(rzg2_hihope_rzg2n_conf, nb_of_conf);
#elif (RCAR_LSI == RZ_G2H)
	nb_of_conf = ARRAY_SIZE(rzg2_hihope_rzg2h_conf);
	bl2_ecc_dual_init();
	bl2_ecc_single_init(rzg2_hihope_rzg2h_conf, nb_of_conf);
	return;
#else
#error "Don't have ECC initialize routine(unknown)."
#endif
#else  /* RZG_DRAM_ECC == 1 */
	NOTICE("BL2: DRAM don't have ECC configuration\n");
#endif /* RZG_DRAM_ECC == 1 */
}
