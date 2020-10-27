/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_RZG_H
#define IO_RZG_H

int32_t rzg_register_io_dev(const io_dev_connector_t **dev_con);
int32_t rzg_get_certificate(const int32_t name, uint32_t *cert);
void rzg_read_certificate(uint64_t cert, uint32_t *size, uintptr_t *dest);

#endif /* IO_RZG_H */
