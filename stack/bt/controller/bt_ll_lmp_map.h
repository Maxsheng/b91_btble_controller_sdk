/********************************************************************************************************
 * @file     bt_ll_lmp_map.h
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

#ifndef BT_LL_LMP_MAP_H_
#define BT_LL_LMP_MAP_H_

#include "bt_ll_lmp.h"
/*
 * STRUCTURES
 ****************************************************************************************
 */
enum{
	BTC_LMP_MSG_ID_IDLE,
	BTC_LMP_MSG_ID_RX,
	BTC_LMP_MSG_ID_TX_ACK,
};

///LMP msg
typedef struct LMP_MSG {
    ///message identifier
	uint8_t  msg_id;
    ///Link identifier
    uint8_t  link_id;
    ///length of the LMP PDU
    uint8_t  len;
    ///reserved for future use
    uint8_t  rsvd;
    ///buffer of the LMP PDU
    uint8_t*  pdu;
} lmp_msg_t;

typedef int32_t (*func_lmp_command_t) (uint8_t link_id, uint8_t* p , uint8_t len);

typedef int32_t (*lmp_pdu_func_t)(uint8_t link_id, uint8_t *p, uint8_t len);

my_fifo_t btc_lmp_msg_fifo;
my_fifo_t btc_lmp_msg_fifo_dft;

void btc_lmp_msg_fifo_init(uint8_t fifo_num, uint8_t fifo_size,uint8_t* lmp_msg_fifo);

static inline void btc_lmp_msg_fifo_reset(void)
{
	tmemcpy(&btc_lmp_msg_fifo,&btc_lmp_msg_fifo_dft,sizeof(my_fifo_t));
}

static inline int32_t btc_lmp_msg_is_pending(void)
{
	return (btc_lmp_msg_fifo.rptr != btc_lmp_msg_fifo.wptr);
}

typedef void(*lmp_msg_mainloop_func_t)(void);

lmp_msg_mainloop_func_t btc_lmp_msg_mainloop_cb;

int32_t lmp_accept_opcode_req(uint8_t link_id,uint8_t opcode, uint8_t errorcode);
int32_t lmp_accept_ext_opcode_req(uint8_t link_id,uint8_t esc, uint8_t code, uint8_t errorcode);
int32_t lmp_send_timing_accuracy_req(uint8_t link_id);
int32_t lmp_send_timing_accuracy_res(uint8_t link_id);
int32_t lmp_send_auto_rate(uint8_t link_id);
int32_t lmp_send_preferred_rate(uint8_t link_id, preferred_rate_t data_rate);

void bt_ll_send_lmp(uint8_t link_id, void *param, uint8_t param_len);

int32_t btc_lmp_command_exec(uint8_t link_id, uint8_t *p, uint8_t len);
int32_t btc_lmp_tx_cfm_handler(uint8_t link_id,uint8_t* pdu,uint8_t len);

void btc_mem_init(void);

#endif /* BT_LL_LMP_MAP_H_ */
