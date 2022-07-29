/********************************************************************************************************
 * @file     bt_ll_inq.h
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

#ifndef _BT_LL_INQ_H_
#define _BT_LL_INQ_H_

#include <stdint.h>

enum{
	ST_INQ_IDLE,
	ST_INQ_RX_FHS,
	ST_INQ_RX_EIR,
};
enum{
	ST_INQ_STATE_IDLE,
	ST_INQ_STATE_GOING,
	ST_INQ_STATE_DONE,
};

typedef struct bt_ll_inq_res
{
    /// BdAddr
	uint8_t bd_addr[BD_ADDR_LEN];
    /// Page Scan Repetition Mode
    uint8_t page_scan_rep_mode;
    /// Class of device
    uint8_t class_of_dev[3];

    /// RSSi
    uint8_t  rssi;
    /// EIR bit in FHS packet
    bool     fhs_eir_bit;
    /// Extended inquiry response length
    uint8_t  eir_len;

    /// Clock Offset | Bit 15 Reserved | Bits 14-0: Bits 16-2 of CLKNslave-CLK
    uint32_t clk_off;
}bt_ll_inq_res_t;


typedef struct bt_ll_inq_env {

    // inquiryTO
    uint32_t inq_length_us;
    uint32_t inq_ext_length_us;
    uint32_t inq_ts;		// unit: tick
    uint32_t inq_latency;	// unit: tick
    uint32_t inq_end_ts;	// unit: tick

    // A/B Train
    uint16_t n_inq;
    // inquiry enable
    uint8_t inq_enable;
    // inquiry mode
    uint8_t inq_mode;

    uint8_t step;
    // inquiry responses number
    uint8_t nb_rsp_curr;
    uint8_t nb_rsp_max;
    // inquiry EIR mode enable
    uint8_t eir_en;

    // inquiry LAP
    uint8_t inq_lap[3];

    uint8_t bch[8];

    uint8_t state;
    uint8_t end_reason;
} bt_ll_inq_env_t;

extern bt_ll_inq_env_t inq_env;          	/*  inquiry  */

uint8_t bt_ll_access_allowed(bool Continuous);

uint8_t bt_ll_access_start_inq(uint8_t *plap, uint8_t InquiryLength, uint8_t NumResponse);

uint8_t bt_ll_access_cancel_inquiry(void);

u32 bt_ll_access_inquiry_init(uint32_t slot);

u32 bt_ll_isr_inquiry(uint8_t sequence, uint32_t slot);

_attribute_retention_code_access_ void bt_ll_inq_keep_mechanism(void);
_attribute_ram_code_ void bt_ll_inq_end(uint8_t reason);

#endif /* _BT_LL_INQ_H_ */
