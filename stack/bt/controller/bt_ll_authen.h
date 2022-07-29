/********************************************************************************************************
 * @file     bt_ll_authen.h
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

#ifndef BT_LL_AUTHEN_H_
#define BT_LL_AUTHEN_H_

enum{
	ST_LEGACY_AUTHEN_IDLE,
	ST_LEGACY_AUTHEN_START,
	ST_LEGACY_AUTHEN_WAIT_LINKKEY_REPLY,
	ST_LEGACY_AUTHEN_SEND_AU_RAND,
	ST_LEGACY_AUTHEN_WAIT_SRES,
	ST_LEGACY_AUTHEN_WAIT_AU_RAND,
	ST_LEGACY_AUTHEN_DONE,
};

enum{
	ST_SECURE_AUTHEN_IDLE,
	ST_SECURE_AUTHEN_START,
	ST_SECURE_AUTHEN_WAIT_LINKKEY_REPLY,
	ST_SECURE_AUTHEN_WAIT_AU_RAND,
	ST_SECURE_AUTHEN_WAIT_SRES,
	ST_SECURE_AUTHEN_DONE,
};

enum{
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_IDLE,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_START,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_MASTER_SETUP_START,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_SLAVE_SETUP_START,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_HCI_LK_REQ_REPLY,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_HCI_LK_REQ_NEG_REPLY,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_LMP_NOT_ACCEPTED,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_LMP_SRES,
	LEGACY_AUTHEN_MAINLOOP_TASK_ID_LMP_AURAND,

};

enum{
	LEGACY_AUTHEN_IRQ_TASK_ID_IDLE,
	LEGACY_AUTHEN_IRQ_TASK_ID_START,

};

typedef int32_t (*btc_legacy_authen_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);

btc_legacy_authen_task_callback_t btc_legacy_authen_mainloop_task_callback;
btc_legacy_authen_task_callback_t btc_legacy_authen_irq_task_callback;


enum{
	SECURE_AUTHEN_MAINLOOP_TASK_ID_IDLE,
	SECURE_AUTHEN_MAINLOOP_TASK_ID_START,

};

enum{
	SECURE_AUTHEN_IRQ_TASK_ID_IDLE,
	SECURE_AUTHEN_IRQ_TASK_ID_START,

};

typedef int32_t (*btc_secure_authen_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);
btc_secure_authen_task_callback_t btc_secure_authen_mainloop_task_callback;
btc_secure_authen_task_callback_t btc_secure_authen_irq_task_callback;

int bt_ll_task_legacy_authen(int Lid,int task_id);
int32_t btc_task_legacy_authen_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);
int32_t btc_task_secure_authen_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);
int bt_ll_task_secure_authen(int Lid,int task_id);
uint8_t hci_lk_req_reply_handler(uint8_t *bdaddr,uint8_t* linkkey);
uint8_t hci_lk_req_neg_reply_handler(uint8_t *bdaddr);
int hci_authen_req_handler(uint16_t con_hdl);
uint8_t  bt_ll_get_sp_debug_mode(void);
extern uint8_t linkkey_view[16];
void hci_send_authentication_complete_event(uint8_t status,uint16_t con_hdl);

void btc_legacy_authen_register_module(void);



#endif /* BT_LL_AUTHEN_H_ */
