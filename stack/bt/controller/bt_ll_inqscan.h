/********************************************************************************************************
 * @file     bt_ll_inqscan.h
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

#ifndef _BT_LL_INQSCAN_H_
#define _BT_LL_INQSCAN_H_

#include "stack/hci/hci_cmd.h"

#define    SCAN_ENABLE         0
#define    SCAN_STOP           1

/// Interlace Scan state
enum iscan_inter_state {
    ISCAN_INTER_WIN0,
    ISCAN_INTER_WIN1,
    ISCAN_INTER_OFF,
};

enum iscan_low_power_state {
    ISCAN_LOW_POWER_ENABLE = 1,
    ISCAN_LOW_POWER_DONE = 2,
};

typedef struct bt_ll_inqscan_env {
    //////////////////////////// inquiry scan ///////////////////////////////////////
    uint32_t iscn_tick_next;
    uint32_t iscn_tick;
    uint32_t iscn_duruation;

    uint16_t iscn_eir_pkt_type;
    uint16_t iscn_eir_len;

    uint8_t iscn_eir_slots;
    // parameter, set through hci command
    uint8_t eir_en;
    /// Interlace scan state (1st window, 2nd window, or no interlace scan)
    uint8_t interlace_state;
    /// Page scan repetition mode
    uint8_t pscan_rep_mode;

    /// Amount of time between consecutive inquiry scans in slots (625 us)
    uint16_t iscn_intv;
    /// Amount of time for the duration of the inquiry scan in slots (625 us)
    uint16_t iscn_win;

    /// inquiry scan type
    uint8_t iscn_typ;
    uint8_t iscn_use_fec;
    uint8_t low_power_en;
    uint8_t phase_offset;

    uint8_t low_power_task_abort;
    uint8_t iscn_lap[3];
} bt_ll_inqscan_env_t;

extern bt_ll_inqscan_env_t inqscan_env;   	/*  inquiry scan */

extern void bt_hw_start_iscn(uint8_t *Lap);

co_error_t  bt_ll_access_read_scan_enable(uint8_t *en);
co_error_t 	bt_ll_access_write_scan_enable(bb_scan_en_t en);
uint8_t bt_ll_access_write_cur_iac_lap(uint8_t num, uint8_t *lap);

co_error_t	bt_ll_access_write_inquiry_scan_activity(inqscan_inr_t interval, inqscan_win_t window);

co_error_t	bt_ll_access_write_inquiry_scan_type(uint8_t type);

u32 bt_ll_access_inqscan_init(uint32_t clock);

void bt_iscan_low_power_enable(int en);

u32 bt_ll_isr_inquiry_scan(uint8_t sequence, uint32_t clock);

void bt_ll_iscn_frm_isr(uint32_t clock);
co_error_t bt_ll_access_read_inquiry_scan_activity(uint16_t* interval, uint16_t* window);
_attribute_ram_code_ void bt_ll_scan_disable(void);
_attribute_ram_code_ void bt_ll_scan_restore(void);

_attribute_ram_code_ void bt_hw_inquiry_scan(uint32_t clock, int stop);

#endif /* _BT_LL_INQSCAN_H_ */
