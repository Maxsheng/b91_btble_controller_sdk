/********************************************************************************************************
 * @file     bt_ll_sniff.h
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

#ifndef _BT_LL_SNIFF_H_
#define _BT_LL_SNIFF_H_
#include "co_hci.h"

typedef int32_t (*btc_sniff_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);
btc_sniff_task_callback_t btc_sniff_mainloop_task_callback;
btc_sniff_task_callback_t btc_sniff_irq_mainloop_task_callback;
enum{
	SNIFF_MAINLOOP_TASK_ID_SNIFF_IDLE = 0,
	SNIFF_MAINLOOP_TASK_ID_HCI_SNIFF_MODE,
	SNIFF_MAINLOOP_TASK_ID_HCI_UNSNIFF_MODE,
	SNIFF_MAINLOOP_TASK_ID_LMP_SNIFF_REQ,
	SNIFF_MAINLOOP_TASK_ID_LMP_UNSNIFF_REQ,
	SNIFF_MAINLOOP_TASK_ID_LMP_SNIFF_REQ_ACCEPTED_ACK,
	SNIFF_MAINLOOP_TASK_ID_LMP_UNSNIFF_REQ_ACCEPTED_ACK,
	SNIFF_MAINLOOP_TASK_ID_LMP_SNIFF_ACC,
	SNIFF_MAINLOOP_TASK_ID_LMP_SNIFF_NOT_ACC,
	SNIFF_MAINLOOP_TASK_ID_LMP_UNSNIFF_ACC,

};
#define				SNIFF_HALF_RXWND_SLOT			4

/// Sniff Subrating parameters
typedef struct bt_ll_sniff_para
{
    uint16_t intv_max;
    uint16_t intv_min;
    uint16_t intv;
    uint16_t offset;
    uint16_t attempt;
    uint16_t to;
    uint8_t  timing_flag;
    bool     initiator;
}__attribute__ ((__packed__)) __attribute__ ((aligned (4))) bt_ll_sniff_para_t;

typedef struct BTC_SNIFF{
    /// Value of last_sync_ts used for the calculation of sched_rx_win_size, sched_clock_off and sched_bit_off
    uint32_t sched_last_sync_ts_used;
    /// Scheduled RX window size (in half-us)
    uint32_t sched_rx_win_size;
    /// Scheduled clock offset (in half-slots)
    uint32_t sched_clock_off;
    /// Anchor point in local clock domain (in half-slots)
    uint32_t anchor_point;

    uint32_t sniff_next_irq_tick; //60 fe ae 0a
    uint32_t cur_tick;//38 10 af 0a
    uint32_t last_tick;//18 14 40 0a
    uint32_t kick_clkn;//18 14 40 0a
    uint32_t cur_clkn;//18 14 40 0a

    uint32_t frame_no;
    uint32_t sniff_timeout;
    uint16_t cur_fcnt;//18 14 40 0a
    /// Scheduled bit offset (in half-us)
    int16_t sched_bit_off;
    /// Sniff offset (in BT slots)
    uint16_t offset;
    /// Sniff interval (in BT slots)
    uint16_t intv;
    /// Number of requested sniff attempts
    uint16_t att;
    /// Number of scheduled sniff attempts
    uint16_t sched_att;
    /// Sniff timeout (in BT slots)
    uint16_t to;

    /// Initialization method (0: init 1 / 1: init 2), from timing control flags
    uint8_t init;
    uint8_t step;

    uint16_t lcl_itv_max;
    uint16_t lcl_itv_min;

    /// Sniff parameter
    bt_ll_sniff_para_t sniff_params;
}__attribute__ ((__packed__)) __attribute__ ((aligned (4))) btc_sniff_t;


enum{
	ST_LMP_SNIFF_ACCEPT,
	ST_LMP_SNIFF_REQUEST,
	ST_LMP_SNIFF_REJECT,
};


enum{
    ST_SNIFF_IDLE = 0,

	ST_SNIFF_WAIT_ACCEPTED_ACK,
	ST_SNIFF_ENTER_TRANSITION,
	ST_SNIFF_ENTER_SNIFF,
	ST_SNIFF_ENTER_SNIFF_DATA,
	ST_SNIFF_ENTER_SSR,
    ST_SNIFF_NEGOTIATING,

};

typedef enum {
    BT_SNIFF_LP_MODE_SUSPEND = 0,
    BT_SNIFF_LP_MODE_DEEP_RET,
} bt_sniff_lp_mode_t;
enum{
	SNIFF_IRQ_TASK_ID_IDLE,
	SNIFF_IRQ_TASK_ID_WAIT_ENTER_SNIFF,
	SNIFF_IRQ_TASK_ID_FSM_ISR_PROCESS,
	SNIFF_IRQ_TASK_ID_FSM_ISR_LP,
	SNIFF_IRQ_TASK_ID_CONTINUE_LP,
	SNIFF_IRQ_TASK_ID_NEXT_IRQ,



};
extern _attribute_bt_data_retention_ bt_sniff_lp_mode_t bt_sniff_lp_mode;

extern _attribute_bt_data_retention_ volatile u32 deep_ret_cnt;
// sniff parameters
//extern btc_sniff_t sniff_par[MAX_NB_ACTIVE_ACL];

uint8_t bt_ll_acl_sniff_mode(uint16_t con_hdl, uint16_t sniff_max_interval,uint16_t sniff_min_interval, uint16_t sniff_attempt,uint16_t sniff_timeout);

//uint8_t bt_ll_acl_exit_sniff_mode(uint16_t con_hdl);

uint8_t bt_ll_acl_start_transition_mode(uint8_t link_id);

uint8_t bt_ll_acl_exit_transition_mode(uint8_t link_id);

void hci_send_mode_change_event(uint8_t status,uint16_t connection_handle,uint8_t cur_mode,uint16_t interval);

uint8_t bt_ll_acl_lowpow_sniff_allowed(uint32_t clock, uint8_t link_id);

uint8_t bt_ll_acl_program_et_allowed(uint32_t clock, uint8_t link_id);

u32 bt_ll_set_next_sniff_irq(uint8_t link_id);

void bt_ll_unsniff_restore(uint8_t link_id);

uint8_t bt_ll_sniff_peer_request(uint8_t link_id,uint8_t init, uint16_t d_sniff, uint16_t t_sniff, uint16_t sniff_attempt, uint16_t sniff_to);

void bt_set_sniff_lcl_interval(int link_id, uint16_t itv_max, uint16_t itv_min);
uint8_t bt_ll_acl_sniff(uint8_t link_id, uint16_t offset, uint16_t intv, uint16_t att, uint16_t to, uint8_t init);
void bt_ll_acl_sniff_exit(uint8_t link_id);
void bt_ll_acl_sniff_enter(uint8_t link_id);
_attribute_retention_code_ int bt_hw_sniff_prog(uint32_t clock, uint8_t link_id);
int lmp_send_lsto(uint8_t link_id,uint16_t timeout);

void bt_ll_sniff_process(uint8_t link_id, uint8_t sniff_miss);

static inline void bt_ll_set_sniff_lp_mode(bt_sniff_lp_mode_t mode)
{
    u32 r = core_disable_interrupt ();
    bt_sniff_lp_mode = mode;
    core_restore_interrupt (r);
}
void btc_sniff_register_module(btc_sniff_t* env_sniff);
#endif /* _BT_LL_SNIFF_H_ */
