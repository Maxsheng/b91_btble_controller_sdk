/********************************************************************************************************
 * @file     bt_sp.h
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
* @file bt_sp.h
*
* @brief Link Manager simple pairing
*
* Copyright (C) OpenC 2009-2013
*
*
****************************************************************************************
*/
#ifndef BT_SP_H_
#define BT_SP_H_
/**
****************************************************************************************
* @defgroup LMSP SP Functions
* @ingroup LM
* @brief LM module for simple pairing functions.
* @{
****************************************************************************************
*/
#include "types.h"
/*
 * DEFINITION
 * ***************************************************************************************
 */
//#define   bool  bool2
typedef   int  bool2;

#define LM_SP_JUST_WORKS                    0x00
#define LM_SP_NUM_COMP                      0x01
#define LM_SP_PASSKEY                       0x02
#define LM_SP_OOB                           0x03

#define LM_OPER_SUCCESS                     0x00
#define LM_OPER_PENDING                     0x01
#define LM_OPER_FAILED                      0x02

#define LM_INVALID_INDEX                    0xFF
#define LM_PUB_X_COORDINATE                 0x00
#define LM_PUB_BOTH_COORDINATE              0x01

#define LM_SP_NUM_SIMUL_INSTANCE            0x02
#define LM_SP_DHKEY_CALC_NUM_BITS_PER_CALL  0x08
/*
 * FUNCTION DECLARATIONS
 * ***************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief This function initializes the SP block
 *
 ****************************************************************************************
 */
void lm_init_sp(void);
/**
 ****************************************************************************************
 * @brief This functions is used to get the index of the buffer to be used for SP.
 *
 ****************************************************************************************
 */
uint8_t lm_sp_dhkey_get_buffer_idx(void);
/**
 ****************************************************************************************
 * @brief This functions is used to free the buffer pointed to by the index.
 *
 * @param[in] Index      buffer index
 *
 ****************************************************************************************
 */
void lm_sp_dhkey_free_buffer(uint8_t Index);
/**
 ****************************************************************************************
 * @brief This function is used to read the simple pairing mode.
 *
 * @return Simple pairing mode
 *
 ****************************************************************************************
 */
uint8_t lm_sp_get_pairing_mode(void);
/**
 ****************************************************************************************
 * @brief This function is used to write the simple pairing mode.
 *
 * @param[in] Mode      simple pairing mode
 *
 ****************************************************************************************
 */
void lm_sp_set_pairing_mode(uint8_t Mode);

/**
 ****************************************************************************************
 * @brief This function is used to validate the encapsulated header received.
 *
 * @param[in] EncapsulatedMajorType   The Major type received in the encapsulated header
 * @param[in] EncapsulatedMinorType   The Minor type received in the encapsulated header
 * @param[in] PayloadLength           The payload length received in the encapsulated header.
 *
 * @return Status
 *
 ****************************************************************************************
 */
bool lm_check_encap_pdu_hdr(uint8_t EncapsulatedMajorType, uint8_t EncapsulatedMinorType,
                            uint8_t PayloadLength);

/**
 ****************************************************************************************
 * @brief This function is used to get the authentication algorithm based on
 *        the device's IO capability
 *
 * @param[in] LocalCap           Local IO capability pointer structure
 * @param[in] RemoteCap          Remote IO capability pointer structure
 *
 * @return Simple pairing method
 ****************************************************************************************
 */
uint8_t lm_get_auth_method(struct io_capability* LocalCap, struct io_capability* RemoteCap);
/**
 ****************************************************************************************
 * @brief This function retrieves the nonce (Byte random number)
 *
 * @param[out] Nonce            Output nonce value
 *
 ****************************************************************************************
 */
void lm_get_nonce(struct byte16* Nonce);

/**
 ****************************************************************************************
 * @brief This function is used to calculate the F1 Value.
 *
 * @param[in] LocRandN         Pointer to ltk structure local random number
 * @param[in] Commitment       Pointer to ltk structure commitment value
 * @param[in] Number           Numerical value
 * @param[in] Role             Device role
 * @param[in] BufferIndex      Buffer index for sp
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_f1(struct byte16* LocRandN, struct byte16* Commitment, uint8_t Number,
              uint8_t Role, uint8_t BufferIndex);
/**
 ****************************************************************************************
 * @brief This function is used to calculate the F1 Value for OOB.
 *
 * @param[in] Nonce               Pointer to ltk structure nonce value
 * @param[in] Commitment          Pointer to ltk structure commitment value
 * @param[in] Number              Numerical value
 * @param[in] IsPublicKeyLocal    Check if local public key
 * @param[in] BufferIndex         Buffer index for sp
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_oob_f1(struct byte16* Nonce, struct byte16* Commitment, uint8_t Number,
                  uint8_t IsPublicKeyLocal, uint8_t BufferIndex);
/**
 ****************************************************************************************
 * @brief This function is the G function.
 *
 * @param[in] Na                Pointer to ltk structure Na
 * @param[in] Nb                Pointer to ltk structure Nb
 * @param[in] Role              Role
 * @param[in] BufferIndex       Buffer index for sp
 * @param[in] UserConfirmNum    User confirmation number
 *
 ****************************************************************************************
 */
uint8_t lm_g(struct byte16* Na, struct byte16* Nb, uint8_t Role, uint8_t BufferIndex,
             uint32_t* UserConfirmNum);
/**
 ****************************************************************************************
 * @brief Based on the local and remote device IO capability, this function
 *        tells whether device need to send the Pass key request to the host or not.
 *
 * @param[in] LocalCap         Local IO capability
 * @param[in] RemoteCap        Remote IO capability
 *
 * @return boolean if pass key request is possible
 *
 ****************************************************************************************
 */
bool lm_passkey_req(struct io_capability* LocalCap, struct io_capability* RemoteCap);
/**
 ****************************************************************************************
 * @brief This function is used to get the bit value specified by bit in a pass key.
 *
 * @param[in] Passkey          Pass key value
 * @param[in] Bit              Bit value
 *
 * @return Pass key bit
 ****************************************************************************************
 */
uint8_t lm_get_passkey_bit(uint32_t Passkey, uint8_t Bit);
/**
 ****************************************************************************************
 * @brief This function is used to generate the random pass key.
 *
 * @return passkey value
 *
 ****************************************************************************************
 */
uint32_t lm_passkey_generate(void);
/**
 ****************************************************************************************
 * @brief This function is used to calculate the F3 Value.
 *
 * @param[in] N1                        N1 16-byte length value
 * @param[in] N2                        N2 16-byte length value
 * @param[in] R                         Randomizer
 * @param[in] IOCAP                     IO capability
 * @param[in] B                         BD address
 * @param[in] DHKeyBufferIndex          DHKey buffer index for SP
 * @param[in] CheckValue                Value check
 *
 * @return Status
 ****************************************************************************************
 */
uint8_t lm_f3(struct byte16* N1,          struct byte16* N2, struct randomizer* R,
              struct io_capability IOCAP, struct bd_addr* A, struct bd_addr* B,
              uint8_t DHKeyBufferIndex,   struct byte16* CheckValue);
/**
 ****************************************************************************************
 * @brief This function compares the generated DHkey values.
 *
 * @param[in] CheckValue1          DHKey value 1
 * @param[in] CheckValue2          DHKey value 2
 *
 * @return compare result
 *
 ****************************************************************************************
 */
bool lm_dhkey_check_compare(struct ltk* CheckValue1, struct ltk* CheckValue2);
/**
 ****************************************************************************************
 * @brief This function is used to calculate the F2 Value.
 *
 * @param[in] N1                 N1 16-byte length value
 * @param[in] N2                 N2 16-byte length value
 * @param[in] A                  BD address A
 * @param[in] B                  BD address B
 * @param[in] BufferIndex        Buffer index for SP
 * @param[in] LinkKey            Link key LTK structure
 *
 * @return Status
 ****************************************************************************************
 */
uint8_t lm_f2(struct byte16* N1, struct byte16* N2, struct bd_addr* A, struct bd_addr* B,
              uint8_t BufferIndex, struct ltk* LinkKey);
/**
 ****************************************************************************************
 * @brief Initialize Diffie Hellman key calculation
 *
 * @param[in] BufferIndex          Buffer index for sp
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_dhkey_calc_init(uint8_t BufferIndex);
/**
 ****************************************************************************************
 * @brief This function compares DHkeys.
 *
 * @param[in] BufferIndex          Buffer index for sp
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_dhkey_compare(uint8_t BufferIndex);
/**
 ****************************************************************************************
 * @brief This function is used to set the simple pairing debug mode.
 *
 * @param[in] DebugEnable     Set or reset simple pairing mode
 *
 ****************************************************************************************
 */
void lm_sp_set_debug_pairing_mode(uint8_t DebugEnable);
/**
 ****************************************************************************************
 * @brief This function checks if key is Debug key is used for authentication
 *
 * @param[in] BufferIndex          Buffer index for sp instance
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_is_debug_key(uint8_t BufferIndex);
/**
 ****************************************************************************************
 * @brief This function gets the local OOB data
 *
 * @param[in]  R              Randomizer value
 * @param[out] C              Hash value
 ****************************************************************************************
 */
void lm_get_oob_local_data(struct randomizer* R, struct hash* C);
/**
 ****************************************************************************************
 * @brief This function is used to set a particular 32 bit number in a 16 byte number
 *
 * @param[out] A        Output array
 * @param[in]  Number   32-bit number to be copied
 *
 ****************************************************************************************
 */
void lm_sp_32bits_to_array(struct byte16* A, uint32_t Number);

/**
 ****************************************************************************************
 * @brief This function is used to Get the commitment for OOB algorithm.
 *
 * @param[out] A        Output parameter (randomizer) where commitment will be copied
 * @param[in]  IOCAP    IO capability
 *
 ****************************************************************************************
 */
void lm_get_oob_local_commit(struct randomizer* A, struct io_capability IOCAP);
/**
 ****************************************************************************************
 * @brief This function compares commitment values (16-byte numbers).
 *
 * @param[in] LocCommitment           Local commitment value
 * @param[in] RemCommitment           Remote commitment value
 *
 * @return compare result
 *
 ****************************************************************************************
 */
bool lm_compare_commit(struct byte16* LocCommitment, struct byte16* RemCommitment);
/**
 ****************************************************************************************
 * @brief This function is used to save the remote public key received as encapsulated PDU
 *
 * @param[in] EncapDataInPtr    The input data to be saved
 * @param[in] BufferIndex       The index to the simple pairing instance
 * @param[in] EncapPduCtr       The number of Encapsulated pay load data
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_set_encap_pdu_data(struct byte16* EncapDataInPtr, uint8_t BufferIndex, uint8_t EncapPduCtr);
/**
 ****************************************************************************************
 * @brief This function is used to get the encapsulated payload.
 *
 * @param[out] EncapDataOutPtr       Pointer to encapsulated data
 * @param[in]  EncapPduCtr           Counter of the encapsulated PDU
 *
 * @return Status
 *
 ****************************************************************************************
 */
uint8_t lm_get_encap_pdu_data(struct byte16* EncapDataOutPtr, uint8_t EncapPduCtr);

///@} LMSP

#endif // BT_SP_H_
