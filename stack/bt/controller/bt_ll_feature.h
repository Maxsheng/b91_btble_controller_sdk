/********************************************************************************************************
 * @file     bt_ll_feature.h
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

#ifndef _BT_LL_FEATURE_H_
#define _BT_LL_FEATURE_H_

#include <stdint.h>
#include "bt_ll.h"
/// Features definitions
#define LM_3_SLOT_FEATURE           0
#define LM_5_SLOT_FEATURE           1
#define LM_ENC_FEATURE              2
#define LM_SLOT_OFFSET_FEATURE      3
#define LM_TIMING_ACC_FEATURE       4
#define LM_SWITCH_FEATURE           5
#define LM_HOLD_FEATURE             6
#define LM_SNIFF_FEATURE            7

#define LM_PARK_FEATURE             8
#define LM_RSSI_FEATURE             9
#define LM_QUALITY_FEATURE          10
#define LM_SCO_FEATURE              11
#define LM_HV2_FEATURE              12
#define LM_HV3_FEATURE              13
#define LM_ULAW_FEATURE             14
#define LM_ALAW_FEATURE             15

#define LM_CVSD_FEATURE             16
#define LM_PAGING_FEATURE           17
#define LM_POWER_FEATURE            18
#define LM_TRANSP_SCO_FEATURE       19
#define LM_BCAST_ENCRYPT_FEATURE    23

#define LM_EDR_2MB_FEATURE          25
#define LM_EDR_3MB_FEATURE          26
#define LM_ENH_INQSCAN_FEATURE      27
#define LM_INT_INQSCAN_FEATURE      28
#define LM_INT_PAGESCAN_FEATURE     29
#define LM_RSSI_INQRES_FEATURE      30
#define LM_EV3_FEATURE              31

#define LM_EV4_FEATURE              32
#define LM_EV5_FEATURE              33
#define LM_AFH_CAPABLE_S_FEATURE    35
#define LM_AFH_CLASS_S_FEATURE      36
#define LM_BR_EDR_NO_SUPP_FEATURE   37
#define LM_LE_FEATURE               38
#define LM_3_SLOT_EDR_FEATURE       39
#define LM_5_SLOT_EDR_FEATURE       40
#define LM_SNIFF_SUBRAT_FEATURE     41
#define LM_PAUSE_ENCRYPT_FEATURE    42
#define LM_AFH_CAPABLE_M_FEATURE    43
#define LM_AFH_CLASS_M_FEATURE      44
#define LM_EDR_ESCO_2MB_FEATURE     45
#define LM_EDR_ESCO_3MB_FEATURE     46
#define LM_3_SLOT_EDR_ESCO_FEATURE  47
#define LM_EIR_FEATURE              48
#define LM_LE_BR_EDR_FEATURE        49
#define LM_SSP_CTRL_FEATURE         51
#define LM_ENCAP_PDU_FEATURE        52
#define LM_ERRO_DATA_REPORT         53
#define LM_NFLUSH_PBF_FEATURE       54
#define LM_LSTO_CHG_EVT_FEATURE     56
#define LM_INQ_TXPWR_FEATURE        57
#define LM_EPC_FEATURE              58
#define LM_EXTENDED_FEATURES        63

#define LM_SSP_HOST_FEATURE         64
#define LM_LE_HOST_FEATURE          65
#define LM_LE_BR_EDR_HOST_FEATURE   66
#define LM_SEC_CON_HOST_FEATURE     67

#define LM_CSB_MASTER_FEATURE          128
#define LM_CSB_SLAVE_FEATURE           129
#define LM_SYNC_TRAIN_FEATURE          130
#define LM_SYNC_SCAN_FEATURE           131
#define LM_INQ_RES_NOTIF_EVT_FEATURE          132
#define LM_GEN_INTERL_SCAN_FEATURE            133
#define LM_COARSE_CLK_ADJ_FEATURE             134
#define LM_SEC_CON_CTRL_FEATURE               136
#define LM_PING_FEATURE                       137
#define LM_SAM_FEATURE                        138
#define LM_TRAIN_NUDGING_FEATURE              139


extern _attribute_data_retention_sec_ uint8_t local_supp_feats[FEATURE_PAGE_MAX][FEATS_LEN];
extern _attribute_data_retention_sec_ uint8_t local_supp_cmds[39];

/**
 ****************************************************************************************
 * @brief                  This function serves to check whether the controller support the specified feature
 *
 * @param[in] feature_ptr  pointer to the local feature list
 *
 * @param[in] feature      set the specified feature
 *
 *
 * @param[out] Result      0->Support ,-1-> Do Not Support
 *
 * @return                 CO_ERROR_NO_ERROR if the lmp procedure is started successfully
 *
 ****************************************************************************************
 */

int8_t bt_ll_check_feature(uint8_t *feature_ptr, uint8_t feature);

int lmp_send_feature_req(int Lid,uint8_t* local_feature);

int8_t bt_ll_set_feature_mask(struct features *FeaturesPtr, uint8_t feature,bool bit);
bool bt_ll_get_feature(struct features *FeaturesPtr, uint8_t feature);
int lmp_send_feature_ext_req(int Lid,uint8_t* local_feature_ext,uint8_t page_num);
void bt_ll_read_features(uint8_t page_nb, uint8_t *page_nb_max, struct features *feats);

static inline uint8_t bt_ll_set_local_host_support_ssp(uint8_t sp_mode)
{
	local_supp_feats[FEATURE_PAGE_1][0] &=  ~(B0_HOST_SSP_MSK);
	local_supp_feats[FEATURE_PAGE_1][0] |= ((sp_mode<<B0_HOST_SSP_POS) & B0_HOST_SSP_MSK);
	return CO_ERROR_NO_ERROR;
}


static inline uint8_t bt_ll_get_local_host_support_ssp(void)
{
	return local_supp_feats[FEATURE_PAGE_1][0]&B0_HOST_SSP_MSK;
}

static inline uint8_t bt_ll_get_local_support_ssp(void)
{
	return local_supp_feats[FEATURE_PAGE_0][6]&B6_SSP_MSK;
}

static inline uint8_t bt_ll_set_local_host_support_secure_con(uint8_t sec_con)
{
	local_supp_feats[FEATURE_PAGE_1][0] &=  ~(B0_HOST_SECURE_CON_MSK);
	local_supp_feats[FEATURE_PAGE_1][0] |= ((sec_con<<B0_HOST_SECURE_CON_POS) & B0_HOST_SECURE_CON_MSK );	
	return CO_ERROR_NO_ERROR;
}

static inline uint8_t bt_ll_get_local_host_support_secure_con(void)
{
	return local_supp_feats[FEATURE_PAGE_1][0]&B0_HOST_SECURE_CON_MSK;
}

static inline uint8_t bt_ll_get_local_support_secure_con(void)
{
	return local_supp_feats[FEATURE_PAGE_2][1]&B1_SEC_CON_CTRL_MSK;
}

static inline uint8_t bt_ll_get_local_support_pause_enc(void)
{
	return local_supp_feats[FEATURE_PAGE_0][5]&B5_PAUSE_ENC_MSK;
}

static inline uint8_t bt_ll_get_remote_support_pause_enc(uint8_t Lid)
{
	bt_ll_acl_env_t *acl_par = bt_ll_acl_env_get(Lid);
    if (!acl_par)
    {
        my_dump_str_u32s(1, "bt_ll_get_local_pause_enc, err", Lid, env_sch.last_link, 0, 0);
        return 0;
    }

	return acl_par->remote_features[FEATURE_PAGE_0][5]&B5_PAUSE_ENC_MSK;
}

int lmp_send_feature_res(int Lid, uint8_t *local_feature);

#endif /* _BT_LL_FEATURE_H_ */
