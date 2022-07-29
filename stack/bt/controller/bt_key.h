/********************************************************************************************************
 * @file     bt_key.h
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

/**
****************************************************************************************
*
* @file lm_key.h
*
* @brief link manager key management
*
* Copyright (C) OpenC 2009-2013
*
*
****************************************************************************************
*/
#ifndef LM_KEY_H_
#define LM_KEY_H_
/**
****************************************************************************************
* @defgroup LMKEY Link Keys
* @ingroup LM
* @brief LM module for link key management.
* @{
****************************************************************************************
*/

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Processes E1 algorithm for Authentication
 *
 * @param[in]  Key       Key Vector[16]
 * @param[in]  BdAddr    BdAddr[6]
 * @param[in]  Random    Random Vector[16]
 * @param[out] Sres      SRES Vector[4]
 * @param[out] Aco       ACIO Vector[12]
 *
 ****************************************************************************************
 */
void E1(struct ltk Key, struct bd_addr BdAddr, struct ltk Random, struct sres_nb *Sres, struct aco *Aco);

/**
 ****************************************************************************************
 * @brief Processes E21 algorithm for Authentication
 *
 * @param[in]  Random      Random Vector[16]
 * @param[in]  BdAddr      BdAddr[6]
 * @param[out] Key         Result Vector[16]
 *
 ****************************************************************************************
 */
void E21(struct ltk Random, struct bd_addr BdAddr, struct ltk *Key);

/**
 ****************************************************************************************
 * @brief Processes E22 algorithm for Authentication
 *
 * @param[in]  Random    Random Vector[16]
 * @param[in]  BdAddr    BdAddr[6]
 * @param[in]  Pin       PIN Vector[1..16]
 * @param[in]  PinLen    PIN length (1..16)
 * @param[out] KeyOut    Result Vector[16]
 *
 ****************************************************************************************
 */
void E22(struct ltk Random, struct bd_addr BdAddr, struct pin_code Pin, uint8_t PinLen, struct ltk *KeyOut);
/**
 ****************************************************************************************
 * @brief Processes E3 algorithm for Authentication
 *
 * @param[in]  Key       Key Vector[16]
 * @param[in]  Cof       COF[12]
 * @param[in]  Random    Random Vector[16]
 * @param[out] KeyOut    Result Vector[16]
 *
 ****************************************************************************************
 */
void E3(struct ltk Key, struct aco Cof, struct ltk Random, struct ltk *KeyOut);

/**
 ****************************************************************************************
 * @brief Encryption : Processes K'c Key for encryption
 *
 * @param[in]  Random      Random Vector[16]
 * @param[in]  BdAddr      BdAddr[6]
 * @param[out] KPrimC      K'c Vector (Result)
 *
 ****************************************************************************************
 */
void KPrimC(struct ltk Kc, uint8_t length, struct ltk *KPrimC);

/**
 ****************************************************************************************
 * @brief Xor of 2 Vectors
 *
 * @param[in]  VectorIn1      Input Vector1 [16]
 * @param[in]  VectorIn2      Input Vector2 [16]
 * @param[out] VectorOut      Result Vector[16]
 *
 ****************************************************************************************
 */
void XorKey(struct ltk VectorIn1, struct ltk VectorIn2, struct ltk *VectorOut);
void LM_E22(uint8_t *Random, uint8_t *BdAddr, uint8_t *Pin, uint8_t PinLen, uint8_t *Result);
void LM_E21(uint8_t *Random, uint8_t *BdAddr, uint8_t *Result);
void LM_E1(uint8_t *Key, uint8_t *BdAddr, uint8_t *Random, uint8_t *Sres, uint8_t *Aco);
void LM_E3(uint8_t *Key, uint8_t *Cof, uint8_t *Random, uint8_t *Result);
void LM_KPrimC(uint8_t *Kc, uint32_t length, uint8_t *Result);


///@} LMKEY

#endif // LM_KEY_H_
