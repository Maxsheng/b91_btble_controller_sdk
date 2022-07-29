/********************************************************************************************************
 * @file     sys_timer.c
 *
 * @brief    This is the source file for BTBLE SDK
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
#include "sys_timer.h"
#include "../drivers/B91/stimer.h"
#include "../stack/stack.h"

unsigned int sys_timer_mask = 0;

sys_timer_t sys_timer_pool[SYS_TIMER_NUM_MAX] = {0};

unsigned int sys_timer_new(unsigned int microsecond, void (*timer_func)(void *), void *arg, int times)
{
	unsigned int i;
    if (!timer_func || times <= 0)
    {
        return 0;
    }
    
	for (i = 0; i < SYS_TIMER_NUM_MAX; i++) {
		if (sys_timer_pool[i].used == 0) {
			break;
		}
	}

	if (i >= SYS_TIMER_NUM_MAX) {
		return 0;
	}

	sys_timer_pool[i].run_enable = 1;
	sys_timer_pool[i].count_init = stimer_get_tick();
	sys_timer_pool[i].interval   = microsecond;
	sys_timer_pool[i].times_init = times;
	sys_timer_pool[i].times_left = sys_timer_pool[i].times_init;
	sys_timer_pool[i].timer_func = timer_func;
	sys_timer_pool[i].func_arg	 = arg;

	sys_timer_pool[i].used       = 1;

    sys_timer_mask |= (1 << i);
    
	return (i + 1);
}

void sys_timer_stop(unsigned int timer_handle)
{
    if (!timer_handle || timer_handle > SYS_TIMER_NUM_MAX)
    {
        return;
    }

    sys_timer_pool[timer_handle - 1].run_enable = 0;
    
    sys_timer_mask &= ~(1 << (timer_handle - 1));
}

#if 0
void sys_timer_resume(unsigned int timer_handle)
{
    if (!timer_handle || timer_handle > SYS_TIMER_NUM_MAX)
    {
        return;
    }
    
    if (0 == sys_timer_pool[timer_handle - 1].run_enable)
        sys_timer_restart(timer_handle);
}
#endif 

void sys_timer_del(unsigned int timer_handle)
{
    if (!timer_handle || timer_handle > SYS_TIMER_NUM_MAX)
    {
        return;
    }
    
    sys_timer_pool[timer_handle - 1].used = 0;
    sys_timer_mask &= ~(1 << (timer_handle - 1));
}

void sys_timer_restart(unsigned int timer_handle)
{    
    if (!timer_handle || timer_handle > SYS_TIMER_NUM_MAX)
    {
        return;
    }
    
	if (sys_timer_pool[timer_handle - 1].used == 0) {
		my_dump_str_data (DUMP_ACL_MSG, "sys_timer_restart used err", ((uint8_t *)&timer_handle), 4);
		return;
	}

	sys_timer_pool[timer_handle - 1].count_init = stimer_get_tick();
	sys_timer_pool[timer_handle - 1].times_left = sys_timer_pool[timer_handle - 1].times_init;
	sys_timer_pool[timer_handle - 1].run_enable = 1;
    
    sys_timer_mask |= (1 << (timer_handle - 1));
}

void sys_timer_task(void)
{
    for (unsigned int i = 0; i < SYS_TIMER_NUM_MAX; i++)
    {
        if (sys_timer_mask & (1 << i))
        {
            if (clock_time_exceed (sys_timer_pool[i].count_init, sys_timer_pool[i].interval)
             && sys_timer_pool[i].timer_func != NULL
            )
            {
                sys_timer_pool[i].timer_func(sys_timer_pool[i].func_arg);
                
                sys_timer_pool[i].times_left--;
                
                if (0 == sys_timer_pool[i].times_left)
                {
                    sys_timer_stop(i + 1);                    
                }
                else 
                {                    
                    sys_timer_pool[i].count_init = stimer_get_tick();
                }
            }
        }
    }
}


#endif
