/********************************************************************************************************
 * @file     bt_ll_name.h
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

#ifndef _BT_LL_NAME_H_
#define _BT_LL_NAME_H_

enum{
	ST_REM_NAME_IDLE=0,
	ST_REM_NAME_START,
	ST_REM_NAME_WAIT_FEA_RES,
	ST_REM_NAME_WAIT_FEA_EXT_RES,
	ST_REM_NAME_WAIT_NAME_RES,
	ST_REM_NAME_DONE,
};

extern uint8_t remote_name_connect_req;
void hci_send_remote_name_request_complete_event(unsigned char status, unsigned char* bd_addr, unsigned char* name);

uint8_t  bt_ll_acl_remote_name_req(uint8_t *bd_addr,uint8_t pscan_rep_mode, uint16_t clockoffset);
uint8_t hci_rd_rem_ver_info_handler(uint16_t con_hdl);
int bt_ll_task_remote_name(int Lid, int task_id);
uint8_t  bt_ll_acl_remote_name_cancel(uint8_t *bd_addr);
void bt_ll_page_end(uint8_t reason);

int lmp_send_remote_name_req(int Lid);
int lmp_send_version_req(uint8_t link_id,uint8_t version,uint16_t comp_id,uint16_t sub_version);
int lmp_send_version_res(uint8_t link_id,uint8_t version,uint16_t comp_id,uint16_t sub_version);
int lmp_send_name_req(uint8_t link_id,uint8_t name_offset);

uint8_t bt_ll_eir_set(uint8_t UseFec, uint8_t ExtInqRes[]);
uint8_t bt_ll_eir_set_name(uint8_t UseFec, char *name);

uint8_t bt_ll_eir_data_set(uint8_t UseFec, char *name, uint16_t *uuid_16, uint8_t num_16, uint8_t *uuid128, uint8_t num_128);
uint8_t bt_ll_eir_get(uint8_t *fec_req, uint8_t *eir_ptr);
char *bt_ll_get_local_name(void);
void bt_ll_set_local_name(char *name);
int32_t btc_task_rem_name_req_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

#endif /* _BT_LL_NAME_H_ */
