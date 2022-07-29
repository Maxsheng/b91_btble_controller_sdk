/********************************************************************************************************
 * @file     bt_crypto.h
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

#ifndef _BT_CRYPTO_H_
#define _BT_CRYPTO_H_

uint8_t* crypto_get_pub_key(void);
uint8_t * crypto_get_pri_key(void);
uint8_t crypto_campare_debug_key(uint8_t * remote_public_key);
uint8_t* crypto_get_256_pub_key(void);
uint8_t * crypto_get_256_pri_key(void);
void crypto_memcpy_revert (uint8_t * pd, uint8_t * ps, int n);
uint8_t crypto_public_key_is_invalid(uint8_t link_id);
void crypto_init(void);
void crypto_get_nonce(uint8_t* Nonce);
void crypto_bdaddr_to_aco(uint8_t *BdAddr, uint8_t *Aco);
uint8_t crypto_get_auth_method(struct io_capability* LocalCap, struct io_capability* RemoteCap);
bool crypto_passkey_req(struct io_capability* LocalCap, struct io_capability* RemoteCap);
uint8_t crypto_get_passkey_bit(uint32_t Passkey,uint8_t BitNum);
uint32_t crypto_passkey_generate(void);

void crypto_hmac_sha_out (uint32_t *p, uint8_t* po);
uint8_t crypto_f1(uint8_t* LocRandN, uint8_t* Commitment, uint8_t Number, uint8_t Role, uint8_t* key);
uint8_t crypto_f2(uint8_t* N1, uint8_t* N2,   uint8_t*  A, uint8_t* B, uint8_t* DHKey_Ptr, uint8_t* LinkKey);
uint8_t crypto_f3(uint8_t* N1, uint8_t* N2, uint8_t* R,	uint8_t* iocap, uint8_t* A, uint8_t* B, uint8_t* DHKey_Ptr, uint8_t* CheckValue);
uint8_t crypto_g( uint8_t Role, uint8_t * Na, uint8_t  * Nb,uint8_t * remote_pubkey , uint32_t* UserConfirmNum);
uint8_t crypto_oob_f1(uint8_t* Nonce, uint8_t* Commitment, uint8_t Number, uint8_t IsPublicKeyLocal, uint8_t* key);



#endif /* _BT_CRYPTO_H_ */
