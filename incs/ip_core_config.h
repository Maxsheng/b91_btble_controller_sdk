/********************************************************************************************************
 * @file     ip_core_config.h
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
 * @file ip_core_config.h
 *
 * @brief Mapping of the exchange memory
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 ****************************************************************************************
 */

#ifndef _IP_CPRE_CONFIG_H_
#define _IP_CPRE_CONFIG_H_

#define BT_EMB_PRESENT              1
#define BLE_EMB_PRESENT             0
#define HCI_PRESENT                 1
#define BLE_HOST_PRESENT            0
#define BLE_APP_PRESENT             0

/******************************************************************************************/
/* -----------------------   SLOT AVAILABILITY MASKS   -----------------------------------*/
/******************************************************************************************/

/// Maximum support peer SAM map size
#define RW_MAX_PEER_SAM_MAP_SLOTS      (256)
#define RW_PEER_SAM_MAP_LEN      (RW_MAX_PEER_SAM_MAP_SLOTS/4) // 2-bit field per slot

/******************************************************************************************/
/* --------------------   SECURE CONNECTIONS SETUP  --------------------------------------*/
/******************************************************************************************/
#define CFG_SEC_CON               1
#define  CFG_ECC_16_BITS_ALGO     1
#if defined(CFG_SEC_CON)
#define SECURE_CONNECTIONS        (1)
#if defined(CFG_ECC_16_BITS_ALGO)
#define ECC_MULT_ALGO_TYPE        (16)
#else // !defined(CFG_ECC_16_BITS_ALGO)
#define ECC_MULT_ALGO_TYPE        (32)
#endif // defined(CFG_ECC_16_BITS_ALGO)
#if defined(CFG_CRYPTO_UT)
#define CRYPTO_UT                 (1)
#else //defined(CFG_CRYPTO_UT)
#define CRYPTO_UT                 (0)
#endif //defined(CFG_CRYPTO_UT)
#else // !defined(CFG_SEC_CON)
#define SECURE_CONNECTIONS        (1)
#define CRYPTO_UT                 (0)
#endif // defined(CFG_SEC_CON)

/// Coexistence Packet traffic information - Index Definitions
enum rwbt_pti_config_idx {
    BT_PTI_PAGERES_IDX  ,
    BT_PTI_MCONPEND_IDX ,
    BT_PTI_PSCANRES_IDX ,
    BT_PTI_SCONPEND_IDX ,
    BT_PTI_MSSWITCH_IDX ,
    BT_PTI_SNIFFANC_IDX ,
    BT_PTI_BEACON_IDX   ,
    BT_PTI_SNIFFATT_IDX ,
    BT_PTI_ACCWIN_IDX   ,
    BT_PTI_POLLINT_IDX  ,
    BT_PTI_PAGE_IDX     ,
    BT_PTI_PSCAN_IDX    ,
    BT_PTI_INQ_IDX      ,
    BT_PTI_INQSCAN_IDX  ,
    BT_PTI_INQRES_IDX   ,
    BT_PTI_SCORSVD_IDX  ,
    BT_PTI_SCORETX_IDX  ,
    BT_PTI_ACLTX_IDX    ,
    BT_PTI_ACLRETX_IDX  ,
    BT_PTI_LMPTX_IDX    ,
    BT_PTI_BCAST_IDX    ,
    BT_PTI_CONNECT_IDX  ,
    BT_PTI_IDX_MAX
};

#endif /* _IP_CPRE_CONFIG_H_ */