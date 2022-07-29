/********************************************************************************************************
 * @file     app_config.h
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

#pragma once

#include <vendor/_proj_btble_controller_/stack_config.h>
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/* TEST MODE:
 * BR: IQ/TP
 * EDR: IQ
 *
 * PA MODE:
 * AUTO(fixed pin)
 * MANUAL(any pin)
 *
 */

#define PA_ENABLE			 				0

#define PA_CTRL_MODE         				0

#if PA_ENABLE
	#if PA_CTRL_MODE //MANUAL MODE
		#define MANUAL_PA_EN          		1
		#define PA_TX_EN_PIN   GPIO_PA6//RF_RFFE_TX_PD7//GPIO_PA6//RF_RFFE_TX_PA6
		#define PA_RX_EN_PIN   GPIO_PA5//RF_RFFE_RX_PD6//GPIO_PA5// RF_RFFE_RX_PA5

	#else//AUTO MODE
		#define MANUAL_PA_EN          		0
	#endif
#endif


#define TP_SWITCH_EN		  				0//1: enable TP modulation for BR; 0- enable IQ modulation for BR/EDR


#if ENABLE_BB_SIGNAL
#define BT_BASEBAND_SIGNAL_DBG_EN  			0
#define BT_BASEBAND_SIGNAL_LOOPBACK_VCD  	1
#endif

#define	MAX_BT_ACL_LINK				(BT_ACL_LINK_NUM+1)

#define	APP_CFG_DUMP_EN				  		1

#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
// #define LED3            GPIO_PB6
#define LED4            GPIO_PB7
#define LED5            GPIO_PD4
#define LED6            GPIO_PD5

// disable iram
#define TMUSIC_ENC_BUFF_RAM_DATA 		1
#define SBC_LOCAL_RAM_ENABLE     		0


#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
