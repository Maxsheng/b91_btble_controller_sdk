/********************************************************************************************************
 * @file     app_sw_timer.h
 *
 * @brief    This is the header file for BTBLE SDK
 *
 * @author	 BTBLE GROUP
 * @date         2,2022
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef BLT_SOFT_TIMER_H_
#define BLT_SOFT_TIMER_H_

#include "vendor/common/user_config.h"
#include "app_timer.h"

//user define
#ifndef APP_SOFTWARE_TIMER_ENABLE
#define APP_SOFTWARE_TIMER_ENABLE					0   //enable or disable
#endif





/**
 * @brief		This function is used to register the call back for pm_appWakeupLowPowerCb
 * @param[in]	none
 * @return      none
 */
void 	app_sw_timer_init(void);


/**
 * @brief		This function is used to add new software timer task
 * @param[in]	func - callback function for software timer task
 * @param[in]	interval_us - the interval for software timer task.max interval 134s.
 * @return      0 - timer task is full, add fail
 * 				1 - create successfully
 */
int 	app_sw_timer_add(blt_timer_callback_t func, u32 interval_us);


/**
 * @brief		This function is used to delete software timer tasks
 * @param[in]	func - callback function for software timer task
 * @return      0 - delete fail
 * 				1 - delete successfully
 */
int 	app_sw_timer_delete(blt_timer_callback_t func);


/**
 * @brief		This function is used to manage software timer tasks
 * @param[in]	type - the type for trigger
 * @return      none
 */
void  	app_sw_timer_process(int type);




#endif /* BLT_SOFT_TIMER_H_ */
