/********************************************************************************************************
 * @file     bt_ll_packet_type.h
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

#ifndef _BT_LL_PACKETTYPE_H_
#define _BT_LL_PACKETTYPE_H_

#include <stdint.h>
#include "bt_ll.h"
enum{
	ST_PACKET_TYPE_IDLE=0,
	ST_PACKET_TYPE_START,
	ST_PACKET_TYPE_REQ_RETRY,
	ST_PACKET_TYPE_WAIT_CFM,
	ST_PACKET_TYPE_DONE = ST_PACKET_TYPE_IDLE,
};


int32_t btc_task_packet_type_mainloop(uint8_t Lid,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

int bt_ll_task_packet_type(int Lid, int task_id);

int lmp_send_max_slot(int Lid, unsigned char max_slot);

int lmp_send_cmd_max_slot_req(int Lid, unsigned char max_slot);
int lmp_send_cmd_ptt_req(int Lid, unsigned char ptt);
void bt_ll_change_pkt_type_retry(uint8_t link_id);
void change_pkt_type_cont(uint8_t link_id, uint8_t status);
void bt_ll_max_slot_mgt(uint8_t link_id, uint8_t max_slot);
uint8_t bt_ll_packet_table(uint8_t link_id);
void bt_ll_allowed_tx_packet_types_set(uint8_t link_id, uint16_t packet_types);
uint16_t bt_ll_compute_packet_type(uint16_t Preferredpackettype, uint16_t currentpackettype, bool KeepCurrent);
uint8_t bt_ll_max_slot(uint16_t packettype);
void bt_ll_suppress_acl_packet(uint16_t *reqpackettype, uint8_t slot);
uint16_t bt_ll_update_acl_packet_type(uint16_t packettype, uint8_t  remotecapability);
uint16_t bt_ll_update_acl_edr_packet_type(uint16_t packettype,
                           uint8_t remotecapability3,
                           uint8_t remotecapability4,
                           uint8_t remotecapability5);


#endif /* _BT_LL_PACKETTYPE_H_ */
