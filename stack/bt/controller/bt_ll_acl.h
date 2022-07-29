/********************************************************************************************************
 * @file     bt_ll_acl.h
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

#ifndef BT_LL_ACL_H_
#define BT_LL_ACL_H_

#include "bt_hw.h"
#include "bt_ll_role_switch.h"
#include "bt_ll_sniff.h"
#include "bt_ll_test.h"
#include "bt_ll_encrypt.h"
#include "bt_ll_pair.h"

#include "bt_ll_timer.h"

#define	EM_BT_AUDIOBUF_SIZE			256
#define ACL_DATA_BUF_SIZE           700

#define	MAX_ACL_DATA_PACKET_LENGTH                	(700)


/// Position of ACL links in the active links bit field
#define ACT_ACL_POS    0
/// Mask of ACL links in the active links bit field
#define ACT_ACL_MSK    (((1<<MAX_NB_ACTIVE_ACL) - 1) << ACT_ACL_POS)
/// Position of SCO links in the active links bit field
#define ACT_SCO_POS    (MAX_NB_ACTIVE_ACL)
/// Mask of SCO links in the active links bit field
#define ACT_SCO_MSK    (((1<<MAX_NB_SYNC) - 1) << ACT_SCO_POS)

/// Set an ACL link as active
#define ACT_ACL_SET(field, link_id)           (field |= (1<<(link_id + ACT_ACL_POS)))
/// Set a SCO link as active
#define ACT_SCO_SET(field, sco_link_id)       (field |= (1<<(sco_link_id + ACT_SCO_POS)))
/// Get an ACL link as active
#define ACT_ACL_CHECK(field, link_id)         (field & (1<<(link_id + ACT_ACL_POS)))
/// Get a SCO link as active
#define ACT_SCO_CHECK(field, sco_link_id)     (field & (1<<(sco_link_id + ACT_SCO_POS)))
/// Clear an ACL link from active links
#define ACT_ACL_CLEAR(field, link_id)         (field &= ~(1<<(link_id + ACT_ACL_POS)))
/// Clear a SCO link from active links
#define ACT_SCO_CLEAR(field, sco_link_id)     (field &= ~(1<<(sco_link_id + ACT_SCO_POS)))

/// Activity type
enum ACT_TYPE
{
    ACT_TYPE_ACL,
    ACT_TYPE_SCO,
};

/// Active mode
enum ACT_MODE
{
    ACT_MODE_OFF,
    ACT_MODE_ON,
};

struct  acl_l2cap_buffer_s {
	unsigned long  len;
	unsigned long  cur_idx;
	uint8_t  acl_buffer_temp[1024];
};

struct DEVICE_INFO{
	uint16_t company_id;
	uint16_t subversion;
	uint8_t version;
};

/// Time information
/*@TRACE*/
typedef struct bt_hw_time{
    /// Integer part of the time (in half-slot)
    uint32_t hs;
    /// Fractional part of the time (in half-us) (range: 0-624)
    uint32_t hus;
} bt_hw_time_t;

typedef struct bt_hw_prog_params {
    /// Timestamp (in half-slots, based on local clock) and event offset (in half-us) of the programmed frame
    bt_hw_time_t time;
    bt_hw_time_t last_time;
    /// Bandwidth duration of the event using priority 1 (in half us)
    uint32_t bandwidth;
    /// Dummy value reported when an event happen during the frame or the frame is completed
    // uint32_t dummy;
    /// Priority during duration of bandwidth
    // uint8_t prio_1;
    /// Priority after bandwidth elapsed
    // uint8_t prio_2;
    /// Priority after trigger conditions
    // uint8_t prio_3;
    /// Priority when specific action occurs during the event
    uint8_t pti_prio;
    /// Control structure index
    uint8_t cs_idx;
    /// Mode (0: BLE, 1:BT)
    // uint8_t mode;

    /// Frame type (0: normal | 1: sniff | 2: ESCO | 3: CSB)
    uint8_t frm_type;
    /// Voice channel
    uint8_t vxchan;

    uint8_t link_id;
} bt_hw_prog_params_t;

struct acl_req{
	bool rsw_start;
	bool rsw_end;
	bool rem_name;
};


typedef struct bt_ll_acl_env
{
	//////		0: 12-byte
	uint8_t		bdaddr[6];// remote bdaddr
    uint8_t     class_of_dev[4]; //4 is just for 4byte alignment, class of dev is only 3byte
	uint8_t		role;
	uint8_t		ltaddr;

	uint16_t    link_policy_settings;
	uint16_t    con_hdl; //acl connection handle

	uint8_t		remote_features[FEATURE_PAGE_MAX][FEATS_LEN];
	uint8_t		remote_name[64];

    /// LMP TX queue
    struct co_list queue_lmp_tx;
    /// ACL TX queue
    struct co_list queue_acl_tx;

    /// Current value of half Rx window size (in us)
    uint32_t rxwinsze_us;

    /// Current L2CAP packet start timestamp (in BT half-slots)
    uint32_t curr_l2cap_start_ts;

    /// Value of CLKN when the last correct packet header is received  (in BT half-slots)
    uint32_t last_hec_ok_ts;

    /// Current clock offset (in BT half-slots)
    uint32_t clk_off;

    /**
     * Link Supervision TO
     * 0x0000: No Link_Supervision_Timeout.
     * N = 0xXXXX:
     *  Size: 2 Octets
     *  Range: 0x0001 to 0xFFFF
     *  Default: 0x7D00
     *  Mandatory Range: 0x0190 to 0xFFFF
     *  Time = N * 0.625 msec
     *  Time Range: 0.625 msec to 40.9 sec
     *  Time Default: BR/EDR 20 sec
     */
    uint16_t lsto;

    /**
     * Flush TO
     * 0: infinite - No Automatic Flush
     * N = 0xXXXX
     * Flush Timeout = N * 0.625 msec
     * Size: 11 bits
     * Range: 0x0001 to 0x07FF
     */
    uint16_t flush_to;

    /// Buffer element of the packet currently under transmission
    struct bt_em_acl_buf_elt* next_buf_elt;
    /// Buffer element of the packet to transmit next (1st segment already pending for transmission)
    struct bt_em_acl_buf_elt* curr_buf_elt;
    /// EM position of the next data to transmit
    uint16_t data_ptr;
    /// Length of data remaining to transmit from the current or next buffer
    uint16_t data_len;
    /// Enhanced Power Control supported by peer
    bool epc_supported;
    /// minimum power received flag
    bool    MinPowerRcv;
    /// maximum power received flag
    bool    MaxPowerRcv;
    /// Accumulated RSSI (to compute an average value)
    int16_t rssi_acc;
    /// Counter of received packets used in RSSI average
    uint8_t rssi_avg_cnt;
    /**
     *  Difference between the measured Received Signal Strength Indication (RSSI) and the limits of the Golden Receive
     *  Power Range (in dBm).
     *  Any positive RSSI value returned by the Controller indicates how many dB the RSSI is above the upper limit, any
     *  negative value indicates how many dB the RSSI is below the lower limit. The value zero indicates that the RSSI
     *  is inside the Golden Receive Power Range.
     *  Range: -128 <= N <= 127 (signed integer)
     */
    int8_t last_rssi_delta;


    /**
     * Allowed Basic Rate packet types: bit=1 => allowed | bit=0 => not allowed
     *  bit 0: DM1
     *  bit 1: DH1
     *  bit 2: DM3
     *  bit 3: DH3
     *  bit 4: DM5
     *  bit 5: DH5
     */
    uint8_t allowed_br_packet_types;
    /**
	 * Allowed Enhanced Data Rate packet types: bit=1 => allowed | bit=0 => not allowed
	 *  bit 0: DM1
	 *  bit 1: DH1_2
	 *  bit 2: DH1_3
	 *  bit 3: DH3_2
	 *  bit 4: DH3_3
	 *  bit 5: DH5_2
	 *  bit 6: DH5_3
	 */
	uint8_t allowed_edr_packet_types;
    /// Maximum number of slots per packet peer device is allowed to use
    uint8_t rx_max_slot;
    /// change packet type request
    uint8_t loc_cpt_req;
    /// token rate
    uint32_t TokenRate;
    /// peak bandwidth
    uint32_t PeakBandwidth;
    /// latency
    uint32_t Latency;
    /// delay variation
    uint32_t DelayVariation;
    /// service type
    uint8_t ServiceType;
    /// ACL basic packet type
    uint16_t acl_packettype;
    /// current packet type
    uint16_t cur_packettype;
    /// transmit current max slot
    uint8_t tx_max_slot_cur;

    /// packet type table 2 MB flag
    bool    PacketTypeTable2Mb;
    /// Rx preferred rate
    uint16_t RxPreferredRate;
    /// current packet type table
    uint8_t CurPacketTypeTable;
    /// temporary packet type table
    uint8_t ptt_tmp;

    /// SCO/ESCO Link identifier
    uint8_t     sco_active_msk;
    uint8_t     sco_link_id;
    uint8_t	    sco_frame_status;
    uint8_t	    sco_tog;
    uint32_t    sco_interval_num;

    /// Mode (0: idle | 1: normal | 2: Sniff transition |3: sniff enter | 4: Sniff | 5: role switch)
    uint8_t     mode;
    /// Current Mode (0: ACTIVE_MODE | 1: HOLD_MODE | 2: SNIFF_MODE |3: PARK_MODE )
    uint8_t     cur_mode;
    /// Maximum value of the peer clock drift (measured in ppm)
    uint8_t     peer_drift;
    /// Maximum value of the peer clock jitter (measured in us)
    uint8_t     peer_jitter;

    /// Number of programmed frames
    uint8_t     nb_prog;
    /// Programming enable/disable
    bool        prog_en;
    /// Enhanced data rate enable/disable
    bool        edr_en;
    /// TX flow ON/OFF
    bool        tx_flow;

    /// Index of TX descriptor currently used by HW [0:1]
    uint8_t txdesc_index_hw;
    /// Index of TX descriptor currently used by SW [0:1]
    uint8_t txdesc_index_sw;
    /// Number of prepared TX descriptors (0, 1, or 2)sam
    uint8_t txdesc_cnt;
    /// PTI update control
    uint8_t pti;

	///////		20:  12-byte + 4-byte
	bt_ll_tick_clkn_fcnt_t tcf;

    ////////	58: 4byte
	int8_t		sp_on;                          // bit0: TXCRYPT, bit1: RXCRYPT.
    uint8_t     sp_on_rsv;

    uint8_t     peer_no_more_data_cnt;
    uint8_t     local_no_more_data_cnt;

	uint8_t     task_state[32];                 // max we have 32 tasks.

	////////	80
	uint32_t    slot;
	uint32_t	acl_last_tx_pkts;

//	btc_test_mode_params_t test_mode;
//	btc_test_mode_t tst_mode;

	uint8_t 	max_slot_received;				/// received max slot


    bt_hw_prog_params_t pro_entry;
    uint32_t    pro_ts;

    uint16_t    clkoffset;
    uint8_t		rxbit_valid;
    uint8_t		sco_start;

    uint32_t    detach_ts;
    uint32_t    detach_to;

    uint16_t	txstat;
    uint16_t	rxstat;

    uint16_t	lasttxstat;
    uint16_t	lastrxstat;

    /// Polling interval (in slots)
    uint16_t    t_poll;
    uint8_t     Nbc;
    bool     	detach_req;

    uint8_t     state;       // State
    uint8_t     link_id;     // Link identifier
    /// Phase of the ACL frames (every 2 slots), compared to local clock (half-slots) (0, 1, 2 or 3)

	int8_t		sp_tid;

    uint8_t     sw_rx_seqn;

    uint32_t    avdtp_next_pkt_slot;
    uint32_t    rx_start_clkn;
    uint32_t	rxbit_offset_adj_num;

    uint8_t     pre_end_link;
    uint8_t     task_running;

	uint8_t 	remote_tx_flow;

    uint8_t     rx_tid;

    int16_t     offset;
    int16_t		offset_max;
    int16_t		offset_dc;
    int16_t		sniff_num;
    uint8_t		sniff_half_rxwnd_slot;
    uint8_t		sniff_miss;
    uint8_t		rx_pkttype;
    uint8_t		remote_support_ssp;

    uint8_t		rssi;
    uint8_t     rx_llid_in_sniff;
    uint16_t    rx_plen_lmp_in_sniff;
    uint8_t     *rx_pkt_lmp_in_sniff;

    uint32_t    rx_plen_acl_in_sniff;
    uint8_t     *rx_pkt_acl_in_sniff;

    int32_t        rxbit_offset_threshold;
#if OFFSET_CALIB_SEL
    int32_t		   rxbit_offset_all;
    uint8_t        rxbit_inc_flag_cnt;
    uint8_t        rxbit_inc_flag_cnt_d1;
    uint8_t        rxbit_dec_flag_cnt;
    uint8_t        rxbit_dec_flag_cnt_d1;
#else
    bt_ll_tick_clkn_fcnt_t tcf_drift;
#endif

	bool		rem_name_req;

    uint8_t		rem_fea_req_cmd;
    uint8_t		rem_ext_fea_req_cmd;
    uint8_t     enc_key_size;
    uint8_t     prefer_rate_sended:1;
    uint8_t     rsvd:7;

    /// Secure Connections (true if supported by both controllers and both Hosts)
    bool 		sp_sec_con;
    bool 		sec_con_en;
    bool 		sp_mode_en;
	uint8_t		link_initiator;

    bool 		tx_flow_off;
    bool 		rx_flow_off;
    bool	    pairing_req;
    bool	    authen_req;

    bool	    chg_lk_req;
    bool	    pause_req;
    bool	    con_setup;
    bool	    tx_flow_save;

    uint8_t	    detach_reason;

	/// quality of service notified flag
    bool        QOSNotified;


	struct acl_req req;

	struct ev_timer_event *bt_ll_timer_evts[BT_TIMER_EVENT_MAX];

    struct DEVICE_INFO remote_info;

} __attribute__ ((__packed__)) __attribute__ ((aligned (4))) bt_ll_acl_env_t;

extern bt_ll_acl_env_t env_acl[MAX_NB_ACTIVE_ACL];

_attribute_retention_code_ bt_ll_acl_env_t *bt_ll_acl_env_get (uint8_t link_id);

typedef int (*bt_ll_event_callback_func_t)(int event, uint8_t *p, int len);
typedef int (*bt_ll_acl_rx_callback_func_t)(int conhdl, int llid, uint8_t *p, int len);
typedef	void (*bt_ll_low_latency_func_t) (int link_id, uint32_t slot);

extern bt_ll_event_callback_func_t  bt_ll_event_callback;
extern bt_ll_acl_rx_callback_func_t bt_ll_acl_rx_callback;
extern bt_ll_low_latency_func_t	bt_ll_low_latency_cb;

extern struct  acl_l2cap_buffer_s  g_acl_l2cap_buffer[];

extern void  bt_context_store(unsigned long * bt_reg, unsigned long * ip_reg, unsigned long * modem_reg, unsigned long * radio_reg, unsigned long * pdzb_reg);
extern void  bt_context_restore(unsigned long * bt_reg, unsigned long * ip_reg, unsigned long * modem_reg, unsigned long * radio_reg, unsigned long * pdzb_reg);
_attribute_ram_code_sec_ void bt_modem_reg_restore(void);
void bt_modem_reg_store(void);
void bt_ll_event_callback_register (bt_ll_event_callback_func_t p);
void bt_ll_acl_rx_register (bt_ll_acl_rx_callback_func_t p);
_attribute_ram_code_sec_ int	bt_ll_acl_data_rx (uint8_t link_id, int llid, uint8_t *p, int len);
uint8_t bt_ll_send_acl(uint8_t link_id, int llid, void *param, int param_len);
_attribute_bt_retention_code_ uint16_t bt_ll_active_link_get(uint8_t link_id);
_attribute_bt_retention_code_ void bt_ll_active_mode_set(uint8_t type, uint8_t link_id, uint8_t mode);
#if OFFSET_CALIB_SEL
_attribute_retention_code_  int  bt_ll_acl_rxbit_offset_get_result(int link_id);
#else
int  bt_ll_acl_rxbit_offset_get_result(uint8_t link_id);
#endif
_attribute_retention_code_ void bt_ll_acl_rxbit_adjust(uint8_t link_id, uint32_t slot);
_attribute_retention_code_ int32_t 	bt_ll_acl_frm_isr(uint32_t slot);
void bt_ll_acl_low_latency (uint8_t link_id, uint32_t slot);
void bt_ll_low_latency_func_register (int en);
uint8_t bt_ll_get_nb_acl(uint8_t acl_flag);
uint8_t bt_ll_adjust_acl_link_prio(uint16_t con_hdl, uint8_t priority);

#endif /* BT_LL_ACL_H_ */
