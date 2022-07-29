/********************************************************************************************************
 * @file     em_map_config.h
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
 * @file em_map_config.h
 *
 * @brief Mapping of the different descriptors in the exchange memory
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _EM_MAP_CONFIG_H_
#define _EM_MAP_CONFIG_H_



/// Maximum number of ACL links
#ifndef MAX_BT_ACL_LINK
#define MAX_BT_ACL_LINK            	 (3)
#endif
#define MAX_NB_ACTIVE_ACL            MAX_BT_ACL_LINK

#ifndef MAX_NB_SYNC
#define MAX_NB_SYNC                  (2)
#endif

/// Number of RX descriptors
#ifndef BT_RXDESC_NB
#define BT_RXDESC_NB                 (2)
#endif

/// Number of RX/TX ACL data buffers
#ifndef ACL_DATA_BUF_NB_RX
#define ACL_DATA_BUF_NB_RX           (BT_RXDESC_NB)
#endif

#ifndef ACL_DATA_BUF_NB_TX
#define ACL_DATA_BUF_NB_TX           (MAX_NB_ACTIVE_ACL + 2)
#endif

/// Number of TX LMP buffers
#ifndef BT_LMP_BUF_NB_TX
#if (MAX_NB_ACTIVE_ACL > 3)
#define BT_LMP_BUF_NB_TX             (2*MAX_NB_ACTIVE_ACL)
#else // (MAX_NB_ACTIVE_ACL > 3)
#define BT_LMP_BUF_NB_TX             (10)
#endif // (MAX_NB_ACTIVE_ACL > 3)
#endif

/// Maximum ACL Data Packet Size
#ifndef ACL_DATA_BUF_SIZE
#define ACL_DATA_BUF_SIZE            (CO_ALIGN2_HI(DH5_3_PACKET_SIZE))
#endif

/******************************************************************************************/
/* --------------------------     CSB SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Support of Connectionless Slave Broadcast
#ifndef CSB_SUPPORT
#define CSB_SUPPORT  0
#endif

#ifndef PCA_SUPPORT
#define PCA_SUPPORT			0
#endif

#ifndef RW_BT_MWS_COEX
#define RW_BT_MWS_COEX		0
#endif

#endif
