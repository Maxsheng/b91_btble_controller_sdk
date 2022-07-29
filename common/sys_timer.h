/********************************************************************************************************
 * @file     sys_timer.h
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

#if 0
#ifndef _SYS_TIMER_H_
#define _SYS_TIMER_H_

#ifndef	SYS_TIMER_NUM_MAX
#define  SYS_TIMER_NUM_MAX    24
#endif

extern unsigned int sys_timer_mask;

#define sys_timer_busy()    (sys_timer_mask)

typedef struct _sys_timer {
	unsigned char   used;
	unsigned char   run_enable;             /* Flag indicating whether timer is enabled(1)        */
	int             times_init;
	int             times_left;
	unsigned int    count_init;             /* tick count    */
	unsigned int    interval;               /* us            */
	void    (*timer_func)(void *);          /* Function to execute when timer times out           */
	void     *func_arg;                     /* Arguments supplied to user defined function        */
} sys_timer_t;

unsigned int sys_timer_new(unsigned int microsecond, void (*timer_func)(void *), void *arg, int times);
void sys_timer_stop(unsigned int timer_handle);
//void sys_timer_resume(unsigned int timer_handle);
void sys_timer_del(unsigned int timer_handle);
void sys_timer_restart(unsigned int timer_handle);

void sys_timer_task(void);


#endif

#endif
