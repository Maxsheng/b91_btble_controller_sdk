/********************************************************************************************************
 * @file     bt_hw.h
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

#ifndef _BT_HW_H_
#define _BT_HW_H_
#include "incs.h"

#include "drivers.h"
#include "bt_ll_acl.h"
/// Prefetch Abort time (in us)
#define BT_PREFETCHABORT_TIME_US  (150) // unit: us

/// Prefetch time (in us)
#define BT_PREFETCH_TIME_US       (140) // unit: us

#define	BT_SNIFF_RXWIN_SIZE			800 // unit: us

#define	SLOT_OFFSET_COMPENSTATE		0  // unit: us

#define PAGESCAN_WIN_SIZE           100 // unit: us

#define NORMAL_WIN_SIZE             14	// unit: hus		/// Normal Rx window size

#define	SYNC_WIN_SIZE				56  // unit: hus

#define RXWIN_SIZE          		((NORMAL_WIN_SIZE/2 + BT_RADIO_RX_PATH_DELAY + SYNC_WIN_SIZE + 1)/2*2)

#define	BT_RXBIT_FCNT_REF			(625 - (NORMAL_SYNC_POS + BT_RADIO_RX_PATH_DELAY) * 2 - SYNC_WIN_SIZE)

#define	TICK_PER_US					16
#define MAX_SLOT_CLOCK      		((1L<<27) - 1)
#define MAX_HSLOT_CLOCK      		((1L<<28) - 1)
/// Mode
enum BT_HW_PROG_TYPE {
    BT_HW_PROG_BLE,
    BT_HW_PROG_BT,
};

/// Frame type
enum BT_HW_FRAME_TYPE {
    BT_HW_FRAME_TYPE_NORMAL    = 0x00,
    BT_HW_FRAME_TYPE_SNIFF     = 0x01,
    BT_HW_FRAME_TYPE_ESCO      = 0x02,
    BT_HW_FRAME_TYPE_CSB       = 0x03,
    BT_HW_FRAME_TYPE_ESCO_RETX = 0x04,
};

enum bt_hw_prio_dft {
    /// ACL event default priority
    RWIP_PRIO_ACL_DFT               = 5,
    /// ACL event priority with activity
    RWIP_PRIO_ACL_ACT               = 10,
    /// ACL Role Switch event default priority
    RWIP_PRIO_ACL_RSW               = 20,
    /// ACL sniff event default priority
    RWIP_PRIO_ACL_SNIFF_DFT         = 15,
    /// ACL sniff transition event default priority
    RWIP_PRIO_ACL_SNIFF_TRANS       = 10,
    /// SCO event default priority
    RWIP_PRIO_SCO_DFT               = 18,
    /// Broadcast ACL event default priority
    RWIP_PRIO_BCST_DFT              = 5,
    /// Broadcast ACL event with LMP activity priority
    RWIP_PRIO_BCST_ACT              = 10,
    /// CSB RX event default priority
    RWIP_PRIO_CSB_RX_DFT            = 10,
    /// CSB TX event default priority
    RWIP_PRIO_CSB_TX_DFT            = 10,
    /// Inquiry event default priority
    RWIP_PRIO_INQ_DFT               = 5,
    /// Inquiry Scan event default priority
    RWIP_PRIO_ISCAN_DFT             = 5,
    /// Page event default priority
    RWIP_PRIO_PAGE_DFT              = 8,
    /// Page first packet event default priority
    RWIP_PRIO_PAGE_1ST_PKT          = 20,
    /// PCA event default priority
    RWIP_PRIO_PCA_DFT               = 20,
    /// Page scan event default priority
    RWIP_PRIO_PSCAN_DFT             = 8,
    /// Page scan event priority increment when canceled
    RWIP_PRIO_PSCAN_1ST_PKT         = 20,
    /// Synchronization Scan event default priority
    RWIP_PRIO_SSCAN_DFT             = 10,
    /// Synchronization Train event default priority
    RWIP_PRIO_STRAIN_DFT            = 10,
};


/// Structure of a frame element
struct bt_hw_prog_frm_elt {
    /// Timestamp of the programmed frame (in BLE half slots, based on local clock)
    uint32_t timestamp;
    /// Dummy value (to be reported to the driver)
    uint32_t dummy;
    /// Indicate if the frame is valid (programmed and not skipped or finished)
    uint8_t valid;
};

/// SCH_PROG environment structure
typedef struct bt_hw_prog_env {
    /// Frame elements pool
    struct bt_hw_prog_frm_elt tab[REG_EM_ET_SIZE];

    /// Exchange table index of the oldest entry currently used by the HW
    uint8_t et_idx_current;

    /// Next exchange table index to program
    uint8_t et_idx_next_prog;

    /// Number of programmed frames
    uint8_t nb_prog;
} bt_hw_prog_env_t;

extern bt_hw_prog_env_t bt_hw_prog_env;

extern bt_hw_prog_params_t prog_par;

void bt_core_init(void);
_attribute_bt_retention_code_ void bt_hw_prog(bt_hw_prog_params_t *params);
_attribute_ram_code_ void bt_hw_access_compute_rx_fhs(uint16_t fhs_buf_ptr, uint8_t *bch_ptr, uint8_t *bd_addr, uint8_t *class_of_dev, uint8_t *lt_addr, uint32_t *clk_frm, uint8_t *page_scan_rep_mode);
_attribute_bt_retention_code_ u32 bt_hw_get_et_next_slot (uint32_t hslot);
_attribute_bt_retention_code_ void bt_hw_acl_prog(uint8_t link_id, uint32_t hslot);

#endif /* _BT_HW_H_ */
