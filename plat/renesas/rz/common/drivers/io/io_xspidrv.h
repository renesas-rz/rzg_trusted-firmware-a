/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_XSPIDRV_H
#define IO_XSPIDRV_H

struct io_dev_connector;

int register_io_dev_xspidrv(const struct io_dev_connector **dev_con);

#endif /* IO_XSPIDRV_H */
