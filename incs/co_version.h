/********************************************************************************************************
 * @file     co_version.h
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
 * @file co_version.h
 *
 * @brief Version definitions for BT5.0
 *
 * Copyright (C) RivieraWaves 2009-2018
 *
 *
 ****************************************************************************************
 */

#ifndef _CO_VERSION_H_
#define _CO_VERSION_H_
/**
 ****************************************************************************************
 * @defgroup CO_VERSION Version Defines
 * @ingroup COMMON
 *
 * @brief Bluetooth Controller Version definitions.
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
//#include "rwip_config.h"        // SW configuration options


#if (BLE_HW_50_ISO == 0)        // BLE 5.0 HW
/// RWBT SW Major Version
#define RWBT_SW_VERSION_MAJOR                   (RW_BT50_VERSION)
/// RWBT SW Minor Version
#define RWBT_SW_VERSION_MINOR                   0
/// RWBT SW Build Version
#define RWBT_SW_VERSION_BUILD                   19
/// RWBT SW Major Version
#define RWBT_SW_VERSION_SUB_BUILD               0

#else // (BLE_HW_50_ISO == 1)       // BLE 5.0 + ISO HW
/// RWBT SW Major Version
#define RWBT_SW_VERSION_MAJOR                   (RW_BT50_VERSION)
/// RWBT SW Minor Version
#define RWBT_SW_VERSION_MINOR                   0xA
/// RWBT SW Build Version
#define RWBT_SW_VERSION_BUILD                   9
/// RWBT SW Major Version
#define RWBT_SW_VERSION_SUB_BUILD               0
#endif // (BLE_HW_50_ISO == 0)



/// @} CO_VERSION


#endif // _CO_VERSION_H_
