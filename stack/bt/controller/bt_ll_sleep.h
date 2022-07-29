/********************************************************************************************************
 * @file     bt_ll_sleep.h
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

#ifndef _BT_LL_SLEEP_H_
#define _BT_LL_SLEEP_H_

/// Definition of the bits preventing the system from sleeping
enum prevent_sleep
{
    /// Flag indicating that the wake up process is ongoing
    WAKE_UP_ONGOING                 = 0x0001,
    /// Flag indicating that an TX transfer is ongoing on Transport Layer
    TL_TX_ONGOING                   = 0x0002,
    /// Flag indicating that an RX transfer is ongoing on Transport Layer
    TL_RX_ONGOING                   = 0x0004,
    /// Flag indicating HCI timeout is ongoing
    AHI_TIMEOUT                     = 0x0008,
    /// Flag indicating that an encryption is ongoing
    CRYPT_ONGOING                   = 0x0010,
    /// Flag indicating that controller shall not sleep due to not CSB LPO_Allowed
    CSB_NOT_LPO_ALLOWED             = 0x0040,
    /// Flag indicating the MWS/WLAN Event Generator is in operation
    MWS_WLAN_EVENT_GENERATOR_ACTIVE = 0x0080,
    /// Flag to indicate that platform does not support deep sleep
    PLF_DEEP_SLEEP_DISABLED         = 0x0100,
    /// Flag to indicate that a baseband frame is ongoing
    BB_FRAME_ONGOING                = 0x0200,
    /// Flag to indicate that BLE Hopping computation on-going
    HOP_CALC_ONGOING                = 0x0400,
    /// Flag to indicate that BT is in active mode (ACL, SCO)
    BT_ACTIVE_MODE                  = 0x0800,
    /// Flag to indicate that BT is allowed to step into deep sleep mode
    PLF_DEEP_SLEEP_ALLOWED          = 0x1000,
};

/// RWIP Environment structure
typedef struct bt_ll_sleep_env
{
    #if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Half slot target timer (in half slots)
    uint32_t timer_hs_target;
    /// Half us target timer (in half us)
    uint32_t timer_hus_target;
    #endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// 10 ms target timer (in half slots)
    uint32_t timer_10ms_target;
    #if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Contains sleep duration accumulated timing error (32kHz: 1/2 half us | 32.768kHz: 1/256 half-us)
    uint32_t sleep_acc_error;
    /// Power_up delay (in LP clock cycle unit, depends on Low power clock frequency)
    uint32_t lp_cycle_wakeup_delay;
    /// Duration of sleep and wake-up algorithm (depends on CPU speed) expressed in half us.
    uint16_t sleep_algo_dur;
    #endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// Prevent sleep bit field
    uint16_t prevent_sleep;
    #if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
    /// External wake-up support
    bool     ext_wakeup_enable;
    #endif // (BLE_EMB_PRESENT || BT_EMB_PRESENT)
}bt_ll_sleep_env_t;


/*
 * GLOBAL DEFINITIONS
 ****************************************************************************************
 */

/// RW SW environment
extern bt_ll_sleep_env_t sleep_env;

void bt_ll_prevent_sleep_set(uint16_t prv_slp_bit);

void bt_ll_prevent_sleep_clear(uint16_t prv_slp_bit);
#endif /* _BT_LL_SLEEP_H_ */
