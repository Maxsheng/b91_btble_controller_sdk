/********************************************************************************************************
 * @file     co_bt.h
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
 * @file co_bt.h
 *
 * @brief This file contains the common Bluetooth defines, enumerations and structures
 *        definitions for use by all modules in RW stack.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef CO_BT_H_
#define CO_BT_H_

/**
 ****************************************************************************************
 * @addtogroup COMMON Common SW Block
 * @ingroup ROOT
 * @brief The Common RW SW Block.
 *
 * The COMMON is the block with Bluetooth definitions and structures shared
 * to all the protocol stack blocks. This also contain software wide error code
 * definitions, mathematical functions, help functions, list and buffer definitions.
 *
 * @{
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup CO_BT Common Bluetooth defines
 * @ingroup COMMON
 * @brief Common Bluetooth definitions and structures.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>       // standard boolean definitions
#include <stddef.h>        // standard definitions
#include <stdint.h>        // standard integer definitions

/*
 * DEFINES
 ****************************************************************************************
 */

#include "co_bt_defines.h" // Bluetooth defines
//#include "co_lmp.h"        // Bluetooth LMP definitions
//#include "co_hci.h"        // Bluetooth HCI definitions
#include "co_error.h"      // Bluetooth error codes definitions

/// Maximum value of a Bluetooth clock (in 312.5us half slots)
#define BT_MAX_CLOCK_TIME              ((1L<<28) - 1)

/// Bluetooth Test Mode defines    Bluetooth Test Mode: Table 3.2
#define TEST_MODE_OFF                  0x00
#define TEST_MODE_ACTIVE               0x01

/// Undefined BT clock value
#define LD_CLOCK_UNDEF                 0xFFFFFFFF

/// @} CO_BT
#endif // CO_BT_H_
