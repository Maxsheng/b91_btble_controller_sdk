/********************************************************************************************************
 * @file     bt_ll_sco.h
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

#ifndef STACK_BT_CONTROLLER_BT_LL_SCO_H_
#define STACK_BT_CONTROLLER_BT_LL_SCO_H_


enum{
	SCO_MAINLOOP_TASK_ID_SCO_IDLE = 0,
	SCO_MAINLOOP_TASK_ID_HCI_SCO_SETUP,
	SCO_MAINLOOP_TASK_ID_HCI_SCO_ACCEPT_CON_REQ,
	SCO_MAINLOOP_TASK_ID_HCI_SCO_REJECT,
	SCO_MAINLOOP_TASK_ID_HCI_SCO_DISCONNECT,


	SCO_MAINLOOP_TASK_ID_LMP_SCO_REQ_ACCEPT,
	SCO_MAINLOOP_TASK_ID_LMP_SCO_REMOVE_REQ_ACCEPT,

	SCO_MAINLOOP_TASK_ID_LMP_SCO_REQ,
	SCO_MAINLOOP_TASK_ID_LMP_TX_CFM,

	SCO_MAINLOOP_TASK_ID_LMP_SCO_RMV_REQ,


};
enum{
	SCO_IRQ_MAINLOOP_TASK_ID_SCO_IDLE = 0,
	SCO_IRQ_MAINLOOP_TASK_ID_STIME_SRART,



};
enum{
	ST_TASK_ID_SCO_IDLE = 0,
	ST_TASK_ID_SCO_NEGO_WAIT_PEER_RSP,
	ST_TASK_ID_SCO_NEGO_WAIT_PEER_MOD_RSP,
	ST_TASK_ID_SCO_NEGO_WAIT_HOST_RSP,
	ST_TASK_ID_SCO_CONNECTED,

	ST_TASK_ID_SCO_NEGO_WAIT_PEER_CFM,
	ST_TASK_ID_SCO_NEGO_WAIT_PEER_MOD_CFM,
	ST_TASK_ID_SCO_NEGO_WAIT_PEER_DISC_CFM,
	ST_TASK_ID_SCO_WAIT_ACCEPTED_ACK,



	SCO_TASK_ID_LMP_UNSCO_REQ,
	SCO_TASK_ID_LMP_SCO_REQ_ACCEPTED_ACK,
	SCO_TASK_ID_LMP_UNSCO_REQ_ACCEPTED_ACK,
	SCO_TASK_ID_LMP_SCO_ACC,
	SCO_TASK_ID_LMP_SCO_NOT_ACC,
	SCO_TASK_ID_LMP_UNSCO_ACC,

};
/*
 * DEFINES
 *****************************************************************************************
 */

/// Default sync handle
#define DEFAUT_SYNC_HANDLE     0x00


/// Definitions for Synchronous data path configuration
#define LINK1_CFG_MSK       0x000F
#define LINK1_CFG_SFT       0
#define LINK2_CFG_MSK       0x00F0
#define LINK2_CFG_SFT       4
#define LINK3_CFG_MSK       0x0F00
#define LINK3_CFG_SFT       8

/// Voice coding configurations
#define AULAW_CODE_POS    0
#define AULAW_CODE_MSK    0xF
#define CVSD_EN_POS       4
#define CVSD_EN_MSK       0x10
#define AULAW_EN_POS      5
#define AULAW_EN_MSK      0x20
#define VOICE_CODE_MSK    0xF
#define LINEAR_CVSD       0x10
#define LINEAR_ALAW       0x22
#define LINEAR_ULAW       0x2A
#define ALAW_CVSD         0x31
#define ALAW_ALAW         0x00
#define ALAW_ULAW         0x2B
#define ULAW_CVSD         0x35
#define ULAW_ALAW         0x27
#define ULAW_ULAW         0x00
#define TRANSP            0x00

/// Linear format
#define LINEAR_FORMAT_8_BITS    0x00
#define LINEAR_FORMAT_13_BITS   0x01
#define LINEAR_FORMAT_14_BITS   0x02
#define LINEAR_FORMAT_16_BITS   0x03

/// Threshold on eSCO interval to skip one interval when switching to new parameters (in slots)
#define LD_SCO_UPD_INTV_SKIP_THR     6

#define SCO_RSVD_SLOT   1
#define SCO_RETX_SLOT   2


#define VOICE_OVER_HCI  1

#if VOICE_OVER_HCI
/// Number of Synchronous TX buffers used for one VoHCI link
#define SYNC_TX_BUF_NB     4
/// Number of Synchronous RX buffers used for one VoHCI link
#define SYNC_RX_BUF_NB     3
/// Guard space reserved for HCI header in Sync RX buffers
#define SYNC_RX_BUF_HEADER_SPACE      (HCI_SYNC_HDR_LEN + 1)
#endif //(VOICE_OVER_HCI)

#if VOICE_OVER_HCI
/// Audio interrupt delay (time before an (e)SCO instant where the audio IRQ is raised)
#define LD_SCO_AUDIO_IRQ_DELAY     2 //rwip_prog_delay
#endif //VOICE_OVER_HCI


#define ASSERT_INFO(cond, param0, param1)

/*
 * DEFINITIONS
 ****************************************************************************************
 */
/// Number of eSCO packet types
#define ESCO_LINKTYPE_NUM       7
/// Number of eSCO interval (supported)
#define ESCO_INTERVAL_NUM       17

/// eSCO packet indexes
#define EV3_PACKET_INDEX        0
#define EV4_PACKET_INDEX        1
#define EV5_PACKET_INDEX        2
#define EV3_2_PACKET_INDEX      3
#define EV3_3_PACKET_INDEX      4
#define EV5_2_PACKET_INDEX      5
#define EV5_3_PACKET_INDEX      6

/// eSCO maximum interval
#define ESCO_MAX_INTERVAL           36
#define ESCO_WESCO_MAX_1SLOTPKT     4
#define ESCO_WESCO_MIN_1SLOTPKT     2
#define ESCO_WESCO_3SLOTPKT         6
#define ESCO_WESCO_NO_REPETITION    0

/// Max number of negotiation before failure
#define ESCO_NEGOCNT_MAX            10

/// Commands for synchronous link
#define SYNC_HL_REQ             0
#define SYNC_PEER_REQ           1
#define SYNC_HL_RSP             2
#define SYNC_PEER_RSP           3
#define SYNC_HL_MODIF           4
#define SYNC_PEER_MODIF         5
#define SYNC_UPDATE             6

#define HCI_SYNC_HDR_PSF_GOOD      0
#define HCI_SYNC_HDR_PSF_INVALID   1
#define HCI_SYNC_HDR_PSF_NO_RCVD   2
#define HCI_SYNC_HDR_PSF_LOST      3

/// SCO Negotiation States
enum LC_SCO_STATES
{
    LC_SCO_IDLE,
    LC_SCO_NEGO_WAIT_HOST_RSP,
    LC_SCO_NEGO_WAIT_PEER_RSP,
    LC_SCO_CONNECTED,
    LC_SCO_NEGO_WAIT_PEER_MOD_RSP,
    LC_SCO_NEGO_WAIT_PEER_MOD_CFM,
    LC_SCO_NEGO_WAIT_PEER_DISC_RSP,
    LC_SCO_NEGO_WAIT_PEER_DISC_CFM,
    LC_SCO_NEGO_WAIT_PEER_CFM,
};

/// SCO Negotiation Action
enum LC_SCO_ACTION
{
    LC_SCO_ACCEPT,
    LC_SCO_REJECT,
    LC_SCO_REQUEST,
};

/// SCO event states
enum SCO_EVT_STATE
{
    SCO_EVT_WAIT,
    SCO_EVT_ACTIVE,
    SCO_EVT_END,
};

/// SCO update
enum SCO_UPDATE
{
    SCO_NO_UPDATE,
    SCO_UPDATE_STEP1,
    SCO_UPDATE_STEP2,
    SCO_UPDATE_STEP3,
};

/// SCO packet types indexes
enum SCO_IDX
{
    IDX_SCO_PACKET_HV1  ,
    IDX_SCO_PACKET_HV3  ,
    IDX_SCO_PACKET_NULL ,
    IDX_SCO_PACKET_EV3  ,
    IDX_SCO_PACKET_EV4  ,
    IDX_SCO_PACKET_EV5  ,
    IDX_SCO_PACKET_EV3_2,
    IDX_SCO_PACKET_EV3_3,
    IDX_SCO_PACKET_EV5_2,
    IDX_SCO_PACKET_EV5_3,
};


/// SCO invalid flag, Table 2-35 ? (e)SCO Invalid Flag description
enum SCO_INVL_FLAG
{
    SCO_INVL_FLAG_PKT_CORRECT,  // Correct Packet reception: No sync/HEC/CRC Errors
    SCO_INVL_FLAG_HEC_CORRECT,  // No sync/HEC Error, but CRC error detection
    SCO_INVL_FLAG_ERROR,        // Sync or HEC Error detected / CRC error meaningless
    SCO_INVL_FLAG_RSVD,         // Reserved
};


/// SCO Negotiation Action
enum SCO_FRAME_STATE
{
    SCO_FRAME_STS_IDLE,
    SCO_FRAME_STS_START,
	SCO_FRAME_STS_NORMAL,
    SCO_FRAME_STS_END,
};

/// Audio configuration parameters
typedef struct ld_sco_audio_settings
{
    uint8_t in_coding;
    uint8_t air_coding;
    uint8_t data_path;
    uint8_t in_data_format;
    uint8_t in_sample_size;
    uint8_t msb_position;
    uint8_t rsv[2];
}__attribute__ ((__packed__)) ld_sco_audio_settings_t;

/// SCO parameters structure
typedef struct ld_sco_params
{
    /// Audio settings
    struct ld_sco_audio_settings audio;
    /// RX packet length (in bytes)
    uint16_t rx_pkt_len;
    /// TX packet length (in bytes)
    uint16_t tx_pkt_len;

    /// eSCO LT address
    uint8_t lt_addr;
    /// Link type (0: SCO | 2: ESCO)
    uint8_t sync_type;
    /// RX packet type (as defined in BT specification LMP:5.2) (HV2 not supported)
    uint8_t rx_pkt_type;
    /// TX packet type (as defined in BT specification LMP:5.2) (HV2 not supported)
    uint8_t tx_pkt_type;

    /// Synchronous link offset (in slots)
    uint8_t d_esco;
    /// Synchronous link interval (in slots)
    uint8_t t_esco;
    /// Retransmission window size (in slots) (eSCO only)
    uint8_t w_esco;
    /// Initialization method (0: init 1 / 1: init 2), from timing control flags
    uint8_t init;
}__attribute__ ((__packed__)) ld_sco_params_t;

/*
 * STRUCTURE DEFINITION
 *****************************************************************************************
 */

/// LC SCO link parameters structure
typedef struct lc_sco_link_params_tag
{
    /// Synchronous packet type bit field given by Host
    uint16_t sync_pkt_type;
    /// ACL link ID
    uint16_t link_id;

    /// Synchronous handle used on ESCO link
    uint8_t sync_hdl;
    /// Synchronous LT address used on ESCO link
    uint8_t sync_lt_addr;
    /// Synchronous connection type (SCO / ESCO)
    uint8_t sync_type;
    /// Current state of the synchronous connection
    uint8_t state;

    /// Packet length in receive direction (in bytes)
    uint16_t rx_pkt_len;
    /// Packet length in transmit direction (in bytes)
    uint16_t tx_pkt_len;

    /*
     * Store parameters that can be renegotiated
     */
    /// Interval (in slots)
    uint8_t t_esco;
    /// Retransmission window (in slots)
    uint8_t w_esco;
    /// Disconnection reason
    uint8_t disc_reason;
    uint8_t rsv;

} __attribute__ ((__packed__)) lc_sco_link_params_tag_t;

typedef struct lc_sco_air_params_tag
{
    ///Transaction ID
    uint8_t  tr_id;
    ///eSCO handle
    uint8_t  esco_hdl;
    ///eSCo LT Address
    uint8_t  esco_lt_addr;
    ///timing control flags
    uint8_t  flags;

    ///Packet Length M2S
    uint16_t m2s_pkt_len;
    ///Packet Length S2m
    uint16_t s2m_pkt_len;

    ///eSCO packet type M2S
    uint8_t  m2s_pkt_type;
    ///eSCO packet type S2M
    uint8_t  s2m_pkt_type;
    ///Air Mode
    uint8_t  air_mode;
    ///Negotiation state
    uint8_t  negt_st;

    ///Desco
    uint8_t  d_esco;
    ///t_esco
    uint8_t  t_esco;
    ///Wesco
    uint8_t  w_esco;
    uint8_t  rsv;
}__attribute__ ((__packed__)) lc_sco_air_params_tag_t;

typedef struct lc_sco_host_params_tag
{
    uint32_t      tx_bw;                // Transmit Bandwidth (in B/sec)
    uint32_t      rx_bw;                // Receive Bandwidth (in B/sec)
    uint16_t      tx_cod_fr_sz;         // Transmit Codec Frame Size (in B)
    uint16_t      rx_cod_fr_sz;         // Receive Codec Frame Size (in B)

    uint32_t      in_bw;                // Input Bandwidth (in B/sec)
    uint32_t      out_bw;               // Output Bandwidth (in B/sec)
    uint16_t      in_cod_data_sz;       // Input Coded Data Size (in bits)
    uint16_t      out_cod_data_sz;      // Output Coded Data Size (in bits)

    uint16_t      max_lat;              // Max Latency (in ms)
    uint16_t      packet_type;          // Packet Type (HV1 / HV2 / HV3 / EV3 / EV4 / EV5 / 2EV3 / 3EV3 / 2EV5 / 3EV5)

    uint8_t       in_data_fmt;          // Input PCM Data Format
    uint8_t       out_data_fmt;         // Output PCM Data Format
    uint8_t       in_msb_pos;           // Input PCM Sample Payload MSB Position (in bits)
    uint8_t       out_msb_pos;          // Output PCM Sample Payload MSB Position (in bits)

    uint8_t       in_data_path;         // Input Data Path (VoHCI / PCM / Other ...)
    uint8_t       out_data_path;        // Output Data Path
    uint8_t       in_tr_unit_sz;        // Input Transport Unit Size (in bits)
    uint8_t       out_tr_unit_sz;       // Output Transport Unit Size (in bits)

    uint8_t       tx_cod_fmt[5];        // Transmit Coding Format
    uint8_t       rx_cod_fmt[5];        // Receive Coding Format
    uint8_t       in_cod_fmt[5];        // Input Coding Format
    uint8_t       out_cod_fmt[5];       // Output Coding Format

    uint8_t       retx_eff;             // Retransmission Effort (No, opt power, opt quality, don't care)
    uint8_t       rsv[3];
}__attribute__ ((__packed__)) lc_sco_host_params_tag_t;

typedef struct lc_sco_nego_infos_tag
{
    /// Flag indicating if local is initiator of the negotiation
    bool initiator;

    /// Flag indicating if the old HCI command is used by Host (HCI_setup_sync_con / HCI_acc_sync_con)
    bool old_style_hci;

    /// Flag indicating if the very old HCI command is used by Host when accepting the sync link (HCI_acc_con / HCI_rej_con)
    bool very_old_style_hci;

    /// Flag indicating if the procedure was initiated by the host (true) or not (false)
    bool host_initiated;
}__attribute__ ((__packed__)) lc_sco_nego_infos_tag_t;


/// LC SCO environment structure
typedef struct lc_sco_nego_params_tag
{
    /// General information of the negotiation
    struct lc_sco_nego_infos_tag nego_infos;

    /// Parameters received from Host
    struct lc_sco_host_params_tag host_params;

    /// Parameters received from peer
    struct lc_sco_air_params_tag air_params_received;

    /// Parameters sent to Host
    struct lc_sco_air_params_tag air_params_sent;
}__attribute__ ((__packed__)) lc_sco_nego_params_tag_t;

/// LC SCO environment structure
typedef struct bt_ll_sco_env
{

    /// Anchor point in local clock domain (in half-slots)
    uint32_t anchor_point;

    /// RX packet length (in bytes)
    uint16_t rx_pkt_len;
    /// TX packet length (in bytes)
    uint16_t tx_pkt_len;

    uint8_t sco_used;
    /// End of SCO
    uint8_t end;
    /// eSCO LT address
    uint8_t lt_addr;
    /// Link type (0: SCO | 2: ESCO)
    uint8_t sync_type;

    /// RX packet type (as defined in BT specification LMP:5.2)
    uint8_t rx_pkt_type;
    /// TX packet type (as defined in BT specification LMP:5.2)
    uint8_t tx_pkt_type;
    /// Synchronous link offset (in slots)
    uint8_t d_esco;
    /// Synchronous link interval (in slots)
    uint8_t t_esco;

    uint8_t w_esco;
    /// Initialization method (0: init 1 / 1: init 2), from timing control flags
    uint8_t init;
    /// Number of retransmission attempts (0, 1 or 2) (eSCO only)
    uint8_t retx_att_nb;
    /// Number of reserved slots
    uint8_t rsvd_slots;

    /// Link identifier
    uint8_t link_id;
    /// SCO Link identifier
    uint8_t sco_link_id;
    /// Number of programmed frames (0, 1 or 2 (in case of full bandwidth))
    uint8_t nb_prog;
    /// Flag indicating full bandwidth (e.g. eSCO EV3 / T=6 with 2 reTx)
    bool bw_full;

    /// eSCO parameters update requested by HL
    uint8_t update;
    /// Audio data path (Voice over HCI, PCM, etc ...)
    uint8_t data_path;
    #if VOICE_OVER_HCI
    /// Buffer TX queue
    //struct co_list queue_tx;
    /// Sample size at the entry of the audio path (True: 16-bits - False: 8 bits)
    bool sample_size_16_bits;
    #endif //VOICE_OVER_HCI
    /**
     * Loopback mode
     *
     * 0x00 No Loopback mode enabled (default).
     * 0x01 Enable Local Loopback.
     * 0x02 Enable Remote Loopback. (Not Supported by AMPs)
     * 0x03-0xFF Reserved for future use.
     */
    uint8_t loopback_mode;

    uint8_t mic_mute;
    uint8_t spk_mute;
    uint8_t rsvd[2];

    /// Pointer to the structure for allocated the sync link
    struct lc_sco_link_params_tag link_params;

    /// Pointer to the structure for allocated the negotiation
    struct lc_sco_nego_params_tag nego_params;

} __attribute__ ((__packed__)) btc_sco_t;

#define SCO_BUFF_SIZE           (4 + 4 + 60)
#define SCO_RX_BUFF_NUM          2
#define SCO_TX_BUFF_NUM          2

//extern uint8_t sco_rx_buff[SCO_RX_BUFF_NUM][SCO_BUFF_SIZE];
//extern uint8_t sco_tx_buff[SCO_TX_BUFF_NUM][SCO_BUFF_SIZE];


//extern bt_ll_sco_env_t env_sco[MAX_NB_SYNC];

extern u32 sco_start_tick;
#define sco_start_tick_get()	sco_start_tick

/*** BEGIN: SCO LINK PARA ****/
typedef struct _sco_link {
    ///eSCO handle
    uint8_t esco_hdl;
    ///eSCo LT Address, Valid range is 1-7.
    uint8_t esco_lt_addr;

    ///timing control flags
    uint8_t flags;
    /// Synchronous link offset (in slots)
    uint8_t d_esco;

    /// Synchronous link interval (in slots)
    uint8_t t_esco;
    /// Synchronous retransmission window (in slots)
    uint8_t w_esco;

    ///eSCO packet type M2S
    uint8_t  m2s_pkt_type;
    ///eSCO packet type S2M
    uint8_t  s2m_pkt_type;

    ///Packet Length M2S
    uint16_t m2s_pkt_len;
    ///Packet Length S2m
    uint16_t s2m_pkt_len;

    ///Air Mode
    uint8_t air_mode;
    ///Negotiation state
    uint8_t  negt_st;

	/// Link type (0: SCO | 2: ESCO)
    uint8_t sync_type;
} __attribute__ ((__packed__)) sco_link_t;
/*** END: SCO LINK PARA ****/
extern uint8_t esco_acl_linkid;
extern uint8_t	esco_handle;
extern uint8_t	esco_error_code;
/***7.3.36/7.3.37  Synchronous Flow Control Enable  ****/
extern uint8_t syc_flw_ctr_enable;
int  lmp_send_cmd_esco_link_rmv(int Lid, uint8_t esco_hdl);
_attribute_ram_code_ btc_sco_t *bt_ll_sco_env_get(uint8_t sco_link_id);
uint8_t bt_ll_sco_active_get(void);
btc_sco_t *bt_ll_sco_env_search(uint8_t sync_hdl);

uint8_t bt_ll_sco_add(uint8_t link_id);
uint8_t bt_ll_sco_remove(uint8_t link_id, uint8_t esco_handle, uint8_t reason);

_attribute_ram_code_ void    bt_ll_sco_isr(uint8_t sco_link_id, uint8_t tog);

void bt_ll_sco_reduce_retxnb (int reduce);

int bt_ll_sco_exist(uint8_t acl_link_id);
void sco_link_tws_alloc(void);

uint8_t bt_sco_host_request(bool old_style, bool host_initiated, uint8_t link_id, uint8_t sync_id, struct lc_sco_host_params_tag *p_host_params, uint8_t new_sco);
void bt_sco_peer_request(uint8_t link_id, uint8_t req_type, struct lc_sco_air_params_tag * p_peer_params);
void lc_sco_peer_request_disc(uint8_t link_id, uint8_t scohdl, uint8_t reason, uint8_t sync_type);
void lc_sco_peer_accept(uint8_t link_id);
void lc_sco_peer_reject(uint8_t link_id, uint8_t reason);
uint8_t lc_sco_host_accept(uint8_t link_id, bool old_style, bool very_old_style, uint8_t sync_linkid, struct lc_sco_host_params_tag * p_host_params);
void lc_sco_send_peer_reject(uint8_t link_id, uint8_t reason, uint8_t sync_type, uint8_t tr_id);


uint8_t bt_ll_setup_sync_con(uint16_t conhdl,  ///Connection handle
                             uint32_t tx_bw,   ///Transmit bandwidth
                             uint32_t rx_bw,   ///Receive bandwidth
                             uint16_t max_lat, ///Max latency
                             uint16_t vx_set,  ///Voice setting
                             uint8_t retx_eff, ///Retransmission effort
                             uint16_t pkt_type ///Packet type
                             );
uint8_t bt_ll_accept_sync_con_req(uint8_t *bd_addr, uint32_t tx_bw, uint32_t rx_bw, uint16_t max_lat, uint16_t vx_set, uint8_t retx_eff, uint16_t pkt_type);
void bt_ll_sco_baseband_ack(uint8_t link_id, uint8_t lmp_opcode);
void bt_ll_sco_data_callback_register (void *prx, void *ptx);
uint8_t bt_ll_set_esco_mute(uint8_t sco_link_id, uint8_t mic_spk, uint8_t mute_en);
void bt_ll_sco_data_tx(uint8_t *p);
int bt_ll_sco_data_rx(uint8_t *p, int len);
void bt_ll_sco_free_link(uint8_t link_id, uint8_t sync_linkid, uint8_t reason);
uint8_t get_min_sync_intv(void);
uint8_t get_nb_sync_active(void);
void bt_ll_dual_sco_en(uint8_t en);
btc_sco_t* env_sco_ptr;
static inline btc_sco_t * btc_sco_get_env(uint8_t link_id)
{
	return (btc_sco_t*)(env_sco_ptr? (env_sco_ptr+link_id):0);
}

typedef int32_t (*btc_sco_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);
btc_sco_task_callback_t btc_sco_mainloop_task_callback;
btc_sco_task_callback_t btc_sco_irq_mainloop_task_callback;
typedef	void (*bt_ll_module_sco_data_tx_callback_t) (unsigned char *p);
bt_ll_module_sco_data_tx_callback_t	btc_ll_sco_data_tx_cb;
void btc_sco_register_module(btc_sco_t* env_sco);

#endif /* STACK_BT_CONTROLLER_BT_LL_SCO_H_ */
