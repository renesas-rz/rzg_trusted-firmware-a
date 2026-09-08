/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_SDDRV_H
#define IO_SDDRV_H

struct io_dev_connector;

int register_io_dev_sddrv(const io_dev_connector_t **connector);

#endif /* IO_SDDRV_H */
