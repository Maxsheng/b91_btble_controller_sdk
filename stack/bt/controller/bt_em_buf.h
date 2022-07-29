/********************************************************************************************************
 * @file     bt_em_buf.h
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

#ifndef _BT_EM_BUF_H_
#define _BT_EM_BUF_H_

#include "em_map_bt.h"
#include "co_list.h"

/// ACL data: handle and data length decoding
#define BT_EM_ACL_DATA_LEN_LSB   (0)
#define BT_EM_ACL_DATA_LEN_MASK  (0x03FF)
/// Packet boundary flag
#define BT_EM_ACL_PBF_LSB        (12)
#define BT_EM_ACL_PBF_MASK       (0x3000)
/// Broadcast flag
#define BT_EM_ACL_BF_LSB         (14)
#define BT_EM_ACL_BF_MASK        (0xC000)

/// EM LMP buffer element
struct bt_em_lmp_buf_elt {
    /// List header
    struct co_list_hdr hdr;
    /// EM buffer pointer
    uint16_t buf_ptr;
    /// Data length
    uint8_t length;
};

/// EM ACL buffer element
struct bt_em_acl_buf_elt {
    /// List header
    struct co_list_hdr hdr;

    /// L2CAP packet start timestamp (in BT half-slots)
    uint32_t l2cap_start_ts;

    /// EM buffer pointer
    uint16_t buf_ptr;
    /// Data length + Data Flags (PBF + BF)
    uint16_t data_len_flags;
}__attribute__ ((aligned (4)));


/// BT EM buffer management environment structure
struct bt_em_buf_env_tag {
    /// List of free LMP RX buffers
    struct co_list lmp_tx_free;

    /// List of free ACL TX buffers
    struct co_list acl_tx_free;

    /// Pool of LMP TX buffers (one for all links)
    struct bt_em_lmp_buf_elt lmp_tx_pool[EM_BT_LMPTXBUF_NB];

    /// Pool of ACL TX buffers (one for all links)
    struct bt_em_acl_buf_elt acl_tx_pool[EM_BT_ACLTXBUF_NB];

    // #if VOICE_OVER_HCI
    // /// Environment for synchronous buffer allocation (one per synchronous link)
    // struct bt_util_buf_sync_tag* sync_tag[MAX_NB_SYNC];
    // #endif // VOICE_OVER_HCI
};

/// BT EM buffer management environment
extern struct bt_em_buf_env_tag bt_em_buf_env;

/**
 ****************************************************************************************
 * @brief Initialize BT EM buffers management system
 *
 * @param[in]   none
 ****************************************************************************************
 */
void bt_em_buf_init(void);

/**
 ****************************************************************************************
 * @brief Allocate a LMP TX buffer
 *
 * @return  Pointer to LMP TX buffer, NULL if no buffer available
 ****************************************************************************************
 */
struct bt_em_lmp_buf_elt *bt_em_buf_lmp_tx_alloc(void);

/**
 ****************************************************************************************
 * @brief Free a LMP TX buffer
 *
 * @param[in]  buf  Pointer to LMP TX buffer
 ****************************************************************************************
 */
void bt_em_buf_lmp_tx_free(uint16_t buf);

/**
 ****************************************************************************************
 * @brief Allocate a ACL RX buffer
 *
 * @return  Pointer to ACL RX buffer, NULL if no buffer available
 ****************************************************************************************
 */
struct bt_em_acl_buf_elt *bt_em_buf_acl_rx_alloc(void);

/**
 ****************************************************************************************
 * @brief Free a ACL RX buffer
 *
 * @param[in]  buf  Pointer to ACL RX buffer
 ****************************************************************************************
 */
void bt_em_buf_acl_rx_free(uint16_t buf);

/**
 ****************************************************************************************
 * @brief Allocate a ACL TX buffer
 *
 * @return  Pointer to ACL TX buffer, NULL if no buffer available
 ****************************************************************************************
 */
struct bt_em_acl_buf_elt *bt_em_buf_acl_tx_alloc(void);


/**
 ****************************************************************************************
 * @brief Retrieve TX buffer Element
 *
 * @return  Pointer to ACL TX buffer element
 ****************************************************************************************
 */
struct bt_em_acl_buf_elt *bt_em_buf_acl_tx_elt_get(uint16_t buf);

/**
 ****************************************************************************************
 * @brief Free a ACL TX buffer
 *
 * @param[in]  buf  Pointer to ACL TX buffer
 ****************************************************************************************
 */
void bt_em_buf_acl_tx_free(uint16_t buf);

#endif /* _BT_EM_BUF_H_ */
