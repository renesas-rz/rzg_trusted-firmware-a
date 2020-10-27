#
# Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${RZG_LSI},${RZG_AUTO})
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2N/pfc_init_g2h.c
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2M/pfc_init_g2m.c
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2N/pfc_init_g2n.c
else
  ifeq (${RZG_LSI},${RZG_G2H})
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2H/pfc_init_g2h.c
  endif
  ifeq (${RZG_LSI},${RZG_G2M})
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2M/pfc_init_g2m.c
  endif
  ifeq (${RZG_LSI},${RZG_G2N})
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2N/pfc_init_g2n.c
  endif
  ifeq (${RZG_LSI},${RZG_G2E})
    BL2_SOURCES += drivers/renesas/rzg/pfc/G2E/pfc_init_g2e.c
  endif
endif

BL2_SOURCES += drivers/renesas/rzg/pfc/pfc_init.c
