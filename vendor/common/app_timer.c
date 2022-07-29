/********************************************************************************************************
 * @file     app_timer.c
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
#include "app_timer.h"

#if (APP_SOFTWARE_TIMER_ENABLE || APP_HARDWARE_TIMER_ENABLE)


_attribute_ble_data_retention_	blt_timer_mng_t	blt_timer;


/**
 * @brief		This function is used to Sort the timers according
 * 				to the time of the timed task, so as to trigger the
 * 				timers in turn
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_sec_
int  app_timer_sort(void)
{

	#if (APP_TIMER_DBG_EN) //debug
		if(blt_timer.currentNum ==0 || blt_timer.currentNum > MAX_TIMER_NUM){
			STACK_ERR_DEBUG(APP_TIMER_DBG_EN, 0x88030000 | blt_timer.currentNum);
		}
	#endif


	//BubbleSort
	int n = blt_timer.currentNum;
	u8 temp[sizeof(blt_time_event_t)];

	for(int i=0;i<n-1;i++)
	{
		for(int j=0;j<n-i-1;j++)
		{
			if(app_timer_tick1_exceed_tick2(blt_timer.timer[j].t, blt_timer.timer[j+1].t))
			{
				//swap
				//attention: here use "tmemcpy4" to speed up memory copy, BTBLE team evaluated OK
				tmemcpy4(temp, &blt_timer.timer[j], sizeof(blt_time_event_t));
				tmemcpy4(&blt_timer.timer[j], &blt_timer.timer[j+1], sizeof(blt_time_event_t));
				tmemcpy4(&blt_timer.timer[j+1], temp, sizeof(blt_time_event_t));
			}
		}
	}


	return 1;
}


/**
 * @brief		Timer tasks are originally ordered. When deleting, it will
 * 				be overwritten forward, so the order will not be destroyed
 * 				and there is no need to reorder
 * @param[in]	index - the index for some software timer task
 * @return      0 - delete fail
 * 				other - delete successfully
 */
int  app_timer_delete_by_index(u8 index)
{
	if(index >= blt_timer.currentNum){
		return 0;
	}


	for(int i=index; i<blt_timer.currentNum - 1; i++){
		//attention: here use "tmemcpy4" to speed up memory copy, BTBLE team evaluated OK
		tmemcpy4(&blt_timer.timer[i], &blt_timer.timer[i+1], sizeof(blt_time_event_t));
	}

	blt_timer.currentNum --;

	return 0;
}



#endif //end of (APP_SOFTWARE_TIMER_ENABLE || APP_HARDWARE_TIMER_ENABLE)
