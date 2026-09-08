/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TSIP_LIB_H
#define TSIP_LIB_H

extern int tsip_auth_init(void *shrm, void *heap);
extern int tsip_auth_keys(void *key, unsigned long len);
extern int tsip_auth_image(void *src, unsigned long len, void *dst);

#endif /* TSIP_LIB_H */
