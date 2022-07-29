/********************************************************************************************************
 * @file     bt_ll_pagescan.h
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

#ifndef _BT_LL_PAGESCAN_H_
#define _BT_LL_PAGESCAN_H_
#include <stdint.h>

#include "stack/hci/hci_cmd.h"

/*
 * ENUMERATIONS DEFINITIONS
 ****************************************************************************************
 */

/// Page scan event states
enum PSCAN_EVT_STATE
{
    PSCAN_EVT_WAIT,
    PSCAN_EVT_ACTIVE,
    PSCAN_EVT_END,
};

/// Page scan response steps
enum PSCAN_STEP
{
	PSCAN_STEP_IDLE,
    PSCAN_STEP_PSCAN,
    PSCAN_STEP_1ST_PKT,
};

/// Interlace Scan state
enum pscan_inter_state
{
    PSCAN_INTER_WIN0,
    PSCAN_INTER_WIN1,
    PSCAN_INTER_OFF,
};

enum pscan_low_power_state
{
    PSCAN_LOW_POWER_ENABLE = 1,
    PSCAN_LOW_POWER_PAGE2ACL = 2,
	PSCAN_LOW_POWER_DONE = 4,
};

typedef struct bt_ll_pagescan_env {
    uint32_t pscan_tick_next;
    uint32_t pscan_tick;
    uint32_t start_clk;

    /// Amount of time between consecutive page scans in slots (625 us)
    uint16_t pscan_intv;
    /// Amount of time for the duration of the page scan in slots (625 us)
    uint16_t pscan_win;
    /// New Connection end TS (in 312.5us BT half-slots)
    uint32_t new_con_end_ts;
    
    /// page scan type
    uint8_t pscan_typ;
    /// Link identifier
    uint8_t pscan_lid;
    /// Page Scan Response step
    uint8_t step;
    /// Phase in the page scan hop sequence 
    uint8_t phase_offset;

    /// Interlace scan state (1st window, 2nd window, or no interlace scan)
    uint8_t interlace_state;

    uint8_t	low_power_en;

    uint8_t low_power_task_abort;

    uint8_t rsvd; /// for 4-byte alignment
} bt_ll_pagescan_env_t;

extern bt_ll_pagescan_env_t pagescan_env;	/*  page scan  */

co_error_t	bt_ll_access_write_page_scan_activity(pagescan_inr_t interval, pagescan_win_t window);
co_error_t 	bt_ll_access_write_page_scan_type(uint8_t type);

uint32_t	bt_ll_access_pagescan_init(uint32_t slot);
uint32_t	bt_ll_isr_page_scan(uint8_t sequence, uint32_t slot);

_attribute_ram_code_ void bt_ll_pscan_frm_isr(uint32_t slot);

_attribute_retention_code_access_ void bt_hw_page_scan_en(uint32_t clock, int stop);
_attribute_retention_code_access_ void bt_hw_page_scan_sconnected(uint8_t link_id);

void bt_hw_rx_page_scan(uint32_t clock, uint16_t rxstat);

void bt_pscan_low_power_enable(int en, void *func_callback);

uint8_t bt_ll_reject_con_req(uint8_t *bd_addr, uint8_t reason);
uint8_t hci_accept_con_req_handler(uint8_t *bd_addr, uint8_t role);

co_error_t bt_ll_access_read_page_scan_activity(uint16_t* interval, uint16_t* window);

#endif /* _BT_LL_PAGESCAN_H_ */
