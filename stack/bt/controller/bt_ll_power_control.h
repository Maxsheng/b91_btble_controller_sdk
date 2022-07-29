/********************************************************************************************************
 * @file     bt_ll_power_control.h
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

#ifndef _BT_LL_POWER_CONTROL_H_
#define _BT_LL_POWER_CONTROL_H_

#include <stdint.h>
#include "bt_ll.h"

#define RSSI_40dB_THRHLD -40;
#define RSSI_60dB_THRHLD -60;

/// Enumeration of RF modulations
enum rwip_rf_mod
{
    MOD_GFSK  = 0x01,
    MOD_DQPSK = 0x02,
    MOD_8DPSK = 0x03,
};

/// Index of minimum TX power
extern uint8_t txpwr_min;
/// Index of maximum TX power
extern uint8_t txpwr_max;
extern int8_t rssi_high_thr ;
extern int8_t rssi_low_thr  ;

int32_t lmp_send_min_pwr(uint8_t link_id, uint8_t tid);

int32_t lmp_send_max_pwr(uint8_t link_id, uint8_t tid);

int32_t lmp_send_incr_pwr(uint8_t link_id);

int32_t lmp_send_decr_pwr(uint8_t link_id);

int32_t lmp_send_pwr_ctr_req(uint8_t link_id,uint8_t adjust);

int32_t lmp_send_pwr_ctr_res(uint8_t link_id,uint8_t adjust);

/**
 ****************************************************************************************
 * @brief  the power increase function.
 ****************************************************************************************
 */
int32_t bt_ll_incr_pwr(uint8_t link_id);
/**
 ****************************************************************************************
 * @brief  the power decrease function.
 ****************************************************************************************
 */
int32_t bt_ll_decr_pwr(uint8_t link_id);

int8_t rf_rssi_convert (uint8_t rssi_reg);

uint8_t bt_ll_txpwr_dbm_get(uint8_t tx_pwr, uint8_t mod);

uint8_t bt_ll_current_tx_power_get(uint8_t link_id, uint8_t mod);

#endif /* _BT_LL_POWER_CONTROL_H_ */
