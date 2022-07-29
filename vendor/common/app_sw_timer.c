/********************************************************************************************************
 * @file     app_sw_timer.c
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

#include "tl_common.h"
#include "drivers.h"
#include "app_sw_timer.h"


#if (APP_SOFTWARE_TIMER_ENABLE)


/**
 * @brief		This function is used to add new software timer task
 * @param[in]	func - callback function for software timer task
 * @param[in]	interval_us - the interval for software timer task.max interval 134s.
 * @return      0 - timer task is full, add fail
 * 				1 - create successfully
 */
int app_sw_timer_add(blt_timer_callback_t func, u32 interval_us)
{
	u32 now = clock_time();

	if(blt_timer.currentNum >= MAX_TIMER_NUM){  //timer full
		return 	0;
	}
	else{
		blt_timer.timer[blt_timer.currentNum].cb = func;
		blt_timer.timer[blt_timer.currentNum].interval = interval_us * SYSTEM_TIMER_TICK_1US;
		blt_timer.timer[blt_timer.currentNum].t = now + blt_timer.timer[blt_timer.currentNum].interval;
		blt_timer.currentNum ++;

		app_timer_sort();

		btble_pm_setAppWakeupLowPower(blt_timer.timer[0].t,  1);

		return  1;
	}
}


/**
 * @brief		This function is used to delete software timer task
 * @param[in]	func - callback function for software timer task
 * @return      0 - delete fail
 * 				1 - delete successfully
 */
int 	app_sw_timer_delete(blt_timer_callback_t func)
{

	for(int i=0; i<blt_timer.currentNum; i++){
		if(blt_timer.timer[i].cb == func){
			app_timer_delete_by_index(i);

			if(blt_timer.currentNum){ //timer table not empty
				if(i == 0){ //The most recent timer is deleted, timer needs to be updated
					btble_pm_setAppWakeupLowPower(blt_timer.timer[0].t,  1);
				}
			}
			else{
				btble_pm_setAppWakeupLowPower(0, 0);
			}

			return 1;
		}
	}

	return 0;
}


/**
 * @brief		This function is used to judge whether the timer reach the capture tick,
 *              if yes, and run relevant callback,then according to return value of callback function
 *              to set the timer capture tick or delete timer task(return -1).
 *              this function will be called in main_loop and the time wakeup from suspend by timer capture tick.
 * @param[in]   type - the type for trigger. mailloop trigger or exit suspend trigger.
 * @return      none
 */
void  	app_sw_timer_process(int type)
{
	if(!blt_timer.currentNum){
		return;
	}

	int change_flg = 0;
	for(int i=0; i<blt_timer.currentNum; i++){
		u32 now = clock_time();
		if(app_timer_tick1_exceed_tick2(now + 128*SYSTEM_TIMER_TICK_1US, blt_timer.timer[i].t) ){//timer trigger

			if(blt_timer.timer[i].cb != NULL){

				int update_use_now = app_timer_tick1_exceed_tick2(now, blt_timer.timer[i].t + 1*SYSTEM_TIMER_TICK_1MS);
				int result = blt_timer.timer[i].cb();

				if(result < 0){
					app_timer_delete_by_index(i);
				}
				else{ // ==0 || >0
					change_flg = 1;
					if(result > 0){  //set new timer interval
						blt_timer.timer[i].interval = result * SYSTEM_TIMER_TICK_1US;
					}

					if(update_use_now){
						blt_timer.timer[i].t = now + blt_timer.timer[i].interval;
					}
					else{
						blt_timer.timer[i].t += blt_timer.timer[i].interval;
					}

				}

			}
		}
	}


	if(blt_timer.currentNum ){ //timer table not empty
		if(change_flg){
			app_timer_sort();
		}

		btble_pm_setAppWakeupLowPower(blt_timer.timer[0].t,  1);
	}
	else{
		btble_pm_setAppWakeupLowPower(0, 0);  //disable
	}
}


/**
 * @brief		This function is used to register the call back for pm_appWakeupLowPowerCb
 * @param[in]	none
 * @return      none
 */
void 	app_sw_timer_init(void)
{
	btble_pm_registerAppWakeupLowPowerCb(app_sw_timer_process);
}


#endif  //end of  BLT_SOFTWARE_TIMER_ENABLE

