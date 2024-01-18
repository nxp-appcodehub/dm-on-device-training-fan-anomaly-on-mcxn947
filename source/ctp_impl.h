/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef CTP_IMPL_H
#define CTP_IMPL_H

#include "gt911_ctp.h"

void        ctp_impl_init(void);
gt911_ret_t ctp_impl_xfer(void *handle, gt911_i2c_xfer_t *xfer);

#endif