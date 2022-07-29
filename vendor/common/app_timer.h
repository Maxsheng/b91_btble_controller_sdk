/********************************************************************************************************
 * @file     app_timer.h
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

#ifndef VENDOR_COMMON_APP_TIMER_H_
#define VENDOR_COMMON_APP_TIMER_H_

#include "vendor/common/user_config.h"



/**
 * @brief		timer max number, user can set it in app_config.h
 */
#ifndef MAX_TIMER_NUM
#define MAX_TIMER_NUM								4
#endif



#define		MAINLOOP_ENTRY							0
#define 	CALLBACK_ENTRY							1


/**
 * @brief	callback function for software timer task
 */
typedef int (*blt_timer_callback_t)(void);




typedef struct blt_time_event_t {
	blt_timer_callback_t    cb;
	u32                     t;
	u32                     interval;
} blt_time_event_t;


// timer table management
typedef struct blt_timer_mng_t {
	blt_time_event_t	timer[MAX_TIMER_NUM];
	u8					currentNum;  			//total valid timer number
} blt_timer_mng_t;
extern blt_timer_mng_t	blt_timer;


static inline int app_timer_tick1_exceed_tick2(unsigned int tick1, unsigned int tick2)
{
	return (unsigned int)(tick1 - tick2) < 0x80000000;//cannot exceed 134 seconds.
}


/**
 * @brief		This function is used to Sort the timers according
 * 				to the time of the timed task, so as to trigger the
 * 				timers in turn
 * @param[in]	none
 * @return      none
 */
int		app_timer_sort(void);


/**
 * @brief		Timer tasks are originally ordered. When deleting, it will
 * 				be overwritten forward, so the order will not be destroyed
 * 				and there is no need to reorder
 * @param[in]	index - the index for some software timer task
 * @return      0 - delete fail
 * 				other - delete successfully
 */
int		app_timer_delete_by_index(u8 index);




#endif /* VENDOR_COMMON_APP_TIMER_H_ */
