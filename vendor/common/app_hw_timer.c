/********************************************************************************************************
 * @file     app_hw_timer.c
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
#include "app_hw_timer.h"


#if (APP_HARDWARE_TIMER_ENABLE)


u32 hw_timer_print_tick = 0;

void 	app_hw_timer_set_capture(u32 stimer_target_tick);



/**
 * @brief		This function is used to add new hardware timer task
 * @param[in]	func - callback function for hardware timer task
 * @param[in]	interval_us - the interval for hardware timer task. max interval 134s.
 * @return      0 - timer task is full, add fail
 * 				1 - create successfully
 */
int app_hw_timer_add(blt_timer_callback_t func, u32 interval_us)
{
	u32 r = irq_disable(); //very important

	if(blt_timer.currentNum < MAX_TIMER_NUM){  //timer table not full

		blt_timer.timer[blt_timer.currentNum].cb = func;
		blt_timer.timer[blt_timer.currentNum].interval = interval_us * SYSTEM_TIMER_TICK_1US;
		blt_timer.timer[blt_timer.currentNum].t = clock_time() + blt_timer.timer[blt_timer.currentNum].interval;
		blt_timer.currentNum ++;

		if(blt_timer.currentNum > 1){
			app_timer_sort();
		}

		app_hw_timer_set_capture(blt_timer.timer[0].t);
	}

	irq_restore(r); //very important

	return  1;
}


/**
 * @brief		This function is used to delete hardware timer task
 * @param[in]	func - callback function for hardware timer task
 * @return      0 - delete fail
 * 				1 - delete successfully
 */
int 	app_hw_timer_delete(blt_timer_callback_t func)
{

	u32 r = irq_disable(); //very important

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

			break;
		}
	}

	irq_restore(r); //very important

	return 0;
}


/**
 * @brief		This function is used to register the call back for pm_appWakeupLowPowerCb
 * @param[in]   type - timer0/timer1.
 * @return      none
 */
void 	app_hw_timer_init(timer_type_e type)
{
	if(sys_clk.pclk != 16 && sys_clk.pclk != 24){
		//now only support PCLK is 16M or 24M
		DBG_CHN6_TOGGLE;
		return;
	}

	hw_timer_ctl.en = 1;
	hw_timer_ctl.type = type;

	if(type == TIMER0){
		plic_interrupt_enable(IRQ4_TIMER0);
		plic_set_priority(IRQ4_TIMER0, 1);
		reg_tmr_ctrl0 |= FLD_TMR0_NOWRAP;
	}
	else{
		plic_interrupt_enable(IRQ3_TIMER1);
		plic_set_priority(IRQ3_TIMER1, 1);
		reg_tmr_ctrl0 |= FLD_TMR1_NOWRAP;
	}


	//	reg_system_tick = 0xF8000000;  //debug

	timer_set_mode(type, TIMER_MODE_SYSCLK);
	timer_set_cap_tick(type, BIT(31)); //set to a very far timing in case error trigger
	timer_set_init_tick(type, 0);
	timer_start(type);

	btble_pm_registerAppWakeupLowPowerCb(app_hw_timer_process);
}


/**
 * @brief		This function is used to set timer capture value. note must be ram code.
 * @param[in]   type - timer0/timer1.
 * @return      none
 */
_attribute_ram_code_sec_
void app_hw_timer_set_capture(u32 stimer_target_tick)
{
	timer_set_init_tick(hw_timer_ctl.type, 0);
	u32 stimer_tick_now = clock_time();

	if( app_timer_tick1_exceed_tick2(stimer_tick_now + 10*SYSTEM_TIMER_TICK_1US, stimer_target_tick) ){
		timer_set_cap_tick(hw_timer_ctl.type, 0 + 15 * sys_clk.pclk);
		stimer_target_tick = stimer_tick_now + 15*SYSTEM_TIMER_TICK_1US;
	}
	else{
		u32 cap_tick = (u32)(stimer_target_tick - stimer_tick_now); //default PCLK 16M
		if(sys_clk.pclk == 24){  //PCLK is 24M
			//cap_tick = (cap_tick*3)>>1;//for cap_tick=134s, 'u32' will overflow.
			cap_tick = (cap_tick>>1)*3;//if first /2,then *3,loss of accuracy at most one tick, that can be ignored.
		}

		timer_set_cap_tick(hw_timer_ctl.type, cap_tick);
	}

	btble_pm_setAppWakeupLowPower(stimer_target_tick,  1);
}


/**
 * @brief		This function is used to judge whether the timer reach the capture tick,
 *              if yes, and run relevant callback,then according to return value of callback function
 *              to set the timer capture tick or delete timer task(return -1).
 *              this function will be called in timer IRQ and the time wakeup from suspend by timer capture tick.
 * @param[in]   type - timer0/timer1.
 * @return      none
 */
_attribute_ram_code_sec_
void  	app_hw_timer_process(int type)
{
	if(blt_timer.currentNum)
	{
		int change_flg = 0;

		for(int i=0; i<blt_timer.currentNum; i++){
			u32 now = clock_time();
			if(app_timer_tick1_exceed_tick2(now + 64*SYSTEM_TIMER_TICK_1US, blt_timer.timer[i].t) ){

				if(blt_timer.timer[i].cb != NULL){

					change_flg = 1;
					int update_use_now = app_timer_tick1_exceed_tick2(now, blt_timer.timer[i].t + 1*SYSTEM_TIMER_TICK_1MS);
					int result = blt_timer.timer[i].cb();

					if(result < 0){
						app_timer_delete_by_index(i);
					}
					else{ // result > 0 || result == 0
						if(result > 0){
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
			else{
				break;
			}
		}

		if(blt_timer.currentNum ){
			if(change_flg){
				app_timer_sort();
			}

			app_hw_timer_set_capture(blt_timer.timer[0].t);
		}

	}
	else{
		btble_pm_setAppWakeupLowPower(0, 0);
	}
}


#endif  //end of  APP_HARDWARE_TIMER_ENABLE
