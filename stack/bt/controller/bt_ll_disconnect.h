/********************************************************************************************************
 * @file     bt_ll_disconnect.h
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

#ifndef _BT_LL_DISCONNECT_H_
#define _BT_LL_DISCONNECT_H_

typedef enum {
    ST_DISCONNECT_IDLE = 0x00,     /// idle state
	ST_DISCONNECT_START,
	ST_DISCONNECT_GOING,
	ST_DISCONNECT_DONE,
} LMP_DISCONNECT_STATE;

/**
 ****************************************************************************************
 * @brief This function is the state machine function of the disconnection procedure.
 *
 * @param[in] p_acl      pointer to the acl connection needs to perform disconnection
 *
 * @param[in] Lid        indicates the acl connection needs to perform disconnection
 *
 * @param[in] task_id    the id of the disconnection task
 *
 * @return the msg type the state machine picks from the msg queue is used by the msg
 *         clean function after the msg is processed.
 *
 ****************************************************************************************
 */

int32_t btc_task_disconnection_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

int bt_ll_task_disconnection(int Lid,int task_id);
void bt_ll_disconnection_do_release(uint8_t link_id);
int bt_ll_disconnection_check(uint8_t link_id);
uint8_t bt_ll_acl_disconnection(uint16_t con_hdl,uint8_t reason);
_attribute_retention_code_ void bt_ll_free_lt_addr(uint8_t LtAddr);
int lmp_send_detach_req(uint8_t link_id,uint8_t reason);
_attribute_retention_code_
void hci_send_disconnection_complete_event(unsigned char status,unsigned short connection_handle,unsigned char reason);

#endif /* _BT_LL_DISCONNECT_H_ */
