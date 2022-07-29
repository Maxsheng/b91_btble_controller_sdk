/********************************************************************************************************
 * @file     bt_ll_pair.h
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

#ifndef BT_LL_PAIR_H_
#define BT_LL_PAIR_H_

#include "bt_spcomputation.h"

enum{
	ST_LEGACY_PAIRING_IDLE,
	ST_LEGACY_PAIRING_START,
	ST_LEGACY_PAIRING_WAIT_PIN_CODE_REPLY,
	ST_LEGACY_PAIRING_WAIT_ACCEPTED_OR_NOT_ACCEPTED,
	ST_LEGACY_PAIRING_WAIT_COMBKEY,
	ST_LEGACY_PAIRING_DONE,
};

enum{
	LEGACY_PAIR_MAINLOOP_TASK_ID_IDLE,
	LEGACY_PAIR_MAINLOOP_TASK_ID_START,
	LEGACY_PAIR_MAINLOOP_TASK_ID_LMP_INRAND,
	LEGACY_PAIR_MAINLOOP_TASK_ID_HCI_PIN_CODE_REQ_REPLY,
	LEGACY_PAIR_MAINLOOP_TASK_ID_HCI_PIN_CODE_REQ_NEG_REPLY,
	LEGACY_PAIR_MAINLOOP_TASK_ID_LMP_ACCEPTED,
	LEGACY_PAIR_MAINLOOP_TASK_ID_LMP_NOT_ACCEPTED,
	LEGACY_PAIR_MAINLOOP_TASK_ID_LMP_COMKEY,
	LEGACY_PAIR_MAINLOOP_TASK_ID_LMP_UNITKEY,
};

enum{
	LEGACY_PAIR_IRQ_TASK_ID_IDLE,

};

typedef int32_t (*btc_legacy_pair_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);

btc_legacy_pair_task_callback_t btc_legacy_pair_mainloop_task_callback;
btc_legacy_pair_task_callback_t btc_legacy_pair_irq_task_callback;

void btc_legacy_pair_register_module(void);


enum{
	ST_SSP_IDLE,
	ST_SSP_START,
	ST_SSP_WAIT_IO_CAP_REPLY,
	ST_SSP_WAIT_IO_CAP_RES,
	ST_SSP_PUBLIC_KEY_EXCH_WAIT_HDR_ACCEPTED,
	ST_SSP_PUBLIC_KEY_EXCH_WAIT_PAYLOAD_ACCEPTED,
	ST_SSP_PUBLIC_KEY_EXCH_WAIT_HDR,
	ST_SSP_PUBLIC_KEY_EXCH_WAIT_PAYLOAD,
	ST_SSP_PUBLIC_KEY_EXCH_WAIT_CAL_DHKEY,
	ST_SSP_AUTHEN_STAGE1_NUM_WAIT_SP_CFM,
	ST_SSP_AUTHEN_STAGE1_NUM_WAIT_SP_NB_ACCEPTED,
	ST_SSP_AUTHEN_STAGE1_NUM_WAIT_SP_NB,
	ST_SSP_AUTHEN_STAGE1_NUM_WAIT_USER_CFM_REPLY,
	ST_SSP_AUTHEN_STAGE1_PASSKEY_WAIT_REQ_REPLY,
	ST_SSP_AUTHEN_STAGE1_PASSKEY_WAIT_SP_CFM,
	ST_SSP_AUTHEN_STAGE1_PASSKEY_WAIT_SP_NB_ACCEPTED,
	ST_SSP_AUTHEN_STAGE1_PASSKEY_WAIT_SP_NB,
	ST_SSP_AUTHEN_STAGE1_OOB_WAIT_REQ_REPLY,
	ST_SSP_AUTHEN_STAGE1_OOB_WAIT_SP_NB_ACCEPTED,
	ST_SSP_AUTHEN_STAGE1_OOB_WAIT_SP_NB,

	ST_SSP_AUTHEN_STAGE2_WAIT_DHKEY_CHECK_ACCEPTED,
	ST_SSP_AUTHEN_STAGE2_WAIT_DHKEY_CHECK,
	ST_SSP_DONE,
};


typedef struct btc_ssp
{
    /// Environment to perform DH Key computations
    lm_sp_p192_comp_global_data* lm_sp_p192_dhkey_data;
    /// Environment to perform DH Key computations
    lm_sp_p256_comp_global_data* lm_sp_p256_dhkey_data;
    /// pass key
    uint32_t    Passkey;
    /// local random number
    uint8_t 	LocRandN[16];
    /// remote random number
    uint8_t 	RemRandN[16];
    /// remote oob random number
    uint8_t 	OOB_RemRandN[16];
    /// local commitment value
    uint8_t 	LocCommitment[16];
    /// remote commitment value
    uint8_t 	RemCommitment[16];
    /// DH key check key
    uint8_t 	DHKeyCheck[KEY_LEN];
    /// local IO capability
    struct io_capability IOCap_loc;
    /// remote IO capability
    struct io_capability IOCap_rem;
    /// encapsulated PDU counter
    uint8_t EncapPduCtr;
    /// simple pairing phase1 failure
    uint8_t ssphase1Failed;
    /// simple pairing DH key
    uint8_t SPDHKey;
    /// simple pairing transaction id
    uint8_t SpTId;
    /// simple pairing initiator
    bool SPInitiator;
    /// Secure Connections (true if supported by both controllers and both Hosts)
    bool sec_con;
}btc_ssp_t;



enum{
	SSP_MAINLOOP_TASK_ID_IDLE,
	SSP_MAINLOOP_TASK_ID_START,
	SSP_MAINLOOP_TASK_ID_LMP_IOCAP_REQ,
	SSP_MAINLOOP_TASK_ID_HCI_IO_CAP_REQ_REPLY,
	SSP_MAINLOOP_TASK_ID_HCI_IO_CAP_REQ_NEG_REPLY,
	SSP_MAINLOOP_TASK_ID_LMP_IOCAP_RES,
	SSP_MAINLOOP_TASK_ID_LMP_NOT_ACCEPTED_EXT,
	SSP_MAINLOOP_TASK_ID_LMP_ACCEPTED,
	SSP_MAINLOOP_TASK_ID_LMP_NOT_ACCEPTED,
	SSP_MAINLOOP_TASK_ID_LMP_ENCAPS_HDR,
	SSP_MAINLOOP_TASK_ID_LMP_ENCAPS_PAYL,
	SSP_MAINLOOP_TASK_ID_LMP_SP_CFM,
	SSP_MAINLOOP_TASK_ID_LMP_SP_NB,
	SSP_MAINLOOP_TASK_ID_LMP_DHKEY_CHK,
	SSP_MAINLOOP_TASK_ID_LMP_NUM_CMP_FAIL_EXT,
	SSP_MAINLOOP_TASK_ID_LMP_PASSKEY_FAIL_EXT,
	SSP_MAINLOOP_TASK_ID_LMP_OOB_FAIL_EXT,
};

enum{
	SSP_IRQ_TASK_ID_IDLE,
	SSP_IRQ_TASK_ID_INIT_MEM,
};

typedef int32_t (*btc_ssp_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);

btc_ssp_task_callback_t btc_ssp_mainloop_task_callback;
btc_ssp_task_callback_t btc_ssp_irq_task_callback;



extern uint8_t crypto_oob_f1(uint8_t* Nonce, uint8_t* Commitment, uint8_t Number, uint8_t IsPublicKeyLocal, uint8_t* key);

void hci_send_change_connection_link_key_complete_event(uint8_t status, uint16_t con_hdl);

uint8_t hci_pin_code_req_reply_handler(uint8_t* bd_addr,uint8_t pincode_len,uint8_t* pincode);

uint8_t hci_pin_code_req_neg_reply_handler(uint8_t *bd_addr);

int bt_ll_task_legacy_pairing(int Lid,int task_id);
int32_t btc_task_legacy_pair_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

uint8_t hci_io_cap_req_neg_reply_handler(uint8_t *bd_addr,uint8_t reason);

uint8_t hci_io_cap_req_reply_handler(uint8_t *bd_addr, uint8_t io_capability, uint8_t oob_data_present, uint8_t authen_req);

uint8_t hci_user_cfm_req_reply_handler(uint8_t *bd_addr);

uint8_t hci_user_cfm_req_neg_reply_handler(uint8_t *bd_addr);

uint8_t hci_rem_oob_data_req_reply_handler(uint8_t* bd_addr,uint8_t* commitment,uint8_t * random);

uint8_t hci_rem_oob_data_req_neg_reply_handler(uint8_t* bd_addr);

int bt_ll_task_secure_simple_pairing(int Lid,int task_id);
int32_t btc_task_ssp_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

uint8_t hci_change_connection_link_key_handler(uint16_t con_hdl);
uint8_t hci_user_passkey_req_reply_handler(uint8_t* bd_addr,uint8_t* numeric_value);
uint8_t hci_user_cfm_req_neg_reply_handler(uint8_t *bd_addr);
uint8_t hci_rd_loc_oob_data_handler(uint8_t* commitment,uint8_t* random);
void hci_send_io_capability_response_event(uint8_t * bd_addr, uint8_t io_cap,uint8_t oob_data_present, uint8_t authen_req);
uint8_t hci_user_passkey_req_neg_reply_handler(uint8_t* bd_addr);

extern btc_ssp_t* env_ssp_ptr;

static inline btc_ssp_t * btc_ssp_get_env(uint8_t link_id)
{
	return (btc_ssp_t*)(env_ssp_ptr? (env_ssp_ptr+link_id):0);
}

void btc_ssp_register_module(btc_ssp_t* env_ssp);

#endif /* BT_LL_PAIR_H_ */
