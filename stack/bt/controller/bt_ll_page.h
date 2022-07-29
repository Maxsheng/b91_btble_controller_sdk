/********************************************************************************************************
 * @file     bt_ll_page.h
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

#ifndef _BT_LL_PAGE_H_
#define _BT_LL_PAGE_H_
#include <stdint.h>
#include "bt_ll_bch.h"

enum
{
    /// idle state
	ST_HOST_CON_IDLE = 0x00,
    ST_HOST_CON_VERSION_REQ,           // <
    ST_HOST_CON_VERSION_RSP,           // <
    ST_HOST_CON_FEAT_REQ,			// <
	ST_HOST_CON_FEAT_RSP,			// <
	ST_HOST_CON_EXT_FEAT_REQ,
	ST_HOST_CON_EXT_FEAT_RSP,
	ST_HOST_CON_NAME_REQ,				// >
	ST_HOST_CON_NAME_RSP,
	ST_HOST_CON_CON_REQ,
	ST_HOST_CON_WAIT_ACCEPT,
	ST_HOST_CON_DONE,
	ST_HOST_QOS_REQ,
	ST_HOST_QOS_RSP,
    ST_HOST_CON_WAIT_SETUP_COMPLETE=0x10,
};

enum{
	ST_PAGE_IDLE,
	ST_PAGE_RX_2ND_ID_RSP,
	ST_PAGE_RX_1ST_PACKET_RSP,
};

enum{
	ST_PAGE_STATE_IDLE,
	ST_PAGE_STATE_GOING,
	ST_PAGE_STATE_DONE,
};

/// LD PAGE environment structure
typedef struct ld_page_env_tag
{
    /// Page Timeout (in slots) offered by HCI command
    uint32_t page_to;
    /// Extened Page Timeout (in slots) offered by HCI command
    uint32_t ext_page_to;
    /// Page TS (in tick)
    uint32_t page_ts;
    /// Page latency (in tick)
    uint32_t page_latency;
    /// Page end TS (in tick)
    uint32_t page_end_ts;
    /// New Connection end TS (in 312.5us BT half-slots)
    uint32_t new_con_end_ts;

    /// Local Page Scan Repetition Mode (R0/R1/R2)
    uint8_t page_scan_rep_mode;
    /// Clock Offset (in 312.5us BT half-slots)
	uint8_t page_enable;
    /// BD Address of the paged device
    uint8_t bd_addr[6];
    /// Truncated page
    bool truncated;

    uint32_t clk_off;
    uint32_t clkn_bit4_2_0_1st;
    uint32_t page_sequence;
    uint32_t page_bit16_12_last;
    /// Npage
    uint16_t n_page;

    /// LT address
    uint8_t lt_addr;
    /// Link identifier
    uint8_t link_id;
    /// State
    uint8_t state;
    /// Page Response step
    uint8_t step;

    /// Page step BCH
    uint8_t bch_page[BT_LL_BCH_SIZE];
    /// 1st packet step BCH
    uint8_t bch_1st_pkt[BT_LL_BCH_SIZE];

    /// role switch enable
    bool switch_en;
    uint8_t end_reason;
    uint8_t train;
    bool name_req;

}bt_ll_page_env_t;

extern bt_ll_page_env_t page_env;		 	/*  page  */
int lmp_send_clock_off_req(int Lid);
int lmp_send_clk_off_res(int Lid);
uint8_t bt_ll_access_create_connection(uint8_t *bd_addr, uint16_t pkt_type, uint8_t sr, uint16_t clk_off, uint8_t allow_role_sw);

uint8_t bt_ll_access_cancel_connection(uint8_t *bd_addr);

int lmp_send_setup_complete(int Lid,uint8_t tid);

u32 bt_ll_access_page_init (uint32_t slot);

u32 bt_ll_isr_page(uint8_t sequence, uint32_t slot);

int32_t btc_task_conn_setup_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

int32_t hci_rd_clock_off_req(uint16_t con_hdl);


#endif /* _BT_LL_PAGE_H_ */
