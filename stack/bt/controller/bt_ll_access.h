/********************************************************************************************************
 * @file     bt_ll_access.h
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

#ifndef _BT_LL_ACCESS_H_
#define _BT_LL_ACCESS_H_

#include <stdlib.h>        // standard
#include <stdint.h>        // integer

#include "bt_ll_pagescan.h"
#include "bt_ll_page.h"
#include "bt_ll_inqscan.h"
#include "bt_ll_inq.h"
#include "bt_ll_tcf.h"
#include "bt_ll_timer.h"
#include "bt_ll_bch.h"
#include "bt_em_buf.h"
#include "../../../incs/incs.h"
#include "bt_ll_sco.h"
#include "stack/btble/btble_scheduler.h"
#include "stack/btble/btble_pm.h"

#define BT_LL_ACL_NO_MORE_DATA_THRESH    5
#define BT_LL_ACL_OPPORTUNITY_SCO_PERIOD 6

#define REG_BT_EM_FHSLMP_SIZE 				18

/// Number on correctly received packets for interpreting the RSSI average
#define LD_ACL_RSSI_AVG_NB_PKT     			150

/// Maximum drop below the RSSI low threshold before max power is requested
#define LD_ACL_RSSI_BELOW_LOW_THR  			3

/// The maximum size the Rx window can reach before continuous programming is disabled (in us)
#define LD_ACL_RX_WIN_SIZE_PROG_LIM         900

/// Event delay threshold where 1 half-slot is added to the stop notification delay (in half-us)
#define LD_ACL_STOP_NOTIF_BIT_OFF_THR       400

/// Minimum scheduling delay threshold where SW tries to advance the schedule (in BT slots)
#define LD_ACL_EVT_DELAY_RESCHED_MIN        10

/// ACL event duration min default (in us)
#define LD_ACL_EVT_DUR_MIN_DFT     (2*SLOT_SIZE - LD_ACL_INTERFRAME_MARGIN)
/// ACL event duration min when the traffic is ongoing (in us)
#define LD_ACL_EVT_DUR_MIN_TRAFFIC  (4*SLOT_SIZE - LD_ACL_INTERFRAME_MARGIN)
/// ACL event automatic rescheduling attempts
#define LD_ACL_EVT_AUTO_RESCHED_ATT 4
/// ACL event stop threshold (considering 1 slot for TX and 1 slot for RX) (in BT half-slots)
#define LD_ACL_EVT_STOP_THR_MIN    (LD_PROG_DELAY + 2)

/// ACL event duration for  single Poll-Null exchange (in us)
#define LD_ACL_EVT_DUR_POLL_NULL   (SLOT_SIZE + 126)
/// ACL event priority increment on data exchange
#define LD_ACL_DATA_XCHG_PRIO_INC  1

/// Maximum value for RX/TX traffic weight indicator
#define LD_ACL_TRAFFIC_MAX              120

/// Default RX traffic value after receiving a packet successfully
#define LD_ACL_RX_TRAFFIC_DFT           2

/// Factor to apply to traffic weight for increasing event duration (increased by 1 every 2^n missed transmissions/receptions)
#define LD_ACL_TRAFFIC_DUR_INCR_FACTOR  4

/// Factor to apply to traffic weight for increasing event priority (increased by 1 every 2^n missed transmissions/receptions)
#define LD_ACL_TRAFFIC_PRIO_INCR_FACTOR  4

/// ACL Role Switch preparation delay (in BT slots)
#define LD_ACL_RSW_PREPARE_DELAY    10

/// ACL Role Switch event duration min (in BT slots)
#define LD_ACL_RSW_EVT_DUR_MIN         (2*SLOT_SIZE - LD_ACL_INTERFRAME_MARGIN)

/// ACL sniff event duration min (in BT slots)
#define LD_ACL_SNIFF_EVT_DUR_MIN       (2*SLOT_SIZE - LD_ACL_INTERFRAME_MARGIN)

/// Protection delay to stop programming ACL before sniff event in sniff transition mode (in BT half-slots)
#define LD_ACL_SNIFF_TRANS_ANC_PROTEC_DELAY  (8+LD_PROG_DELAY)

/// Minimum synchronization window when switching to a new piconet (in us)
#define LD_ACL_RSW_SYNC_WIN_SIZE_MIN   50
/// Maximum synchronization window when switching to a new piconet (in us)
#define LD_ACL_RSW_SYNC_WIN_SIZE_MAX   150

/// Minimum drift to notify peer's for new slot offset, when waiting for slave->master role switch (in us)
#define LD_ACL_RSW_DRIFT_NOTIF_THR     2

/// ACL Role Switch drift notification delay (in BT slots)
#define LD_ACL_RSW_DRIFT_NOTIF_DELAY   (LD_ACL_RSW_PREPARE_DELAY + 40)

/// Macro to enable a Basic Rate packet type in LD local bit field, from HCI bit field
#define LD_ACL_EN_BR_PKT_TYPE(br_types, pkt_types, pkt_type)       br_types  |= (packet_types & PACKET_TYPE_##pkt_type##_FLAG)  ? (1 << pkt_type##_IDX) : 0;

/// Macro to enable a Enhanced Data Rate packet type in LD local bit field, from HCI bit field
#define LD_ACL_EN_EDR_PKT_TYPE(edr_types, pkt_types, pkt_type, rate)     edr_types |= (packet_types & PACKET_TYPE_NO_##rate##_##pkt_type##_FLAG) ? 0 : (1 << pkt_type##_##rate##_IDX);

#define	ACL_PREFETCH_HUS			((BT_PREFETCHABORT_TIME_US + 25) * 2)

#define	ACL_PREFETCH_IRQ_GUARD		(90 * 2)

/// Index for DM1 packet type (used in both BR and EDR)
#define DM1_IDX         0

/// Basic Rate packet types indexes (classified by packet size)
#define DH1_IDX         1
#define DM3_IDX         2
#define DH3_IDX         3
#define DM5_IDX         4
#define DH5_IDX         5
#define BR_IDX_MAX      6

/// Enhanced Data Rate packet types indexes (classified by packet size)
#define DH1_2_IDX       1
#define DH1_3_IDX       2
#define DH3_2_IDX       3
#define DH3_3_IDX       4
#define DH5_2_IDX       5
#define DH5_3_IDX       6
#define EDR_IDX_MAX     7

/// ACL mode
enum ACL_MODE
{
	ACL_MODE_IDLE,
    ACL_MODE_NORMAL,
    ACL_MODE_SNIFF_TRANS,
    ACL_MODE_SNIFF_ENTER,
    ACL_MODE_SNIFF,
    ACL_MODE_ROLE_SWITCH_SCH_ENTER,
	ACL_MODE_ROLE_SWITCH_ENTER,
    ACL_MODE_ROLE_SWITCH,
};

enum{
	LINK_ID_PREALLOCATE,
	LINK_ID_ALLOCATE,
	LINK_ID_INVALID=0xFF,
};

/// ACL event states
enum ACL_STATE
{
    ACL_EVT_WAIT,
    ACL_EVT_ACTIVE,
    ACL_EVT_END,
};

enum PA_MODE
{
	PA_MODE_IDLE,
	PA_MODE_INQ,
	PA_MODE_PAGE,
	PA_MODE_INQSCAN,
	PA_MODE_PSCAN,
	PA_MODE_ACL,
	PA_MODE_ESCO,
};

typedef struct bt_ll_access_env {

	bt_ll_tick_clkn_fcnt_t tcf;
	bt_ll_tick_clkn_fcnt_t * ptcf;


	uint8_t   	lid;
    uint8_t  	state;
    uint8_t		scan_en;          /// scanning enable
    uint8_t		scan_en_cur;

    uint8_t		le_legAdv_en : 1;	//LE legacy ADV, only 0 and 1, can not be other value
    uint8_t		le_legScan_en: 1;   //LE legacy Scan
    uint8_t		app_scan_en  : 1;
    uint8_t		schedule_state_change: 1;
    uint8_t		le_legInit_en: 1;   //LE legacy Init
    uint8_t		rsvd : 3;

    ///lt_addr allocate
    uint8_t 	lt_addr_bitmap;
    // Authentication_Enable
    uint8_t 	hci_auth_en;
    /// Current RX descriptor index [0:N-1]
    uint8_t 	curr_rxdesc_index;
    /// Round trip delay
    uint8_t 	round_trip_delay;

    // 2 + 6 + 3 + 1 = 12
    uint16_t 	slot_isr_occur_instant;
    // BD ADDR of remote device
	uint8_t 	remote_bdaddr[6];
    // class of device of remote device
	uint8_t 	remote_class[3];
	bool		hci_dut_mode_en;

    uint8_t     hci_sp_mode;
    uint8_t     hci_le_supported_host;
    uint8_t     hci_simultaneous_le_host;
    uint8_t     hci_sec_con_host_supp;

    /// Bit field indicating the active links (Bits [M+N-1:N] -> SCO | Bits [N-1:0] -> ACL)
    uint16_t 	active_links;
    /// Event mask
    struct evt_mask 	evt_msk;

    /// Event mask page 2
    struct evt_mask 	evt_msk_page_2;

    uint8_t     oob_r[KEY_LEN];
    uint8_t     oob_c[KEY_LEN];

	uint8_t     hci_pin_type;
	uint8_t     hci_sp_debug_mode;
    uint8_t     current_acl_num;
    uint8_t		scan_en_save;          /// scanning enable

    uint32_t	instant_interval;
    uint16_t    dft_link_policy_settings;
    bool        reset_req;
    bool        detach_req;

    uint8_t     pa_mode;
    uint8_t     pa_ctrl;
}_attribute_aligned_(4) bt_ll_access_env_t;

typedef enum
{
    /// idle state
    ST_SP_HEADER=0x01,			// <
    ST_SP_ENCAP_PAYLOAD,
    ST_SP_HEADER1,				// >
    ST_SP_ENCAP_PAYLOAD1,
    // cal pending: ST_SP_CAL_DHKEY
    ST_SP_CONFIRM,
    ST_SP_NUMBER0,				// <
    // cal pending: ST_SP_CAL_DHKEYCHK
    ST_SP_NUMBER1,				// >
    ST_SP_DHKEY0,				// <
    ST_SP_DHKEY1,				// >

    ST_LMP_AUTH0 = 0x10,
    //cal pending: ST_SP_CAL_AUTH
    ST_LMP_AUTH1,
    ST_LMP_ENC_START_RCVD = 0x20,
    ST_LMP_ENC_PAUSE = 0x21,
    ST_LMP_ENC_RESUME = 0x22,
    ST_LMP_ENC_START = 0x23,

    ST_SP_ERR = 0x40,


    ST_SP_CAL=0x80,
    ST_SP_CAL_DHKEY=0x80,	// generate random number, calculate DHKEY & commitment (f1)
    ST_SP_CAL_DHKEYCHK,		// calculate Link key(f2) & DHKey check (f3)
    ST_SP_CAL_AUTH,			// generate random number, calculate response(e1)
    ST_SP_CAL_ENC,			// calculate encryption key
} LMP_SECURITY_STATE;

typedef enum
{
    /// idle state
    ST_BT_LL_IDLE=0,
    /// inquiry state
    ST_BT_LL_INQUIRY,
    /// inquiry scan state
    ST_BT_LL_INQUIRY_SCAN,
    /// inquiry scan response wait state
    ST_BT_LL_INQSCAN_RESP_WAIT,
    /// page state
    ST_BT_LL_PAGE,
    /// page response state
    ST_BT_LL_PAGE_RESP,
    /// master connect pending state
    ST_BT_LL_MCONNECT_PENDING,
    /// page scan state
    ST_BT_LL_PAGE_SCAN,
    /// page scan response state
    ST_BT_LL_PSCAN_RESP,
    /// slave connect pending state
    ST_BT_LL_SCONNECT_PENDING,


    ST_BT_LL_ACL = 0x10,
    ST_BT_LL_STATE_MAX
} BT_LL_ACCESS_STATE;

//////////////////// Link index ////////////////////////////////////
typedef struct bt_ll_lid {
    uint8_t cur;
    uint8_t max;
    uint8_t enbit;
    uint8_t rsvd;
} __attribute__ ((aligned (4))) bt_ll_lid_t;

extern bt_ll_lid_t env_lid;

extern bt_ll_access_env_t env_access;

typedef struct page_task_env{
	uint8_t update_flag;
	uint8_t page_duration;
	uint8_t page_interval;
	uint8_t rsvd;
	uint32_t page_next_start_tick;	//page start time
}page_task_env_t;

extern page_task_env_t page_start_env ;		/*	used buy ble to negotiate task*/



static inline int bt_ll_get_current_lid (void)
{
	return env_sch.last_link;
}

static inline void	bt_ll_lid_free (uint8_t lid)
{
	if (lid < env_lid.max)
	{
		GLOBAL_INT_DISABLE();
		env_lid.enbit &= ~(1<<lid);
		GLOBAL_INT_RESTORE();
	}
}
//static inline int	bt_ll_lid_get_cur (void) {return env_lid.cur;}
static inline void	bt_ll_lid_set_cur (uint8_t lid)
{
	env_lid.cur = lid;
}
static inline void	bt_ll_lid_set_max (uint8_t max)
{
	env_lid.max = max;
}
static inline void	bt_ll_lid_init (uint8_t max)
{
	env_lid.max = max;
	env_lid.cur = 0;
	env_lid.enbit = 0;
}

int	bt_ll_acl_exist (uint8_t link_id);
int bt_ll_acl_alive(void);


typedef void(*btc_module_init_callback_t)(void);

btc_module_init_callback_t btc_module_init_cb;

static inline void btc_module_init_register_callback(btc_module_init_callback_t init_func)
{
	if(init_func)
	{
		btc_module_init_cb = init_func;
	}
}


void bt_ll_controller_reset(void);
uint8_t hci_reset_cmd_handler(void);
void btc_env_reset(void);
_attribute_retention_code_access_ uint8_t bt_ll_lid_allocate(uint8_t allocate);
_attribute_ram_code_ void bt_ll_compute_fhs(uint16_t fhs_buf_ptr, uint8_t *bch_ptr, uint8_t eir, uint8_t sr, uint8_t *bd_addr, uint8_t *class_of_dev, uint8_t lt_addr, uint8_t page_scan_mode);
_attribute_retention_code_ void bt_ll_obid_check(uint8_t cs_idx, uint16_t rxstat, uint8_t rxtype);
_attribute_retention_code_ uint32_t bt_ll_acl_next_et_slot(uint8_t link_id);
_attribute_retention_code_ u8 bt_ll_acl_rx_handle(uint8_t link_id, uint32_t slot, uint8_t nb_rx);
_attribute_ram_code_ void bt_acl_tx_cfm_handler(uint8_t link_id, uint32_t clock);
_attribute_retention_code_ void bt_ll_acl_tx_handle(uint8_t link_id, uint32_t clock);
_attribute_ram_code_ u32 bt_ll_set_next_esco_irq (uint8_t link_id, uint32_t slot);
_attribute_retention_code_ void bt_ll_isr_acl_end_task (uint8_t link_id, uint32_t sleep_1m25);
_attribute_retention_code_ int32_t bt_ll_acl_pre_end (uint8_t sch_link);
_attribute_retention_code_ uint32_t bt_ll_acl_stimer_isr(uint8_t sequence, uint8_t sch_link);
_attribute_retention_code_ uint8_t bt_ll_rxdesc_check(uint8_t label);
_attribute_retention_code_ void bt_ll_rxdesc_free(uint8_t link_id);
_attribute_retention_code_ uint8_t bt_ll_release_rxdesc(uint8_t en);
_attribute_retention_code_ uint8_t bt_ll_get_hw_rxdesc(void);
uint8_t bt_ll_acl_lmp_tx(uint8_t link_id, struct bt_em_lmp_buf_elt* buf_elt);
uint8_t bt_ll_acl_data_tx(uint8_t link_id, struct bt_em_acl_buf_elt* buf_elt);
uint8_t bt_ll_acl_flow_off(uint8_t link_id);
uint8_t bt_ll_acl_flow_on(uint8_t link_id);
uint8_t bt_ll_acl_flow_disable(uint8_t link_id);
uint8_t bt_ll_acl_flow_restore(uint8_t link_id);
uint8_t bt_ll_acl_data_flush(uint8_t link_id, uint8_t *nb_flushed, bool all);
uint8_t bt_ll_acl_reset(uint8_t link_id);

_attribute_retention_code_ int32_t bt_ll_access_update (uint8_t scan_allow);
int32_t bt_ll_acl_tx_is_empty(uint8_t link_id);
int32_t bt_ll_lmp_tx_is_empty(uint8_t link_id);

static inline uint8_t bt_ll_page_is_enable(void)
{
	return (page_env.page_enable && page_start_env.update_flag);
}

page_task_env_t bt_ll_get_page_state(void);

#endif // _BT_LL_ACCESS_H_
