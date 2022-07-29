/********************************************************************************************************
 * @file     bt_ll_bch.h
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
* @file bt_ll_bch.h
*
* @brief    Synchronization word (BCH) creation functions
*
* Copyright (C) OpenC 2009-2013
*
*
****************************************************************************************
*/
#ifndef _BT_LL_BCH_H_
#define _BT_LL_BCH_H_
/**
****************************************************************************************
* @defgroup LDBCH Synchronization Word
* @ingroup LD
* @brief Link driver synchronization word creation.
*
* @{
****************************************************************************************
*/

#include <stdlib.h>        // standard
#include <stdint.h>        // integer

/*
 * DEFINITIONS & MACROS
 ****************************************************************************************
 */
/// BCH size
#define BT_LL_BCH_SIZE                          8

/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Generate a BCH code from LAP.
 *
 * The Lap pointer must point on a table :
 *                   Msb Lsb
 *          Lap[0] = a7..a0
 *          Lap[1] = a15..a7
 *          Lap[2] = a23..a16
 * The Bch result is stored in :
 *                   Msb  Lsb
 *          Bch[0] = a7..a0
 *          Bch[1] = a15..a8
 *          Bch[2] = a23..a16
 *          Bch[3] = a31..a24
 *          Bch[4] = a39..a32
 *          Bch[5] = a47..a40
 *          Bch[6] = a55..a48
 *          Bch[7] = a63..a56
 *
 * @param[in]  Lap    Input LAP Vector[3]
 * @param[out] Bch    Output BCH [8]
 *
 ****************************************************************************************
 */
void bt_ll_bch_create(uint8_t *Lap, uint8_t *Bch);

/*
 * VARIABLE DECLARATION
*****************************************************************************************
 */
/// BCH (used for sync and FHS)
extern uint8_t m_bch[BT_LL_BCH_SIZE];

///@} LDBCH

#endif // _BT_LL_BCH_H_
