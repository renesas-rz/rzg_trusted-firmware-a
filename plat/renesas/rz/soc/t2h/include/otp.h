/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTP_H__
#define __OTP_H__

extern uint32_t otp_read_anti_rb_setting(uint32_t *setting);
extern uint32_t otp_read_anti_rb_version(uint32_t *version);
extern uint32_t otp_read_rotpk_hash(uint32_t *hash, uint32_t hash_size);
extern uint32_t otp_read_decrypt_key(uint32_t key_id, uint32_t *key, uint32_t key_size);

#endif /* __OTP_H__ */
