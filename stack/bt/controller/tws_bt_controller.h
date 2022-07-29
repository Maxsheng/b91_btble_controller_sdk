/********************************************************************************************************
 * @file     tws_bt_controller.h
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

#ifndef APPLICATION_TWS_BT_TWS_BT_CONTROLLER_H_
#define APPLICATION_TWS_BT_TWS_BT_CONTROLLER_H_

#if 1//TWS_ENABLE
typedef int (* return_int_func_t)(void);

void bt_ll_arbiter_en_set(int elt_idx, uint8_t arbiter_en);
void bt_ll_observer_en_set(int elt_idx, uint8_t observer_en);

/**************BEGIN: bt_ll_tws_flag*******************/
    
#define TWS_FLAG_LINKKEY_UPDATE             (0x01UL << 0)
#define TWS_FLAG_CS_PARA_SEND               (0x01UL << 1)

#define TWS_FLAG_A2DP_CONNECTED             (0x01UL << 2)
#define TWS_FLAG_HFP_CONNECTED              (0x01UL << 3)

#define TWS_FLAG_AVRCP_CONNECTED            (0x01UL << 4)

#define TWS_FLAG_HANDOVER_SEND              (0x01UL << 5)
#define TWS_FLAG_HANDOVER_SEND_PREPARED     (0x01UL << 6)
#define TWS_FLAG_HANDOVER_BEGIN             (0x01UL << 7)

#define TWS_FLAG_TWS_EN                     (0x01UL << 8)
#define TWS_FLAG_LSTO_TPOLL_SYNC            (0x01UL << 9)
#define TWS_FLAG_AFH_MAP_SEND               (0x01UL << 10)

#define TWS_FLAG_TWS_MODE                   (0x01UL << 11)
#define HEADSET_PIN_MISSING_FLAG            (0x01UL << 12)
#define TWS_FLAG_ACL_CONNECTED              (0x01UL << 13)

extern _attribute_data_retention_sec_ unsigned int bt_ll_tws_flag;
extern void bt_ll_access_dump(void);
extern void bt_ll_acl_rxbit_offset_init(uint8_t link_id,int hus_of_sample);

static inline void bt_ll_tws_flag_set (unsigned int flag, unsigned int en)
{
	u32 r = core_disable_interrupt ();
	if (en)
	{
		bt_ll_tws_flag |= flag;
	}
	else
	{
		bt_ll_tws_flag &= ~flag;
	}
	core_restore_interrupt (r);
}

#define bt_ll_tws_flag_get(f)                      (bt_ll_tws_flag&(f))

/**************END: bt_ll_tws_flag*******************/

/***********************************************************************
BEGIN: re-build acl link.
***********************************************************************/
typedef struct _afh_map_sync{
//Part1. afh map already take effected.
	uint32_t afh_instant_curr;
	uint8_t afh_map_curr[CHNL_MAP_LEN];		
    
//Part2. afh map take effect in the future.
	uint32_t afh_instant_future;
	uint8_t afh_map_future[CHNL_MAP_LEN];			
}__attribute__ ((__packed__)) afh_map_sync_t;

typedef struct _acl_link {
/// ACL LINK
    uint8_t acl_link_id : 4;     			// ACL Link identifier: should be 0
    uint8_t lt_addr : 4;

	uint8_t bdaddr[BD_ADDR_LEN];			//
	uint8_t dev_class[DEV_CLASS_LEN];  		// device of class

	// offset: 10
	uint8_t sp_linkkey[16];
	uint8_t sp_cfm[16];

    uint8_t ptt : 3;                        // packet type table
    uint8_t role : 1;                       // local piconet role
    uint8_t scan_repe : 4;                  // scan repetition


    /*** BEGIN: HOST STACK PARA: offset-56 ****/
    
    // A2DP - alloc a2dp-snk from a2dp-src.    
    // A2DP - alloc a2dp-snk from a2dp-snk.    

    // offset: 43
    uint8_t     avdtp_seid_used;

    uint16_t    avdtp_sig_scid;
    uint16_t    avdtp_sig_dcid;

    uint16_t    avdtp_media_scid;
    uint16_t    avdtp_media_dcid;


    // AVRCP
    uint16_t    avctp_scid;
    uint16_t    avctp_dcid;

    // HFP
    uint16_t    hf_rfcomm_scid;
    uint16_t    hf_rfcomm_dcid;
	
    // offset: 60
    uint8_t     hf_rfcomm_chn; 
    uint8_t     hf_rfcomm_dlc_addr; 
    uint8_t     hf_rfcomm_tx_credits;
    uint8_t     hf_rfcomm_rx_credits;
	
	uint16_t 	hf_idxCall : 4;
	uint16_t 	hf_idxService : 4;
	uint16_t 	hf_idxCallSetup : 4;
	uint16_t 	hf_idxCallHeld : 4;

    // SPP
    uint16_t    spp_scid;
    uint16_t    spp_dcid;

    /*** END: HOST STACK PARA ****/


/// ESCO LINK
    sco_link_t esco_link;
	
    uint8_t hf_siri_support : 1;	// hfp-hf siri_support.
    uint8_t hfp_hf_codec : 2;		// hfp-hf codec, cvsd or msbc.
    uint8_t hfp_hf_initiator : 1; 	// hfp-hf rfcomm_session_initiator.
    uint8_t afh_mode : 1;       	// 1: afh enable, 0: afh disable. 
	uint8_t voice_recog_support : 1;	/* Voice Recognition */
    uint8_t rsvd : 2;       		// 
	
    uint8_t hfp_hf_vgs : 4;       	// 0~0x0f, hfp-hf spk volume.
    uint8_t hfp_hf_vgm : 4;       	// 0~0x0f, hfp-hf mic volume.
} __attribute__ ((__packed__)) acl_link_t;

extern _attribute_data_retention_sec_ afh_map_sync_t afh_map_tws;

extern _attribute_data_retention_sec_ acl_link_t acl_link_tws;

enum {
	HANDOVER_ST_IDLE	  = 0,
    HANDOVER_ST_START     = 1,
    HANDOVER_ST_BEGIN     = 2,
    HANDOVER_ST_END       = 3,
};

typedef struct _tws_handover_cmd {
    uint32_t handover_instant;  // half-slot
	uint8_t ho_state;
	
	uint32_t hf_state : 4;
	uint32_t hf_statusCall : 4;
	uint32_t hf_statusCallSetup : 4;
	uint32_t hf_statusCallHeld : 4;
	
	uint32_t avrcp_state : 4;
	uint32_t siri_support : 1;
	uint32_t siri_enabled : 1;
	uint32_t voice_recog_support : 1;	/* Voice Recognition */
	uint32_t rsvd : 1;
	
	uint32_t hf_at_cmd_pending : 8;

	uint16_t hf_idxCall : 4;
	uint16_t hf_idxService : 4;
	uint16_t hf_idxCallSetup : 4;
	uint16_t hf_idxCallHeld : 4;
} __attribute__ ((__packed__)) tws_handover_cmd_t;

extern _attribute_data_retention_sec_ tws_handover_cmd_t tws_handover_cmd;

void acl_link_build_afh(uint32_t Lid);
void acl_link_tws_alloc(bt_ll_tick_clkn_fcnt_t *ptcf);
void acl_link_tws_destroy(void);
int  acl_link_tws_send(void);
int  acl_link_afh_map_tws_send(void);
void acl_link_tws_bt_token_init(void);
void acl_link_tws_dump(void);
void acl_link_tws_cs_para_check(void);

#define acl_link_tws_handover_launch()          bt_ll_tws_flag_set(TWS_FLAG_HANDOVER_SEND, 1)
void  acl_link_tws_handover_send_prepare(void);
int  acl_link_tws_handover_send(void);
int  acl_link_tws_handover_received(uint8_t *p, int len);
void acl_link_tws_handover (void);
int acl_link_tws_lsto_tpoll_sync(uint8_t acl_link_id);
void tws_handover_app_cb_register(return_int_func_t f);

#endif

extern _attribute_data_retention_sec_ return_int_func_t tws_is_master_cb;
extern _attribute_data_retention_sec_ return_int_func_t tws_is_slave_cb;
void tws_is_master_slave_func_register(return_int_func_t m_f, return_int_func_t s_f);

typedef int (* tsync_get_status_flag_func_t)(unsigned char flag);
extern _attribute_data_retention_sec_ tsync_get_status_flag_func_t tsync_get_status_flag_cb;
void tsync_get_status_flag_func_register(tsync_get_status_flag_func_t f);

typedef int (* tsync_tws_conn_func_t)(void);
extern _attribute_data_retention_sec_ tsync_tws_conn_func_t tsync_tws_connect_cb;
extern _attribute_data_retention_sec_ tsync_tws_conn_func_t tsync_tws_disconn_cb;
void tsync_tws_conn_func_register(tsync_tws_conn_func_t f_conn, tsync_tws_conn_func_t f_disconn);

typedef void (* tws_ble_disconnect_func_t)(void);
extern _attribute_data_retention_sec_ tws_ble_disconnect_func_t tws_ble_disconnect_cb;
void tws_ble_disconnect_func_register(tws_ble_disconnect_func_t f);

void acl_link_tws_init(void);

#endif /* APPLICATION_TWS_BT_TWS_BT_CONTROLLER_H_ */
