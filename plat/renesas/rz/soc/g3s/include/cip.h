/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CIP_H
#define CIP_H

void cip_init(void);

void cip_rng(uintptr_t buffer, size_t len);

#endif /* CIP_H */

