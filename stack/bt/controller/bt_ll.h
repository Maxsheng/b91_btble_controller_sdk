/********************************************************************************************************
 * @file     bt_ll.h
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

#ifndef _BT_LL_H_
#define _BT_LL_H_
#include "bt_ll_role_switch.h"
#include "bt_ll_sniff.h"
#include "bt_ll_test.h"
#include "bt_ll_encrypt.h"
#include "bt_ll_pair.h"
#include "bt_ll_acl.h"



#ifndef LOCAL_CLASS_OF_DEVICE
#define         LOCAL_CLASS_OF_DEVICE   0x240404
#endif

#define         LOCAL_BT_VERSNR         		0x0b    // bt core spec 5.2
#define         LOCAL_BT_COMPID         		0x0211  // Telink semiconductor
#define         LOCAL_BT_SUBVERSNR      		0x7D30
#define         LMP_PAL_VERSION     		    0x0b
#define         LMP_PAL_SUBVERSION     		    11
#define         HCI_REVISION					12

#define         LD_DELAY                2
#define         LD_PROG_DELAY			3
/// Offset of UAP part in BDADDR buffer
#define         LD_UAP_OFFSET           3

#define         LD_BCAST_LTADD          0
#define         LD_FIRST_LTADD          1
#define         LD_LAST_LTADD           7

#define			OFFSET_CALIB_SEL		0



////////////////////  Device Info //////////////////////////
typedef struct bt_ll_device_env {
    uint32_t iocap;
    uint32_t class;
    struct bd_name name_loc;
    uint8_t bdaddr[6];
    uint8_t bch[8];
    uint8_t simple_pairing_en;
} __attribute__ ((aligned (4))) bt_ll_device_env_t;

extern bt_ll_device_env_t env_dev;



void bt_ll_get_bd_addr(uint8_t *bdAddr);
int bt_ll_set_bd_addr(uint8_t *bdAddr);

void bt_ll_set_class_of_device(uint32_t class);


typedef enum
{
	ACL_RX_OFFSET_INVALID = 0x7FFF,
} ACL_STAT_E;


typedef  enum{
	NO_DEVICE,
	LOCAL_DEVICE,
	REMOTE_DEVICE,
}LINK_INITIATOR_E;




void bt_ll_low_latency_func_register (int en);


extern uint8_t *bt_ll_hci_tx_buffer_ptr;

int			bt_ll_lmp_push_packet (int Lid, int len, uint8_t *p);
uint8_t * 	bt_ll_lmp_pop_packet (int Lid);
uint8_t * 	bt_ll_lmp_get_packet_buffer (int Lid);
int			bt_ll_lmp_release_packet_buffer (int Lid, int len);
int			bt_ll_lmp_tx_buffer_available (int Lid);

int bt_ll_task_acl (void);

#define	FLAG_BT_ACL_CONNHDL			0x0010
#define	FLAG_BT_SCO_CONNHDL			0x0800


static inline uint16_t bt_ll_acl_conhdl_get_by_lid(uint8_t Lid)
{
	return Lid | FLAG_BT_ACL_CONNHDL;
}

static inline int bt_ll_acl_get_lid_from_conhdl (uint16_t conhdl)
{
	return conhdl & 7;
}

static inline uint8_t bt_ll_acl_get_rssi (uint8_t link_id)
{
	bt_ll_acl_env_t *acl_par =bt_ll_acl_env_get(link_id);
	return acl_par->rssi;
}

extern void  bt_context_store(unsigned long * bt_reg, unsigned long * ip_reg, unsigned long * modem_reg, unsigned long * radio_reg, unsigned long * pdzb_reg);
extern void  bt_context_restore(unsigned long * bt_reg, unsigned long * ip_reg, unsigned long * modem_reg, unsigned long * radio_reg, unsigned long * pdzb_reg);

extern unsigned long  DEBUG_BTREG_INIT[];


extern unsigned long  DEBUG_IPREG_INIT[];


extern unsigned long  DEBUG_TL_MODEMREG_INIT[];


extern unsigned long  DEBUG_TL_RADIOREG_INIT[];


extern unsigned long  DEBUG_TL_PDZB_INIT[];

void bt_ll_init (void);
void bt_ll_testcase(void);
uint8_t is_switch_instant_coming(uint32_t sw_instant);


void bt_ll_system_tick_isr(void);
_attribute_ram_code_ void bt_core_isr(void);

void bt_ll_page_frm_isr(uint32_t slot);
void bt_ll_inq_frm_isr(uint32_t slot);
void bt_ll_main_loop(void);

int32_t btc_task_pending(void);

#endif // _BT_LL_H_
