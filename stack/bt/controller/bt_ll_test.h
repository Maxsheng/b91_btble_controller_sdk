/********************************************************************************************************
 * @file     bt_ll_test.h
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

#ifndef BT_LL_TEST_H_
#define BT_LL_TEST_H_

/// Test mode action
enum ACL_TEST_MODE_ACTION
{
    TEST_MODE_NO_ACTION,
    TEST_MODE_ENTER,
    TEST_MODE_EXIT,
};

///Test Mode Structure
typedef struct btc_test_mode_params
{
    /// Data length
    uint16_t DataLength;
    /// Test scenario
    uint8_t  TestScenario;
    /// Hopping mode
    uint8_t  HoppingMode;

    /// Transmit frequency
    uint8_t  TxFreq;
    /// Reception frequency
    uint8_t  RxFreq;
    /// Power control test
    uint8_t  PowerControl;
    /// Polling period
    uint8_t  PollPeriod;

    /// Packet type
    uint8_t  PacketType;
    /// Activation state
    bool activated;
}btc_test_mode_params_t;

typedef struct btc_test_mode
{
    /// Test mode parameters (provided by higher layers)
    struct btc_test_mode_params local_params;
    struct btc_test_mode_params remote_params;
    /// Buffer used for sending the test data
    uint16_t em_buf;
    /// Action (enter, update, exit)
    uint8_t action;
}btc_test_mode_t;
btc_test_mode_t* env_test_mode_ptr;
static inline btc_test_mode_t * btc_test_mode_get_env(uint8_t link_id)
{
	return (btc_test_mode_t*)(env_test_mode_ptr? (env_test_mode_ptr+link_id):0);
}
typedef int32_t (*btc_test_mode_set_callback_t)(uint8_t link_id,btc_test_mode_t* p);
typedef int32_t (*btc_test_mode_update_callback_t)(uint8_t link_id);
btc_test_mode_set_callback_t btc_task_test_mode_set_cb;
btc_test_mode_update_callback_t btc_task_test_mode_updata_cb;

#endif /* BT_LL_TEST_H_ */
