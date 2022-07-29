/********************************************************************************************************
 * @file     bt_ll_role_switch.h
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

#ifndef _BT_LL_ROLE_SWITCH_H_
#define _BT_LL_ROLE_SWITCH_H_
#include "bt_ll_tcf.h"


typedef struct btc_rsw
{
	// save current tcf to avoid role switch fail
	bt_ll_tick_clkn_fcnt_t tcf;
    /// Switch instant (in 312.5us BT half-slots)
    uint32_t instant;
    /// New connection end timestamp (in 312.5us BT half-slots)
    uint32_t new_con_end_ts;
    /// Slot offset reception TS (for master->slave switch only)
    uint32_t slot_offset_ts;
    /// FHS et half slot (for slave->master switch only)
    uint32_t fhs_et_hs;

    uint32_t clk_off;
    /// FHS et half us (for slave->master switch only)
    uint16_t fhs_et_hus;
    /// TX descriptor pointer (saved from CS)
    uint16_t txdescptr;
    /// Role Switch step
    uint8_t step;
    /// New LT address (slave->master: given by Higher layers | master->slave: received in FHS packet)
    uint8_t new_lt_addr;
    /// Temporary buffer for saving descriptor content during Role Switch
    uint8_t txdesc_save_buf[REG_EM_BT_TXDESC_SIZE];
    /// Temporary buffer for saving CS content during Role Switch
    uint8_t cs_save_buf[REG_EM_BT_CS_SIZE];

    uint8_t setup_rsw_en;

    uint8_t enc_pending;
    uint8_t end_reason;
    uint8_t initiator;
    uint8_t new_role;

    uint32_t timeout_start_ts;
    uint32_t timeout_us;
    uint32_t instant_interval;

    uint8_t not_accept_host_con_req; //decide if accept remote host con req when rsw fails
    uint8_t initiate_att;

}__attribute__ ((__packed__)) __attribute__ ((aligned (4))) btc_rsw_t;

extern btc_rsw_t* env_rsw_ptr;


/// ACL Role Switch steps
enum ACL_RSW_STEP
{
	ACL_RSW_STEP_IDLE,
	ACL_RSW_STEP_START,
    ACL_RSW_STEP_TDD_SWITCH,
    ACL_RSW_STEP_PICONET_SWITCH,
	ACL_RSW_STEP_INSTANT_PASS,
};

/*
 * VARIABLE DECLARATION
*****************************************************************************************
 */
/// Switch environment structure
//extern bt_ll_switch_env_t bt_switch_env;

typedef enum {
    ST_ROLE_SWITCH_IDLE = 0x00,     /// idle state
	ST_ROLE_SWITCH_CHECK_ENCRYPTION,
	ST_ROLE_SWITCH_WAIT_DECRYPTION,

	ST_ROLE_SWITCH_INIT_MASTER_START,
	ST_ROLE_SWITCH_INIT_MASTER_WAIT_SLOT_OFFSET,
	ST_ROLE_SWITCH_INIT_MASTER_WAIT_ACCEPT,

	ST_ROLE_SWITCH_INIT_SLAVE_START,
	ST_ROLE_SWITCH_INIT_SLAVE_WAIT_ACCEPT,

	ST_ROLE_SWITCH_RESP_MASTER_START,
	ST_ROLE_SWITCH_RESP_MASTER_WAIT_SW_REQ,
	ST_ROLE_SWITCH_RESP_MASTER_WAIT_ACCEPT_ACK,

	ST_ROLE_SWITCH_RESP_SLAVE_START,


	ST_ROLE_SWITCH_WAIT_ACCEPT_ACK,
	ST_ROLE_SWITCH_WAIT_COMING,
	ST_ROLE_SWITCH_WAIT_START_SCH,
    ST_ROLE_SWITCH_WAIT_DONE,
	ST_ROLE_SWITCH_CHECK_DECRYPTION,
	ST_ROLE_SWITCH_WAIT_ENCRYPTION,
	ST_ROLE_SWITCH_FINISHED,
} LMP_ROLE_SWITCH_STATE;



/**
 ****************************************************************************************
 * @brief This function allows to read the policy settings of the specified acl cnnection.
 * 
 * 
 * @param[in] con_hdl    specify the acl connection whose policy settings will be
 *                       read out.
 *
 * @param[out] setting   the policy settings.
 *
 * @return CO_ERROR_NO_ERROR if the lmp procedure is started successfully 
 *         otherwise an errorcode is returned.      
 *
 ****************************************************************************************
 */
uint8_t bt_ll_acl_rd_link_policy_settings(uint16_t con_hdl, uint8_t *setting);

/**
 ****************************************************************************************
 * @brief This function allows to write the policy settings of the specified acl cnnection.
 * 
 * 
 * @param[in] bd_addr    con_hdl specify the acl connection whose policy settings will be
 *                       written into.
 *
 * @param[in] setting    the policy settings.
 *
 * @return CO_ERROR_NO_ERROR if the lmp procedure is started successfully 
 *         otherwise an errorcode is returned.      
 *
 ****************************************************************************************
 */
uint8_t bt_ll_acl_wr_link_policy_settings(uint16_t con_hdl, uint16_t setting);


typedef int32_t (*btc_rsw_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);
enum{
	RSW_MAINLOOP_TASK_ID_IDLE,
	RSW_MAINLOOP_TASK_ID_HCI_SWITCH_ROLE,
	RSW_MAINLOOP_TASK_ID_HCI_ACC_CON_SWITCH_ROLE,
	RSW_MAINLOOP_TASK_ID_LMP_SWITCH_ROLE,
	RSW_MAINLOOP_TASK_ID_LMP_SWITCH_REQ_ACK,
	RSW_MAINLOOP_TASK_ID_LMP_SWITCH_REQ_ACC,
	RSW_MAINLOOP_TASK_ID_LMP_SWITCH_REQ_NOT_ACC,
	RSW_MAINLOOP_TASK_ID_LMP_SLOT_OFFSET,
	RSW_MAINLOOP_TASK_ID_ENC2_RSW_END,
	RSW_MAINLOOP_TASK_ID_DEC2_RSW_START,
};

enum{
	RSW_IRQ_TASK_ID_IDLE,
	RSW_IRQ_TASK_ID_SAVE_SETUP_RSW_EN,
	RSW_IRQ_TASK_ID_START_SCH,
	RSW_IRQ_TASK_ID_START_RSW,
	RSW_IRQ_TASK_ID_WAIT_RSW_DONE,
};

btc_rsw_task_callback_t btc_rsw_mainloop_task_callback;
btc_rsw_task_callback_t btc_rsw_irq_task_callback;
void btc_rsw_register_module(btc_rsw_t* env_rsw);

#endif /* _BT_LL_ROLE_SWITCH_H_ */
