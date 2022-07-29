/********************************************************************************************************
 * @file     bt_dbg.h
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

#ifndef VENDOR_COMMON_BT_DBG_H_
#define VENDOR_COMMON_BT_DBG_H_

#include "vendor/common/user_config.h"


#ifndef BT_BASEBAND_SIGNAL_DBG_EN
#define BT_BASEBAND_SIGNAL_DBG_EN				0
#endif



#ifndef BT_BASEBAND_SIGNAL_LOOPBACK_VCD
#define BT_BASEBAND_SIGNAL_LOOPBACK_VCD			0
#endif




/**
 * @brief		to enable some BT base_band RF signal output to GPIO
 * @param[in]	none
 * @return      none
 */
void bt_dbg_init(void);


/**
 * @brief		BT base_band signal TX_EN/RX_EN/RX_SYNC loop back to VCD log
 * @param[in]	none
 * @return      none
 */
void usb_vcd_init(void);




#endif /* VENDOR_COMMON_BT_DBG_H_ */
