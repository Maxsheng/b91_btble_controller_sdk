/********************************************************************************************************
 * @file     btble.h
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

#ifndef STACK_BT_CONTROLLER_BB_STACK_H_
#define STACK_BT_CONTROLLER_BB_STACK_H_




/**
 * @brief	BT BLE Controller Event Callback
 */
typedef void (*contr_event_callback_t)(u8 e, u8 *p, int n);


/**
 * @brief	BT BLE Controller Event Name
 */
typedef enum{
	CONTR_EVT_PM_GPIO_EARLY_WAKEUP		= 0,
	CONTR_EVT_PM_SLEEP_ENTER,
	CONTR_EVT_PM_SUSPEND_EXIT,
	CONTR_EVT_LE_RX_FIFO_OVERFLOW,

	CONTR_EVT_MAX_PM_NUM,
}contr_evt_t;







/**
 * @brief   main_loop for BTBLE stack
 * @param	none
 * @return	none
 */
void 		btble_sdk_main_loop (void);






/**
 * @brief	Telink defined Controller Event callBack
 * @param[in]	evt - event number, must use element of "blt_ev_flag_t"
 * @param[in]	func - callBack function
 * @return	none
 */
void 		btble_contr_registerControllerEventCallback (contr_evt_t evt, contr_event_callback_t func);










#endif /* STACK_BT_CONTROLLER_BB_STACK_H_ */
