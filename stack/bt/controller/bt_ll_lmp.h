/********************************************************************************************************
 * @file     bt_ll_lmp.h
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

#ifndef _BT_LL_LMP_
#define _BT_LL_LMP_

//LMP TID
#define LMP_TID_0                          0
#define LMP_TID_1                          0x01
#define LMP_OPCODE_TID_CONSTRUCT(opcode, tid) (((opcode)<<1)|(tid))
#define LMP_TO_US                          5000000
/// Position of transaction ID in 1st byte
#define LMP_TR_ID_POS        0
#define LMP_TR_ID_MASK       0x01
/// Position of opcode in 1st byte
#define LMP_OPCODE_POS       1
#define LMP_OPCODE_MASK      0xFE

//LMP Opcodes
#define LMP_NAME_REQ_OPCODE                1       // LMP_name_req                    1
#define LMP_NAME_RES_OPCODE                2       // LMP_name_res                    2
#define LMP_ACCEPTED_OPCODE                3       // LMP_accepted                    3
#define LMP_NOT_ACCEPTED_OPCODE            4       // LMP_not_accepted                4
#define LMP_CLK_OFF_REQ_OPCODE             5       // LMP_clkoffset_req               5
#define LMP_CLK_OFF_RES_OPCODE             6       // LMP_clkoffset_res               6
#define LMP_DETACH_OPCODE                  7       // LMP_detach                      7
#define LMP_INRAND_OPCODE                  8       // LMP_in_rand                     8
#define LMP_COMBKEY_OPCODE                 9       // LMP_comb_key                    9
#define LMP_UNITKEY_OPCODE                10       // LMP_unit_key                    10
#define LMP_AURAND_OPCODE                 11       // LMP_au_rand                     11
#define LMP_SRES_OPCODE                   12       // LMP_sres                        12
#define LMP_TEMPRAND_OPCODE               13       // LMP_temp_rand                   13
#define LMP_TEMPKEY_OPCODE                14       // LMP_temp_key                    14
#define LMP_ENC_MODE_REQ_OPCODE           15       // LMP_encryption_mode_req         15
#define LMP_ENC_KEY_SIZE_OPCODE           16       // LMP_encryption_key_size_req     16
#define LMP_START_ENC_REQ_OPCODE          17       // LMP_start_encryption_req        17
#define LMP_STOP_ENC_REQ_OPCODE           18       // LMP_stop_encryption_req         18
#define LMP_SWITCH_REQ_OPCODE             19       // LMP_switch_req                  19
#define LMP_HOLD_OPCODE                   20       // LMP_hold                        20
#define LMP_HOLD_REQ_OPCODE               21       // LMP_hold_req                    21
#define LMP_SNIFF_REQ_OPCODE              23       // LMP_sniff                       23
#define LMP_UNSNIFF_REQ_OPCODE            24       // LMP_unsniff_req                 24
#define LMP_PARK_REQ_OPCODE               25       // LMP_park_req                    25
#define LMP_SET_BSWIN_OPCODE              27       // LMP_set_broadcast_scan_window   27
#define LMP_MODIF_BEACON_OPCODE           28       // LMP_modify_beacon               28
#define LMP_UNPARK_BD_REQ_OPCODE          29       // LMP_unpark_BD_ADDR_req          29
#define LMP_UNPARK_PM_REQ_OPCODE          30       // LMP_unpark_PM_ADDR_req          30
#define LMP_INCR_PW_REQ_OPCODE            31       // LMP_incr_power_req              31
#define LMP_DECR_PW_REQ_OPCODE            32       // LMP_decr_power_req              32
#define LMP_MAX_PW_OPCODE                 33       // LMP_max_power                   33
#define LMP_MIN_PW_OPCODE                 34       // LMP_min_power                   34
#define LMP_AUTO_RATE_OPCODE              35       // LMP_auto_rate                   35
#define LMP_PREF_RATE_OPCODE              36       // LMP_preferred_rate              36
#define LMP_VER_REQ_OPCODE                37       // LMP_version_req                 37
#define LMP_VER_RES_OPCODE                38       // LMP_version_res                 38
#define LMP_FEATS_REQ_OPCODE              39       // LMP_features_req                39
#define LMP_FEATS_RES_OPCODE              40       // LMP_features_res                40
#define LMP_QOS_OPCODE                    41       // LMP_quality_of_service          41
#define LMP_QOS_REQ_OPCODE                42       // LMP_quality_of_service_req      42
#define LMP_SCO_LINK_REQ_OPCODE           43       // LMP_sco_link_req                43
#define LMP_RMV_SCO_LINK_REQ_OPCODE       44       // LMP_remove_sco_link_req         44
#define LMP_MAX_SLOT_OPCODE               45       // LMP_max_slot                    45
#define LMP_MAX_SLOT_REQ_OPCODE           46       // LMP_max_slot_req                46
#define LMP_TMG_ACCU_REQ_OPCODE           47       // LMP_timing_accuracy_req         47
#define LMP_TMG_ACCU_RES_OPCODE           48       // LMP_timing_accuracy_res         48
#define LMP_SETUP_CMP_OPCODE              49       // LMP_setup_complete              49
#define LMP_USE_SEMIPERM_KEY_OPCODE       50       // LMP_use_semi_permanent_key      50
#define LMP_HOST_CON_REQ_OPCODE           51       // LMP_host_connection_req         51
#define LMP_SLOT_OFFSET_OPCODE            52       // LMP_slot_offset                 52
#define LMP_PAGE_MODE_REQ_OPCODE          53       // LMP_page_mode_req               53
#define LMP_PAGE_SCAN_MODE_REQ_OPCODE     54       // LMP_page_scan_mode_req          54
#define LMP_SUP_TO_OPCODE                 55       // LMP_supervision_timeout         55
#define LMP_TEST_ACTIV_OPCODE             56       // LMP_test_activate               56
#define LMP_TEST_CNTL_OPCODE              57       // LMP_test_control                57
#define LMP_ENC_KSZ_MSK_REQ_OPCODE        58       // LMP_encryption_key_size_mask_req 58
#define LMP_ENC_KSZ_MSK_RES_OPCODE        59       // LMP_encryption_key_size_mask_res 59
#define LMP_SET_AFH_OPCODE                60       // LMP_set_AFH                     60
#define LMP_ENCAPS_HDR_OPCODE             61       // LMP_encapsulatedheader          61
#define LMP_ENCAPS_PAYL_OPCODE            62       // LMP_encapsulatedpayload         62
#define LMP_SP_CFM_OPCODE                 63       // LMP_simplepairingconfirm        63
#define LMP_SP_NB_OPCODE                  64       // LMP_simplepairingnumber         64
#define LMP_DHKEY_CHK_OPCODE              65       // LMP_DHkeycheck                  65


#define LMP_ESC1                          124
#define LMP_ESC2                          125
#define LMP_ESC3                          126
#define LMP_ESC4                          127

///LMP Escape 4 Extended Opcodes
#define LMP_ACCEPTED_EXTOPCODE             1       //  LMP_accepted_ext                1
#define LMP_NOT_ACCEPTED_EXTOPCODE         2       //  LMP_not_accepted_ext            2
#define LMP_FEATS_REQ_EXTOPCODE            3       //  LMP_features_req_ext            3
#define LMP_FEATS_RES_EXTOPCODE            4       //  LMP_features_res_ext            4
#define LMP_CLK_ADJ_EXTOPCODE              5
#define LMP_CLK_ADJ_ACK_EXTOPCODE          6
#define LMP_CLK_ADJ_REQ_EXTOPCODE          7
#define LMP_PKT_TYPE_TBL_REQ_EXTOPCODE    11       //  LMP_packet_type_table_req       11
#define LMP_ESCO_LINK_REQ_EXTOPCODE       12       //  LMP_esco_link_req               12
#define LMP_RMV_ESCO_LINK_REQ_EXTOPCODE   13       //  LMP_remove_esco_link_req        13
#define LMP_CH_CLS_REQ_EXTOPCODE          16       //  LMP_channel_classification_req  16
#define LMP_CH_CLS_EXTOPCODE              17       //  LMP_channel_classification      17
#define LMP_SNIFF_SUBR_REQ_EXTOPCODE      21       //  LMP_sniff_subrating_req         21
#define LMP_SNIFF_SUBR_RES_EXTOPCODE      22       //  LMP_sniff_subrating_res         22
#define LMP_PAUSE_ENC_REQ_EXTOPCODE       23       //  LMP_pause_encryption_req        23
#define LMP_RESUME_ENC_REQ_EXTOPCODE      24       //  LMP_resume_encryption_req       24
#define LMP_IOCAP_REQ_EXTOPCODE           25       //  LMP_iocapabilityreq             25
#define LMP_IOCAP_RES_EXTOPCODE           26       //  LMP_iocapabilityres             26
#define LMP_NUM_CMP_FAIL_EXTOPCODE        27       //  LMP_numericcomparisonfailed     27
#define LMP_PASSKEY_FAIL_EXTOPCODE        28       //  LMP_passkeyfailed               28
#define LMP_OOB_FAIL_EXTOPCODE            29       //  LMP_OOBfailed                   29
#define LMP_KEYPRESS_NOTIF_EXTOPCODE      30       //  LMP_keypressnotification        30
#define LMP_PW_CNTL_REQ_EXTOPCODE         31       //  LMP_pw_cntl_req
#define LMP_PW_CNTL_RES_EXTOPCODE         32       //  LMP_pw_cntl_res
#define LMP_PING_REQ_EXTOPCODE            33
#define LMP_PING_RES_EXTOPCODE            34
#define LMP_SAM_SET_TYPE0_EXTOPCODE       35
#define LMP_SAM_DEFINE_MAP_EXTOPCODE      36
#define LMP_SAM_SWITCH_EXTOPCODE          37
#define LMP_OFFSET_REQ_EXTOPCODE          62       //  LMP_offset_req                  50

//Table sizes
#define LMP_MAX_PK                        63
#define LMP_EXT_MAX_PK                    21
#define LMP_MAX_UNPK                      66
#define LMP_EXT_MAX_UNPK                  33

#define LMP_SIGNAL_BUFFER_SIZE_MAX  (LMP_MAX_PK + LMP_EXT_MAX_PK)

//PDU lengths (including opcode)
#define LMP_NAME_REQ_LEN                    2
#define LMP_NAME_RES_LEN                    17
#define LMP_ACCEPTED_LEN                    2
#define LMP_NOT_ACCEPTED_LEN                3
#define LMP_CLK_OFF_REQ_LEN                 1
#define LMP_CLK_OFF_RES_LEN                 3
#define LMP_DETACH_LEN                      2
#define LMP_INRAND_LEN                      17
#define LMP_COMBKEY_LEN                     17
#define LMP_UNITKEY_LEN                     17
#define LMP_AURAND_LEN                      17
#define LMP_SRES_LEN                        5
#define LMP_TEMPRAND_LEN                    17
#define LMP_TEMPKEY_LEN                     17
#define LMP_ENC_MODE_REQ_LEN                2
#define LMP_ENC_KEY_SIZE_LEN                2
#define LMP_START_ENC_REQ_LEN               17
#define LMP_STOP_ENC_REQ_LEN                1
#define LMP_SWITCH_REQ_LEN                  5
#define LMP_HOLD_LEN                        7
#define LMP_HOLD_REQ_LEN                    7
#define LMP_SNIFF_REQ_LEN                   10
#define LMP_UNSNIFF_REQ_LEN                 1
#define LMP_PARK_REQ_LEN                    17
#define LMP_INCR_PW_REQ_LEN                 2
#define LMP_DECR_PW_REQ_LEN                 2
#define LMP_MAX_PW_LEN                      1
#define LMP_MIN_PW_LEN                      1
#define LMP_AUTO_RATE_LEN                   1
#define LMP_PREF_RATE_LEN                   2
#define LMP_VER_REQ_LEN                     6
#define LMP_VER_RES_LEN                     6
#define LMP_FEATS_REQ_LEN                   9
#define LMP_FEATS_RES_LEN                   9
#define LMP_QOS_LEN                         4
#define LMP_QOS_REQ_LEN                     4
#define LMP_SCO_LINK_REQ_LEN                7
#define LMP_RMV_SCO_LINK_REQ_LEN            3
#define LMP_MAX_SLOT_LEN                    2
#define LMP_MAX_SLOT_REQ_LEN                2
#define LMP_TMG_ACCU_REQ_LEN                1
#define LMP_TMG_ACCU_RES_LEN                3
#define LMP_SETUP_CMP_LEN                   1
#define LMP_USE_SEMIPERM_KEY_LEN            1
#define LMP_HOST_CON_REQ_LEN                1
#define LMP_SLOT_OFFSET_LEN                 9
#define LMP_PAGE_MODE_REQ_LEN               3
#define LMP_PAGE_SCAN_MODE_REQ_LEN          3
#define LMP_SUP_TO_LEN                      3
#define LMP_TEST_ACTIV_LEN                  1
#define LMP_TEST_CNTL_LEN                   10
#define LMP_ENC_KSZ_MSK_REQ_LEN             1
#define LMP_ENC_KSZ_MSK_RES_LEN             3
#define LMP_SET_AFH_LEN                     16
#define LMP_ENCAPS_HDR_LEN                  4
#define LMP_ENCAPS_PAYL_LEN                 17
#define LMP_SP_CFM_LEN                      17
#define LMP_SP_NB_LEN                       17
#define LMP_DHKEY_CHK_LEN                   17
#define  LMP_PAUSE_ENC_AES_REQ_LEN          17,

///LMP Escape 4 Extended Opcodes
#define LMP_ACCEPTED_EXT_LEN                4
#define LMP_NOT_ACCEPTED_EXT_LEN            5
#define LMP_FEATS_REQ_EXT_LEN               12
#define LMP_FEATS_RES_EXT_LEN               12
#define LMP_CLK_ADJ_LEN                     15
#define LMP_CLK_ADJ_ACK_LEN                 3 
#define LMP_CLK_ADJ_REQ_LEN                 6 
#define LMP_PKT_TYPE_TBL_REQ_EXT_LEN        3
#define LMP_ESCO_LINK_REQ_EXT_LEN           16
#define LMP_RMV_ESCO_LINK_REQ_EXT_LEN       4
#define LMP_CH_CLS_REQ_EXT_LEN              7
#define LMP_CH_CLS_EXT_LEN                  12
#define LMP_SNIFF_SUBR_REQ_EXT_LEN          9
#define LMP_SNIFF_SUBR_RES_EXT_LEN          9
#define LMP_PAUSE_ENC_REQ_EXT_LEN           2
#define LMP_RESUME_ENC_REQ_EXT_LEN          2
#define LMP_IOCAP_REQ_EXT_LEN               5
#define LMP_IOCAP_RES_EXT_LEN               5
#define LMP_NUM_CMP_FAIL_EXT_LEN            2
#define LMP_PASSKEY_FAIL_EXT_LEN            2
#define LMP_OOB_FAIL_EXT_LEN                2
#define LMP_KEYPRESS_NOTIF_EXT_LEN          3
#define LMP_PW_CNTL_REQ_EXT_LEN             3
#define LMP_PW_CNTL_RES_EXT_LEN             3
#define LMP_PING_REQ_LEN                    2 
#define LMP_PING_RES_LEN                    2 
#define LMP_SAM_SET_TYPE0_LEN               17
#define LMP_SAM_DEFINE_MAP_LEN              17
#define LMP_SAN_SWITCH_LEN                  9
//#define LMP_OFFSET_REQ_EXT_LEN            ///TBD wrt specs. length of the LMP for Wimax


/*
 * TYPE DEFINITONS
 ****************************************************************************************
 */
///LMP PDU unpack function pointer type definition
typedef void (*lmp_pdu_unpk_func_t)(uint8_t * pdu, uint8_t parlen, uint8_t * param);

///LMP PDU pack function pointer type definition
typedef int  (*lmp_pdu_pk_func_t)(struct ke_msg * msg);


/*
 * STRUCTURES
 ****************************************************************************************
 */

///LMP msg
typedef struct _lmp_data {
    ///Link identifier
    uint8_t  link_id;
    ///length of the LMP PDU
    uint8_t  len;
    ///buffer of the LMP PDU
    uint8_t  pdu[30];
} lmp_data_t;

///LMP tx cfm event
typedef struct _lmp_tx_cfm_event {
    ///Link identifier
    uint8_t  link_id;
    ///pdu
    uint8_t  pdu[7];
} lmp_tx_cfm_event_t;

///LMP PDU packing details structure
struct lmp_pdu_pk_util
{
    ///PDU opcode
    uint8_t opcode;
    ///PDU length
    uint8_t len;
    ///PDU packing handler
    lmp_pdu_pk_func_t func;
};

///LMP_name_req PDU structure
struct lmp_name_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Offset
    uint8_t  offset;
} ;

///LMP_name_res PDU structure
struct lmp_name_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Offset
    uint8_t  offset;
    ///Name Length
    uint8_t  length;
    ///Name Fragment
    struct name_vect name_frag;
} ;

///LMP_accepted PDU structure
struct lmp_accepted_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Opcode of accepted PDU
    uint8_t  opcode;
} ;

///LMP_not_accepted PDU structure
struct lmp_not_accepted_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    //Opcode of un-accepted PDU
    uint8_t  opcode;
    ///Reason for not accepting the PDU (error code)
    uint8_t  reason;
} ;

///LMP_clkoffset_req PDU structure
struct lmp_clk_off_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_clkoffset_res PDU structure
struct lmp_clk_off_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Clock Offset value
    uint16_t clk_offset ;
} ;

///LMP_detach PDU structure
struct lmp_detach_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Reason to detach
    uint8_t  reason;
} ;

///LMP_in_rand PDU structure
struct lmp_inrand_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Random number
    struct ltk random;
} ;

///LMP_comb_key PDU structure
struct lmp_combkey_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Random number
    struct ltk random;
} ;

///LMP_unit_key PDU structure
struct lmp_unitkey_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Key
    struct ltk key;
} ;

///LMP_au_rand PDU structure
struct lmp_aurand_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Random number
    struct ltk random;
} ;

///LMP_sres PDU structure
struct lmp_sres_pdu
{
    ///Link identifier
    uint8_t        link_id;
    ///Transaction ID
    uint8_t        tr_id;
    ///Authentication Response
    struct sres_nb Sres;
} ;

///LMP_temp_rand PDU structure
struct lmp_temprand_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Random number
    struct ltk random;
} ;

///LMP_temp_key PDU structure
struct lmp_tempkey_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Key
    struct ltk key;
} ;

///LMP_encryption_mode_req PDU structure
struct lmp_enc_mode_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Encryption Mode
    uint8_t  enc_mode;
} ;

///LMP_encryption_key_size_req PDU structure
struct lmp_enc_key_size_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Key Size
    uint8_t  key_size;
} ;

///LMP_start_encryption_req PDU structure
struct lmp_start_enc_req_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Random number
    struct ltk random;
};

///LMP_stop_encryption_req PDU structure
struct lmp_stop_enc_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
};

///LMP_switch_req PDU structure
struct lmp_switch_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Switch Instant
    uint32_t switch_inst;
};

#if HOLD_SUPPORT
///LMP_hold PDU structure
struct lmp_hold_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Hold Time
    uint16_t hold_time;
    ///Hold Instant
    uint32_t hold_inst;
} ;

///LMP_hold_req PDU structure
struct lmp_hold_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Hold Time
    uint16_t hold_time;
    ///Hold Instant
    uint32_t hold_inst;
};
#endif

///LMP_sniff_req PDU structure
struct lmp_sniff_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Timing Control Flags
    uint8_t  flags;
    ///Dsniff
    uint16_t d_sniff;
    ///Tsniff
    uint16_t t_sniff;
    ///Sniff attempt
    uint16_t sniff_attempt;
    ///Sniff Timeout
    uint16_t sniff_to;
};

///LMP_unsniff_req PDU structure
struct lmp_unsniff_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_set_broadcast_scan_window PDU structure
struct lmp_set_bdcst_scan_win_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Timing control flags
    uint8_t  flags;
    ///Db
    uint16_t db;
    ///Broadcast Scan Window
    uint16_t bswin;
} ;

///LMP_modify_beacon PDU structure
struct lmp_modif_beacon_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///timing control Flags
    uint8_t  flags;
    ///Db
    uint16_t db;
    ///Tb
    uint16_t tb;
    ///Nb
    uint8_t  nb;
    ///Delta b
    uint8_t  deltab;
    ///Daccess
    uint8_t  daccess;
    ///Taccess
    uint8_t  taccess;
    ///Naccess-slots
    uint8_t  nacc_slots;
    ///Npoll
    uint8_t  npoll;
    ///Access
    uint8_t  access;
};

///LMP_incr_power_req PDU structure
struct lmp_incr_pw_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///For future use
    uint8_t  res;
} ;

///LMP_decr_power_req PDU structure
struct lmp_decr_pw_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///For future use
    uint8_t  res;
} ;

///LMP_max_power PDU structure
struct lmp_max_pw_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_min_power PDU structure
struct lmp_min_pw_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_power_cntl_req PDU structure
struct lmp_pw_cntl_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Power adjustment request
    uint8_t pw_adj;

} ;

///LMP_power_cntl_res PDU structure
struct lmp_pw_cntl_res_pdu
{
    ///Link identifier
    uint8_t link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Power adjustment request
    uint8_t pw_adj;
} ;

///LMP_auto_rate PDU structure
struct lmp_auto_rate_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_preferred_rate PDU structure
struct lmp_pref_rate_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Data Rate
    uint8_t  rate;
} ;

///LMP_version_req PDU structure
struct lmp_ver_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Version number
    uint8_t  ver;
    ///Company ID
    uint16_t co_id;
    ///Subversion number
    uint16_t subver;
} ;

///LMP_version_res PDU structure
struct lmp_ver_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Version number
    uint8_t  ver;
    ///Company ID
    uint16_t co_id;
    ///Subversion number
    uint16_t subver;
} ;

///LMP_features_req PDU structure
struct lmp_feats_req_pdu
{
    ///Link identifier
    uint8_t         link_id;
    ///Transaction ID
    uint8_t         tr_id;
    ///Features
    struct features feats;
} ;

///LMP_features_res PDU structure
struct lmp_feats_res_pdu
{
    uint8_t         link_id;
    ///Transaction ID
    uint8_t         tr_id;
    ///Features
    struct features feats;
} ;

///LMP_quality_of_service PDU structure
struct lmp_qos_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Poll interval
    uint16_t poll_intv;
    ///Nbc
    uint8_t  nbc;
} ;


///LMP_quality_of_service_req PDU structure
struct lmp_qos_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Poll interval
    uint16_t poll_intv;
    ///Nbc
    uint8_t  nbc;
} ;

///LMP_SCO_link_req PDU structure
struct lmp_sco_link_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///SCO handle
    uint8_t  sco_hdl;
    ///timing control flags
    uint8_t  flags;
    ///Dsco
    uint8_t  d_sco;
    ///Tsco
    uint8_t  t_sco;
    ///SCO packet
    uint8_t  sco_pkt;
    ///Air mode
    uint8_t  air_mode;
} ;

///LMP_remove_SCO_link_req PDU structure
struct lmp_rmv_sco_link_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///SCO handle
    uint8_t  sco_hdl;
    ///Reason for its removal(error code)
    uint8_t  reason;
} ;

///LMP_max_slot PDU structure
struct lmp_max_slot_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Max slots
    uint8_t  max_slots;
} ;


///LMP_max_slot_req PDU structure
struct lmp_max_slot_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Max slots
    uint8_t  max_slots;
} ;

///LMP_timing_accuracy_req PDU structure
struct lmp_tmg_accu_req_pdu
{
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_timing_accuracy_res PDU structure
struct lmp_tmg_accu_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Drift
    uint8_t  drift;
    ///Jitter
    uint8_t  jitter;
} ;

///LMP_setup_complete PDU structure
struct lmp_setup_cmp_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_use_semi_permanent_key PDU structure
struct lmp_use_semi_perm_key_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_host_connection_req PDU structure
struct lmp_host_con_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_slot_offset PDU structure
struct lmp_slot_offset_pdu
{
    ///Link identifier
    uint8_t        link_id;
    ///Transaction ID
    uint8_t        tr_id;
    ///Slot Offset
    uint16_t       slot_off;
    ///BD Address
    struct bd_addr addr;
} ;

///LMP_page_mode_req PDU structure
struct lmp_page_mode_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Paging scheme
    uint8_t  page_scheme;
    ///Paging scheme settings
    uint8_t  page_stgs;
} ;

///LMP_page_scan_mode_req PDU structure
struct lmp_page_scan_mode_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Paging scheme
    uint8_t  page_scheme;
    ///Paging scheme settings
    uint8_t  page_stgs;
} ;

///LMP_supervision_timeout PDU structure
struct lmp_sup_to_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Supervision Timeout
    uint16_t sup_to;
} ;

///LMP_test_activate PDU structure
struct lmp_test_activate_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_test_control PDU structure
struct lmp_test_cntl_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Test Scenario
    uint8_t  scenario;
    ///Hopping Mode
    uint8_t  hop;
    ///Tx frequency
    uint8_t  tx_freq;
    ///Rx Frequency
    uint8_t  rx_freq;
    ///Power Control Mode
    uint8_t  pw_cntl;
    ///Poll period
    uint8_t  poll;
    ///Packet type
    uint8_t  pkt_type;
    ///length of test data
    uint16_t datalen;
} ;

///LMP_encryption_key_size_mask_req PDU structure
struct lmp_enc_key_size_msk_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_encryption_key_size_mask_res PDU structure
struct lmp_enc_key_size_msk_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Encryption Key Size Mask
    uint16_t mask;
} ;

///LMP_set_AFH PDU structure
struct lmp_set_afh_pdu
{
    ///Link identifier
    uint8_t         link_id;
    ///Transaction ID
    uint8_t         tr_id;
    ///AFH Instant
    uint32_t        instant;
    ///AFH Mode
    uint8_t         mode;
    ///AFH channel map
    struct chnl_map map;
} ;

///LMP_encapsulated_header PDU structure
struct lmp_encaps_hdr_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Encapsulated major type
    uint8_t  maj_type;
    ///Encapsulated minor type
    uint8_t  min_type;
    ///Encapsulated Payload Length
    uint8_t  payl_len;
} ;

///LMP_encapsulated_payload PDU structure
struct lmp_encaps_payl_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Encapsulated data
    struct byte16 data;
} ;

///LMP_Simple_Pairing_Confirm PDU structure
struct lmp_sp_cfm_pdu
{
    ///Link identifier
    uint8_t    link_id;
    ///Transaction ID
    uint8_t    tr_id;
    ///Commitment Value
    struct byte16 commitment_val ;
} ;

///LMP_Simple_Pairing_Number PDU structure
struct lmp_sp_nb_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///nonce Value
    struct byte16 nonce;
} ;

///LMP_DHkey_check PDU structure
struct lmp_dhkey_chk_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Confirmation Value
    struct ltk cfm_val;
} ;

/*
 * Extended PDUs parameter structures - Escape 4
 ****************************************************************************************/

///LMP_accepted_ext PDU structure
struct lmp_accepted_ext_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Escape opcode
    uint8_t  esc_opcode;
    ///Extended opcode
    uint8_t  ext_opcode;
} ;

///LMP_not_accepted_ext PDU structure
struct lmp_not_accepted_ext_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Escape opcode
    uint8_t  esc_opcode;
    ///Extended opcode
    uint8_t  ext_opcode;
    ///Reason
    uint8_t  reason;
} ;

///LMP_features_req_ext PDU structure
struct lmp_feats_req_ext_pdu
{
    ///Link identifier
    uint8_t        link_id;
    ///Transaction ID
    uint8_t        tr_id;
    ///Features page
    uint8_t        page;
    ///Max supported page
    uint8_t        max_page;
    ///Extended features
    struct features ext_feats;
} ;

///LMP_features_res_ext PDU structure
struct lmp_feats_res_ext_pdu
{
    ///Link identifier
    uint8_t        link_id;
    ///Transaction ID
    uint8_t        tr_id;
    ///Features page
    uint8_t        page;
    ///Max supported page
    uint8_t        max_page;
    ///Extended features
    struct features ext_feats;
} ;

///LMP_packet_type_table_req PDU structure
struct lmp_pkt_type_tbl_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Packet Type table
    uint8_t  pkt_type_tbl;
} ;

///LMP_eSCO_link_req PDU structure
struct lmp_esco_link_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///eSCO handle
    uint8_t  esco_hdl;
    ///eSCo LT Address
    uint8_t  esco_lt_addr;
    ///timing control flags
    uint8_t  flags;
    ///Desco
    uint8_t  d_esco;
    ///t_esco
    uint8_t  t_esco;
    ///Wesco
    uint8_t  w_esco;
    ///eSCO packet type M2S
    uint8_t  m2s_pkt_type;
    ///eSCO packet type S2M
    uint8_t  s2m_pkt_type;
    ///Packet Length M2S
    uint16_t m2s_pkt_len;
    ///Packet Length S2m
    uint16_t s2m_pkt_len;
    ///Air Mode
    uint8_t  air_mode;
    ///Negotiation state
    uint8_t  negt_st;
} ;

///LMP_remove_eSCO_link_req PDU structure
struct lmp_rmv_esco_link_req
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///eSCO handle
    uint8_t  esco_hdl;
    ///Reason
    uint8_t  reason;
} ;

///LMP_channel_classification_req PDU structure
struct lmp_ch_cls_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///AFH reporting mode
    uint8_t  rep_mode;
    ///AFH min interval
    uint16_t min_intv;
    ///AFH max interval
    uint16_t max_intv;
} ;

///LMP_channel_classification PDU structure
struct lmp_ch_cls_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///AFh channel classification
    struct chnl_map ch_cls;
} ;

///LMP_sniff_subrating_req PDU structure
struct lmp_sniff_subr_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Maximum sniff sub-rate
    uint8_t  max_subrate;
    ///Minimum sniff mode timeout
    uint16_t min_to;
    ///Sniff sub-rating instant
    uint32_t inst;
} ;

///LMP_sniff_subrating_res PDU structure
struct lmp_sniff_subr_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Maximum sniff sub-rate
    uint8_t  max_subrate;
    ///Minimum sniff mode timeout
    uint16_t min_to;
    ///Sniff sub-rating instant
    uint32_t inst;
} ;

///LMP_pause_encryption_req PDU structure
struct lmp_pause_enc_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_resume_encryption_req PDU structure
struct lmp_resume_enc_req_pdu
{
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_IO_capability_req PDU structure
struct lmp_iocap_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///IO Capabilities
    uint8_t  io_cap;
    ///OBB Data present
    uint8_t  oob_flag;
    ///Authentication requirements
    uint8_t  auth_req;
} ;


///LMP_IO_capability_res PDU structure
struct lmp_iocap_res_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///IO Capabilities
    uint8_t  io_cap;
    ///OBB Data present
    uint8_t  oob_flag;
    ///Authentication requirements
    uint8_t  auth_req;
} ;

///LMP_numeric_comparison_failed PDU structure
struct lmp_num_compare_fail_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_passkey_failed PDU structure
struct lmp_passkey_fail_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_oob_failed PDU structure
struct lmp_oob_fail_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
} ;

///LMP_keypress_notification PDU structure
struct lmp_keypress_notif_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Notification Type
    uint8_t  type;
} ;

///LMP_offset PDU structure
struct lmp_offset_req_pdu
{
    ///Link identifier
    uint8_t  link_id;
    ///Transaction ID
    uint8_t  tr_id;
    ///Offset value
    uint16_t  OffsetValue;
    ///Instant value
    uint32_t  RendezvousValue;
};

/*
When in Basic Rate mode:
bit0 = 0: use FEC
bit0 = 1: do not use FEC
bit1-2=0: No packet-size preference available
bit1-2=1: use 1-slot packets
bit1-2=2: use 3-slot packets
bit1-2=3: use 5-slot packets

When in Enhanced Data Rate mode:
bit3-4=0: use DM1 packets
bit3-4=1: use 2 Mb/s packets
bit3-4=2: use 3 Mb/s packets
bit3-4=3: reserved for future use
bit5-6=0: No packet-size preference available
bit5-6=1: use 1-slot packets
bit5-6=2: use 3-slot packets
bit5-6=3: use 5-slot packets
bit7: reserved for future use
*/
typedef struct _preferred_rate
{
    ///When in Basic Rate mode:    
    uint8_t  no_fec:1; 				
    uint8_t  pkt_slots:2;			
    
    ///When in Enhanced Data Rate mode
    uint8_t  br_edr:2;
    uint8_t  pkt_slots_edr:2;
    uint8_t  rsvd:1;
}preferred_rate_t;


#endif //_BT_LL_LMP_
