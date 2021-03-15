/**
 * Copyright (2017) Baidu Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * File: lightduer_bind_device.h
 * Auth: Chen Xihao (chenxihao@baidu.com)
 * Desc: Support WeChat Subscription to bind device
 */

#ifndef BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_BIND_DEVICE_H
#define BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_BIND_DEVICE_H

#include "lightduer_types.h"
#include "lightduer_dcs_router.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * DESC:
 * Start the task to bind device.
 *
 * PARAM:
 * @param[in] uuid: the uuid of device
 * @param[in] bind_token: the bind token of device
 * @param[in] lifecycle: the lifecycle of task in seconds, at least 60s
 *
 * @RETURN: success return DUER_OK, failed return DUER_ERR_FAILED.
 */
duer_status_t duer_start_bind_device_task(const char *uuid,
        const char *bind_token, size_t lifecycle);

/**
 * DESC:
 * Stop the task that bind device.
 *
 * PARAM: none
 *
 * @RETURN: success return DUER_OK, failed return DUER_ERR_FAILED.
 */
duer_status_t duer_stop_bind_device_task(void);

/**
 * DESC:
 * Send utoken to cloud to bind device.
 *
 * PARAM:
 * @param[in] utoken: the token passed from App
 * @param[in] callback: the callback function to receive the bind result
 *
 * @RETURN: success return DUER_OK, failed return other.
 */
duer_status_t duer_bind_device_use_utoken(const char *utoken, dcs_directive_handler callback);

#ifdef __cplusplus
}
#endif

#endif /* BAIDU_DUER_LIGHTDUER_INCLUDE_LIGHTDUER_BIND_DEVICE_H */
