/********************************************************************************************************
 * @file     bt_acl_master.h
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

#ifndef BT_ACL_MASTER_H_
#define BT_ACL_MASTER_H_

#include <stdint.h>

typedef struct btc_stack_init{

	uint8_t  inq_en;
	uint8_t  inq_mode;
	uint8_t  inqscan_type;
	uint8_t  inqscan_use_fec;

	uint8_t  inq_lap[3];
	uint8_t  max_link_num;

	uint16_t inqscan_inv;
	uint16_t inqscan_win;

	uint32_t page_to;
	uint32_t ext_page_to;

	uint16_t pagescan_inv;
	uint16_t pagescan_win;

	uint16_t sniff_inv_max;
	uint16_t sniff_inv_min;

	uint32_t classofdevice;

	uint8_t  local_bdaddr[6];
	uint8_t  pagescan_type;
	uint8_t  pagescan_rep_mode;

	uint8_t  local_name[32];
	uint8_t  scan_en;
	uint8_t  page_en;
}btc_stack_init_t;

void btc_stack_init(btc_stack_init_t * stack_init);


extern uint8_t bt_ll_alloc_lt_addr(void);
extern void bt_ll_free_lt_addr(uint8_t LtAddr);

void btc_bb_reset_core(void);
void btc_bb_clk_init(void);
void btc_bb_radio_init(void);
void btc_bb_em_init(void);

#endif /* BT_ACL_MASTER_H_ */
