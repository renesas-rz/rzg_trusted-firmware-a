#
# Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${RZG_LSI},${RZG_AUTO})
# G2E not available for LSI_AUTO
    BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v11.c
    BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v30.c
    BL2_SOURCES += drivers/renesas/rzg/qos/G2N/qos_init_g2n.c
    BL2_SOURCES += drivers/renesas/rzg/qos/G2H/qos_init_g2h.c
else
  ifdef RZG_LSI_CUT_COMPAT
    ifeq (${RZG_LSI},${RZG_G2H})
      BL2_SOURCES += drivers/renesas/rzg/qos/G2H/qos_init_g2h.c
    endif
    ifeq (${RZG_LSI},${RZG_G2M})
      BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v11.c
      BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v30.c
    endif
    ifeq (${RZG_LSI},${RZG_G2N})
      BL2_SOURCES += drivers/renesas/rzg/qos/G2N/qos_init_g2n.c
    endif
    ifeq (${RZG_LSI},${RZG_G2E})
      BL2_SOURCES += drivers/renesas/rzg/qos/G2E/qos_init_g2e.c
    endif
  else
    ifeq (${RZG_LSI},${RZG_G2H})
       BL2_SOURCES += drivers/renesas/rzg/qos/G2H/qos_init_g2h.c
    endif
    ifeq (${RZG_LSI},${RZG_G2M})
      ifeq (${LSI_CUT},11)
       BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v11.c
      else ifeq (${LSI_CUT},13)
       BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v11.c
      else ifeq (${LSI_CUT},30)
       BL2_SOURCES += drivers/renesas/rzg/qos/G2M/qos_init_g2m_v30.c
      endif
    endif
    ifeq (${RZG_LSI},${RZG_G2N})
       BL2_SOURCES += drivers/renesas/rzg/qos/G2N/qos_init_g2n.c
    endif
    ifeq (${RZG_LSI},${RZG_G2E})
       BL2_SOURCES += drivers/renesas/rzg/qos/G2E/qos_init_g2e.c
    endif
  endif
endif

BL2_SOURCES += drivers/renesas/rzg/qos/qos_init.c
