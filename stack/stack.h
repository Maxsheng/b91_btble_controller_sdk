/********************************************************************************************************
 * @file     stack.h
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

#ifndef _BT_STACK_H_
#define _BT_STACK_H_

#define     TICK_PER_US             16
#define     TICK_PER_HALF_US        (TICK_PER_US/2)
#define 	PAGE_INQ_INV			56// unit:slot



#include <stdbool.h>
#include "tl_common.h"

//#include "bt/host/hci/hci_event.h"

#include "hci/hci.h"
#include "hci/hci_const.h"
#include "hci/hci_cmd.h"
#include "hci/hci_event.h"
#include "hci/hci_transport.h"
#include "hci/bt_hci.h"

#include "bt/controller/bt_em_buf.h"
#include "bt/controller/bt_hw.h"
#include "bt/controller/bt_acl_master.h"
#include "bt/controller/bt_ll_pagescan.h"
#include "bt/controller/bt_ll_page.h"
#include "bt/controller/bt_ll_inqscan.h"
#include "bt/controller/bt_ll_inq.h"
#include "bt/controller/bt_ll_access.h"

#include "bt/controller/bt_ll_afh.h"
#include "bt/controller/bt_ll_channel_classification.h"
#include "bt/controller/bt_ll_chn_cls.h"
#include "bt/controller/bt_ll_bch.h"
#include "bt/controller/bt_ll_tcf.h"
#include "bt/controller/bt_ll.h"
#include "bt/controller/bt_ll_lmp.h"
#include "bt/controller/bt_ll_lmp_map.h"
#include "bt/controller/bt_ll.h"
#include "bt/controller/bt_ll_role_switch.h"
#include "bt/controller/bt_ll_feature.h"
#include "bt/controller/bt_ll_disconnect.h"
#include "bt/controller/bt_ll_sniff.h"
#include "bt/controller/bt_ll_sleep.h"
#include "bt/controller/bt_ll_name.h"
#include "bt/controller/bt_ll_qos.h"
#include "bt/controller/btc_task.h"
#include "bt/controller/bt_ll_timer.h"
#include "bt/controller/bt_ll_authen.h"
#include "bt/controller/bt_ll_pair.h"
#include "bt/controller/bt_ll_encrypt.h"
#include "bt/controller/bt_crypto.h"
#include "bt/controller/bt_spcomputation.h"
#include "bt/controller/bt_sp.h"

#include "bt/controller/co_hci.h"
#include "bt/controller/tws_bt_controller.h"
#include "bt/controller/bt_ll_packet_type.h"
#include "bt/controller/bt_ll_power_control.h"

#include "btble/btble.h"
#include "btble/btble_stack.h"
#include "btble/btble_scheduler.h"
#include "btble/btble_pm.h"

#include "bt/controller/bt_ll_sco.h"
#include "bt/controller/bt_ll_test.h"
//#include "application/tws/tws.h"

#include "algorithm/ecc/ecc_ll.h"

#ifndef	 ACL_RX_RAMCODE_EN
#define	ACL_RX_RAMCODE_EN		0
#endif

#if ACL_RX_RAMCODE_EN
	#define  ACL_RX_RAMCODE	_attribute_ram_code_
#else
	#define  ACL_RX_RAMCODE
#endif


#ifndef	DUMP_ACL_MSG
#define DUMP_ACL_MSG    1
#endif

#ifndef	DUMP_HCI_MSG
#define DUMP_HCI_MSG    1
#endif

#ifndef	DUMP_BTM_MSG
#define DUMP_BTM_MSG    1
#endif

#ifndef	DUMP_LMP_MSG
#define DUMP_LMP_MSG    1
#endif

#ifndef	DUMP_EVT_MSG
#define DUMP_EVT_MSG    1
#endif

#ifndef	DUMP_APP_MSG
#define DUMP_APP_MSG    1
#endif

#ifndef	DUMP_SDP_MSG
#define DUMP_SDP_MSG    0
#endif

#ifndef	DUMP_SDP_MSSG_ALWAYS
#define DUMP_SDP_MSSG_ALWAYS    1
#endif

#ifndef	DUMP_ENC_MSG
#define DUMP_ENC_MSG    1
#endif

#ifndef	DUMP_AUTH_MSG
#define DUMP_AUTH_MSG    1
#endif

#ifndef	DUMP_PAIR_MSG
#define DUMP_PAIR_MSG    1
#endif

#ifndef	DUMP_RSW_MSG
#define DUMP_RSW_MSG    1
#endif


#ifndef	DUMP_QOS_MSG
#define DUMP_QOS_MSG    1
#endif

#ifndef	DUMP_AFH_MSG
#define DUMP_AFH_MSG    1
#endif

#ifndef	DUMP_NAME_MSG
#define DUMP_NAME_MSG    1
#endif


#ifndef	DUMP_PAGE_MSG
#define DUMP_PAGE_MSG    1
#endif


#ifndef	DUMP_DETACH_MSG
#define DUMP_DETACH_MSG    1
#endif


#ifndef	DUMP_TIMER_MSG
#define DUMP_TIMER_MSG    1
#endif

#ifndef	DUMP_RESET_CORE_MSG
#define DUMP_RESET_CORE_MSG    0
#endif

#ifndef	DUMP_SNIFF_MSG
#define DUMP_SNIFF_MSG    1
#endif

#ifndef	DUMP_SCO_MSG
#define DUMP_SCO_MSG     1
#endif

#ifndef	DUMP_TASK_MSG
#define DUMP_TASK_MSG    1
#endif


#ifndef	DUMP_RFCOMM_MSG
#define DUMP_RFCOMM_MSG   0
#endif

#ifndef	DUMP_RFCOMM_MSSG_ALWAYS
#define DUMP_RFCOMM_MSSG_ALWAYS   1
#endif

#ifndef	DUMP_RFCOMM2_MSG
#define DUMP_RFCOMM2_MSG   1
#endif

#ifndef	DUMP_L2CAP_MSG
#define DUMP_L2CAP_MSG   	0
#endif

#ifndef	DUMP_L2CAP_ERR_MSG
#define DUMP_L2CAP_ERR_MSG   1
#endif

#ifndef	DUMP_L2CAP_MSG_ALWAYS
#define DUMP_L2CAP_MSG_ALWAYS   1
#endif



#define DUMP_HFP_MSG        0

#ifndef DUMP_GOEP_MSG
#define DUMP_GOEP_MSG         1
#endif

#ifndef	DUMP_HFP_MSSG_ALWAYS
#define DUMP_HFP_MSSG_ALWAYS    1
#endif

#define DUMP_A2DP_MSG       0

#ifndef	DUMP_A2DP_MSSG_ALWAYS
#define DUMP_A2DP_MSSG_ALWAYS    1
#endif

#define DUMP_AVRCP_MSG      0

#ifndef APP_CFG_DUMP_EN
#define	APP_CFG_DUMP_EN		1
#endif

#ifndef SUCCESS
#define SUCCESS     0
#endif

#ifndef FAILURE
#define FAILURE     1
#endif

#ifndef FAILURE_NO_CREDIT
#define FAILURE_NO_CREDIT     1
#endif

#ifndef BTBLE_DUALMODE_EN
#define BTBLE_DUALMODE_EN     0
#endif

#ifndef	SSP_PAIRING_TEST
#define SSP_PAIRING_TEST     0
#endif
#ifndef	RF_MODE_IQ_ONLY_EN
#define RF_MODE_IQ_ONLY_EN     0  // 1: JUST ONLY USE IQ MODE, 0: BOTH USE IQ AND TWO POINT.
#endif



#ifndef	CODEC_MODE_CHG_STS_EN
#define CODEC_MODE_CHG_STS_EN     0
#endif

extern my_fifo_t	uart_txfifo;
/*
#if ENABLE_PRINT
	#if(PRINT_MODE==1)
	#define		hci_dump_msg_data(en,s,d,l)  	if(en) {printf_str_data(s, (uint8_t*)d, l>16 ? 16 : l);}
	#define     hci_dump_msg_str(en,s)          if(en) {printf_str_u32s(s,0,0,0,0);}
	#elif(PRINT_MODE==0)
	#define		hci_dump_msg_data(en,s,d,l)  	if(en) {usb_send_str_data (s, (uint8_t*)d, l>16 ? 16 : l);}
	#define     hci_dump_msg_str(en,s)          if(en) {usb_send_str_data(s,0,0);}
	#define     hci_dump_msg_str_int(en,s,d)    if(en) {usb_send_str_int(s,d);}
	#endif
#else
#define		hci_dump_msg_data(en,s,d,l)  	
#define     hci_dump_msg_str(en,s)        
#define     hci_dump_msg_str_int(en,s,d)   
#define     sys_warning()

#endif
*/
#endif // _BT_STACK_H_
