/********************************************************************************************************
 * @file     bt_ll_encrypt.h
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

#ifndef BT_LL_ENCRYPT_H_
#define BT_LL_ENCRYPT_H_


/// Range of key size supported by the FW
#define ENC_KEY_SIZE_MAX        16
#define ENC_KEY_SIZE_MIN        1
#define ENC_KEY_SIZE_MASK       0xFFFF

enum{
	ST_CRYPT_IDLE,

	ST_CRYPT_ENCRYPT_MODE_0_REQ,
	ST_CRYPT_WAIT_ENCRYPT_MODE_0_REQ_ACCEPTED,
	ST_CRYPT_WAIT_0_KEY_SIZE_NEGO_OR_ACCEPTED,

	ST_CRYPT_CHECK_ENCRYPT_PAUSE,
	ST_CRYPT_WAIT_ENCRYPT_PAUSE,
	ST_CRYPT_WAIT_ENCRYPT_STOP,
	ST_CRYPT_WAIT_ENCRYPT_STOP_ACCEPTED,
	ST_CRYPT_ENCRYPT_STOP_DONE,

	ST_CRYPT_WAIT_TASK_DONE,

	ST_CRYPT_ENCRYPT_MODE_1_REQ,
	ST_CRYPT_WAIT_ENCRYPT_MODE_1_REQ_ACCEPTED,
	ST_CRYPT_WAIT_1_KEY_SIZE_NEGO_OR_ACCEPTED,

	ST_CRYPT_ENCRYPT_RESUME,
	ST_CRYPT_WAIT_ENCRYPT_START,
	ST_CRYPT_ENCRYPT_WAIT_ACCEPTED_ACK,
	ST_CRYPT_ENCRYPT_WAIT_ACCEPTED_ACK_DONE,
	ST_CRYPT_WAIT_ENCRYPT_START_ACCEPTED,
	ST_CRYPT_ENCRYPT_START_DONE,
};

typedef struct btc_enc
{
    /// key size mask
    uint16_t KeySizeMask;
    /// Key given from Host (or generated via pairing)
    uint8_t  key_from_host;
    /// key type
    uint8_t  KeyType;

    /// key flag
    uint8_t  KeyFlag;
    /// new key flag
    uint8_t  NewKeyFlag;
    /// status of the key
    uint8_t  KeyStatus;
    /// status of pin
    uint8_t  PinStatus;

    /// length of the pin
    uint8_t  PinLength;
    /// encryption size
    uint8_t  EncSize;
    /// encryption mode
    uint8_t  EncMode;
    /// new encryption mode
    uint8_t  NewEncMode;

    /// Authentication key (secure connections)
    uint8_t  Combkey[KEY_LEN];
    /// long term key
    uint8_t  LTKey[KEY_LEN];
    /// semi permanent key
    uint8_t  SemiPermanentKey[KEY_LEN];
    /// overlay key
    uint8_t  Overlay[KEY_LEN];
    /// random rx key
    uint8_t  RandomRx[KEY_LEN];
    /// random tx key
    uint8_t  RandomTx[KEY_LEN];
    /// encryption key
    uint8_t  EncKey[KEY_LEN];

    /// signed response
    uint8_t  Sres[SRES_LEN];
    /// signed response expected from peer
    uint8_t  Sres_expected[SRES_LEN];
    /// aco value
    uint8_t  Aco[ACO_LEN];
    /// pin code
    uint8_t  PinCode[PIN_CODE_MAX_LEN];

    /// boolean key server in progress
    bool     KeyServerInProgress;
    /// boolean link key valid
    bool     LinkKeyValid ;
    /// Prevent encryption change event
    bool     PreventEncEvt;
    /// encryption enable
    uint8_t  EncEnable;

}btc_enc_t;


enum{
	ENC_MAINLOOP_TASK_ID_IDLE,
	ENC_MAINLOOP_TASK_ID_LMP_ENC_MODE_REQ,
	ENC_MAINLOOP_TASK_ID_HCI_SET_CON_ENC,
	ENC_MAINLOOP_TASK_ID_LMP_ACCEPTED,
	ENC_MAINLOOP_TASK_ID_LMP_NOT_ACCEPTED,
	ENC_MAINLOOP_TASK_ID_LMP_PAUSE_ENC_REQ,
	ENC_MAINLOOP_TASK_ID_RSW_PAUSE_ENC,
	ENC_MAINLOOP_TASK_ID_LMP_STOP_ENC_REQ,
	ENC_MAINLOOP_TASK_ID_LMP_RESUME_ENC_REQ,
	ENC_MAINLOOP_TASK_ID_LMP_ENC_KEY_SIZE,
	ENC_MAINLOOP_TASK_ID_LMP_START_ENC_REQ,
	ENC_MAINLOOP_TASK_ID_ACK_LMP_ACCEPTED,
};

enum{
	ENC_IRQ_TASK_ID_IDLE,
	ENC_IRQ_TASK_ID_INIT_MEM,
};

typedef int32_t (*btc_enc_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);

btc_enc_task_callback_t btc_enc_mainloop_task_callback;
btc_enc_task_callback_t btc_enc_irq_task_callback;


void btc_bb_enc_stop(uint8_t link_id, uint8_t txdisable, uint8_t rxdisable);
void btc_bb_enc_start (uint8_t link_id, uint8_t txen, uint8_t rxen, uint8_t *key,uint8_t keysize);
void btc_bb_enc_key_load(uint8_t link_id, struct ltk *key, struct initialization_vector *iv);
int lmp_send_pause_enc_req(uint8_t link_id, uint8_t tid);
int lmp_send_stop_enc_req(uint8_t link_id,uint8_t tid);
int lmp_send_resume_enc_req(uint8_t link_id,uint8_t tid);
int lmp_send_start_enc_req(uint8_t link_id, uint8_t *rand, uint8_t tid);
int32_t btc_task_encrypt_decrypt_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

uint8_t hci_set_con_enc_handler(uint16_t con_hdl,uint8_t enc_enable);
uint8_t hci_rd_enc_key_size_handler(uint16_t con_hdl,uint8_t* keysize);
void hci_send_encryption_change_event(uint8_t status,uint16_t con_hdl,uint8_t encrypt_enabled);
void hci_send_encryption_change_event(uint8_t status,uint16_t con_hdl,uint8_t encrypt_enabled);

extern btc_enc_t* env_enc_ptr;

static inline btc_enc_t * btc_enc_get_env(uint8_t link_id)
{
	return (btc_enc_t*)(env_enc_ptr? (env_enc_ptr+link_id):0);
}

#endif /* BT_LL_ENCRYPT_H_ */
