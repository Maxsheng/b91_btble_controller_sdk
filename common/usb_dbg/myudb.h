/********************************************************************************************************
 * @file     myudb.h
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

#ifndef		__MYUDB_H__
#define		__MYUDB_H__
#pragma once

#include "../../tl_common.h"

#include "log_def_stack.h"

//#include "application/tws/log_def_tws.h"

#include "../../incs/co_bt_defines.h"


#ifndef			VCD_EN
#define			VCD_EN						1
#endif

#ifndef			VCD_BLE_EN
#define			VCD_BLE_EN					0
#endif

#ifndef			DUMP_STR_EN
#define 		DUMP_STR_EN                 1
#endif

#define			SL_STACK_VCD_EN				1
/* Timing VCD id enable */
#define			SL_STACK_TIMING_EN			0
#define			SL_STACK_RESET_EN			1
/* schedule VCD id enable */
#define			SL_STACK_SCH01_EN			1
#define			SL_STACK_SCH02_EN			0
/* frame VCD id enable */
#define			SL_STACK_FRAME_EN			1
#define			SL_STACK_FRAME_ST_EN		0
#define			SL_STACK_RADIO_EN			0
#define			SL_STACK_FLOW_EN			0
/* access link VCD id enable */
#define			SL_STACK_INQ_EN				1
#define			SL_STACK_INQSCAN_EN			1
#define			SL_STACK_PAGE_EN			1
#define			SL_STACK_PAGESCAN_EN		1

/* acl-c link VCD id enable */
#define			SL_STACK_ACL_EN				1
#define			SL_STACK_LMP_EN				1
#define			SL_STACK_CON_EN				0
#define			SL_STACK_RSW_EN				0
#define			SL_STACK_SNIFF_EN			0
#define			SL_STACK_AFH_EN             0
#define			SL_STACK_AFH_CLS_EN         0
#define			SL_STACK_DETACH_EN			1

/* sco/e-sco link VCD id enable */
#define			SL_STACK_SCO_EN				0
/* authen/pair/encrypt VCD id enable */
#define			SL_STACK_AUTH_EN			0
#define			SL_STACK_PAIR_EN			0
#define			SL_STACK_ENCRYPT_EN			0
/* PA VCD id enable */
#define			SL_STACK_PA_EN				0
#define			SL_STACK_TEST_EN			0
/* application VCD id enable */
#define			SL_APP_MUSIC_EN				0
#define			SL_APP_MUSIC_02_EN			0
#define			SL_APP_AUDIO_EN				0
#define			SL_APP_EC_EN				0
#define			SL_APP_SP_EN				0
#if (ENABLE_PRINT)
typedef void (*my_dump_str_data_callback_t)(u8 en,char *str, u8 *ph, u16 n);
my_dump_str_data_callback_t my_dump_str_data;
typedef void (*my_dump_str_u32s_callback_t)(u8 en,char *str, u32 d0, u32 d1, u32 d2, u32 d3);
my_dump_str_u32s_callback_t my_dump_str_u32s;
#else
#define	usb_send_str(s)
#define	usb_send_data(p,n)
#define my_dump_str_data(en,s,p,n)
#define my_dump_str_u32s(en,s,d0,d1,d2,d3)
#define my_uart_send_str_data
#define	my_uart_send_str_int
#endif
/*
#if ((DUMP_STR_EN)&&(ENABLE_PRINT))
	#if(PRINT_MODE==1)
	#define my_dump_str_data(en,s,p,n)		if(en){printf_str_data(s,(u8*)(p),n);}
	#define my_dump_str_u32s(en,s,d0,d1,d2,d3)		if(en){printf_str_u32s(s,(u32)(d0),(u32)(d1),(u32)(d2),(u32)(d3));}
	#elif(PRINT_MODE==0)
		#define	usb_send_str(s)	usb_send_str_data(s, 0, 0)
		#define	usb_send_data(p,n) usb_send_str_data(0,p,n)
		#define my_dump_str_data(en,s,p,n)		if(en){usb_send_str_data(s,(u8*)(p),n);}
		#define my_dump_str_u32s(en,s,d0,d1,d2,d3)		if(en){usb_send_str_u32s(s,(u32)(d0),(u32)(d1),(u32)(d2),(u32)(d3));}
	#endif
#else
#define	usb_send_str(s)
#define	usb_send_data(p,n)
#define my_dump_str_data(en,s,p,n)
#define my_dump_str_u32s(en,s,d0,d1,d2,d3)
#define my_uart_send_str_data
#define	my_uart_send_str_int
#endif
*/
#define ACL_RXFIFO_SIZE		(DH5_3_PACKET_SIZE+ 8)
#define ACL_RXFIFO_NUM		16

#define ACL_TXFIFO_SIZE		(512 + 8)
#define ACL_TXFIFO_NUM		4

typedef int (*func_myudb_hci_cmd_cb_t) (unsigned char *, int);


void 	myudb_register_hci_cb (void *p);
void    myudb_register_hci_debug_cb (void *p);
void 	myudb_register_hci_eq_cb (void *p);

void    myudb_usb_init(u16 id, void *p, void * p_print);

void 	myudb_usb_bulkout_ready ();

void 	myudb_usb_handle_irq(void);

int 	myudb_usb_to_hci (u8 *p);

void 	usb_send_status_pkt(u8 status, u8 buffer_num, u8 *pkt, u16 len);

void 	myudb_capture_enable (int en);

void 	usb_send_str_data (char *str, u8 *ph, int n);
int	usb_send_str_int (char *str,int w);

void 	usb_send_str_u32s (char *str, u32 d0, u32 d1, u32 d2, u32 d3);

void usb_send_str_data_cb (u8 en, char *str, u8 *ph, int n);

void usb_send_str_u32s_cb (u8 en,char *str, u32 d0, u32 d1, u32 d2, u32 d3);

void	myudb_set_txfifo_local ();

void	myudb_set_txfifo (void *p);

void 	myudb_set_acl_packet_length_limit (unsigned char type, int n);

///////////////////////////////////////////////////////////////////////////////////////
#define			GLOBAL_INT_DISABLE()		u32 rie = core_disable_interrupt ()
#define			GLOBAL_INT_RESTORE()		core_restore_interrupt(rie)
#define			LOG_EVENT_TIMESTAMP		0
#define			LOG_DATA_B1_0			0
#define			LOG_DATA_B1_1			1

#define			get_systemtick()  	    stimer_get_tick()


#if(VCD_MODE)
#define			log_uart(d)				uart_send_byte_dma(1,d)
#define         DEBUG_PORT				GPIO_PE2
#define			log_ref_gpio_h()		gpio_set_high_level(DEBUG_PORT)
#define			log_ref_gpio_l()		gpio_set_low_level(DEBUG_PORT)
#else
	#if(ENABLE_VCD)
	#define			log_uart(d)				reg_usb_ep8_dat=d
	#else
	#define			log_uart(d)				(0)
	#endif
#define			log_ref_gpio_h()
#define			log_ref_gpio_l()
#endif


#if TWS_165_EN || HEADSET_UI_EN
#define 		DEBUG_PORT_TX_EN_PIN	GPIO_PD3
#define 		DEBUG_PORT_RX_EN_PIN	GPIO_PD4
#define 		DEBUG_PORT_RX_WIN_PIN   GPIO_PD5
#define 		DEBUG_PORT_RX_SYNC_PIN	GPIO_PD6
#else
#define 		DEBUG_PORT_TX_EN_PIN	GPIO_PB3
#define 		DEBUG_PORT_RX_EN_PIN	GPIO_PB4
#define 		DEBUG_PORT_RX_WIN_PIN   GPIO_PB5
#define 		DEBUG_PORT_RX_SYNC_PIN	GPIO_PB6

#endif




#define	log_hw_ref()	if(VCD_EN){GLOBAL_INT_DISABLE();log_ref_gpio_h();log_uart(0x20);int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);log_ref_gpio_l();GLOBAL_INT_RESTORE();}

// 4-byte sync word: 00 00 00 00
#define	log_sync(en)	if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0);log_uart(0);log_uart(0);log_uart(0);GLOBAL_INT_RESTORE();}
//4-byte (001_id-5bits) id0: timestamp align with hardware gpio output; id1-31: user define
#define	log_tick(en,id)	if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0x20|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);GLOBAL_INT_RESTORE();}

//1-byte (000_id-5bits)
#define	log_event(en,id) if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0x00|(id&31));GLOBAL_INT_RESTORE();}

//1-byte (01x_id-5bits) 1-bit data: id0 & id1 reserved for hardware
#define	log_task(en,id,b)	if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(((b)?0x60:0x40)|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);GLOBAL_INT_RESTORE();}

//2-byte (10-id-6bits) 8-bit data
#define	log_b8(en,id,d)	if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0x80|(id&63));log_uart(d);GLOBAL_INT_RESTORE();}

//3-byte (11-id-6bits) 16-bit data
#define	log_b16(en,id,d) if(VCD_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0xc0|(id&63));log_uart(d);log_uart((d)>>8);GLOBAL_INT_RESTORE();}




//BLE used only //
#define	log_sync_mainloop(en)	if(VCD_BLE_EN&&en) {GLOBAL_INT_DISABLE();log_uart(0);log_uart(0);log_uart(0);log_uart(0);GLOBAL_INT_RESTORE();}

#define	log_tick_mainloop(en,id) if(VCD_EN&&en) {my_irq_disable();log_uart(0x20|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);my_irq_restore();}

#define	log_tick_irq(en,id)		if(VCD_BLE_EN&&en) {log_uart(0x20|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);}

#define	log_event_irq(en,id) 	if(VCD_BLE_EN&&en) {log_uart(0x00|(id&31));}

#define	log_task_irq(en,id,b)	if(VCD_BLE_EN&&en) {log_uart(((b)?0x60:0x40)|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);}

#define	log_b8_irq(en,id,d)		if(VCD_BLE_EN&&en) {log_uart(0x80|(id&63));log_uart(d);}

#define	log_b16_irq(en,id,d)	if(VCD_BLE_EN&&en) {log_uart(0xc0|(id&63));log_uart(d);log_uart((d)>>8);}


#endif
