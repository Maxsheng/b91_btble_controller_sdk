/********************************************************************************************************
 * @file     bt_hci.h
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
 * @file bt_hci.h
 *
 * @brief This file contains definitions related to the HCI module.
 *
 * Copyright (C) OpenC 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef BT_HCI_H_
#define BT_HCI_H_

#include <stdint.h>
#include "fifo.h"
#include "uart.h"
#include "bt/controller/hci_func_map.h"


#define NUM_OF_ACL_PACKET    4

#define NUM_OF_SCO_PACKET    8

#define MAX_SCO_DATA_PACKET_LENGTH   0x40

/**
 *  @brief  Definition for HCI packet type & HCI packet indicator
 */
typedef enum hci_type_e {
	HCI_TYPE_CMD = 0x01,
	HCI_TYPE_ACL_DATA,
	HCI_TYPE_SCO_DATA,
	HCI_TYPE_EVENT,
    HCI_TYPE_ISO_DATA,
} hci_type_t;


extern my_fifo_t uart_rxfifo;
extern my_fifo_t uart_txfifo;
extern my_fifo_t	uart_print_txfifo;

extern uint8_t *bt_ll_hci_tx_buffer_ptr;

extern volatile int hci_rx_status;

extern struct evt_mask hci_rsvd_evt_msk ;
extern struct evt_mask hci_env_tag;
extern struct evt_mask hci_env_tag_page_2;


u8 * bt_hci_get_event_buffer(void);
void bt_hci_release_event_buffer (u8 evt, int n);
int bt_hci_send_data (u32 h, u8 *para, int n);
u8 * my_uart_tx_get_buffer (void);
u8 * my_uart_rx_get_buffer (void);
void bt_ll_hci_tx_process(void);
void hci_start_dma_tx(uart_num_e uart_num,unsigned char * addr);

typedef int return_type_of_hci_command;
typedef return_type_of_hci_command (*func_hci_command_t) (unsigned char *, int);
extern return_type_of_hci_command hci_command_exec (unsigned char *p, int len);


////////////////////////////////////////////////////////////////////////////////////
typedef struct hci_txfifo_read
{
	u8 *	p;
	u16		left;
	u16		len;
	u16		pkt_len_limit;
	u8		pkt_type;
	u8		sel;
	u8		rptr;
	u8		rptr_print;
} __attribute__ ((aligned (4))) hci_txfifo_read_t;

typedef struct hci2uart_cfg
{
	u8*	p;
	u32	tick;
} hci2uart_cfg_t;

#define		HCI_IF_HOST		0
#define		HCI_IF_USB		1
#define		HCI_IF_UART		2

extern my_fifo_t	print_fifo;
extern my_fifo_t	hci_txfifo;
extern my_fifo_t	hci_rxfifo;
extern int			hci_txfifo_idx_ref;
extern hci_txfifo_read_t hci_txfifo_r[3];

void 	my_hci_to_uart(uart_num_e uart);

int  	hci_controller_to_host(void);

void 	hci_host_to_controller(void);

u8* 	hci_rxfifo_get (void);

u8*		hci_txfifo_get (int idx, int nsize);

void	hci_txfifo_next (int idx);

int		hci_txfifo_add ( u8 *para, int n);

int		hci_txfifo_full (void);

int		hci_rxfifo_full (void);

int		hci_print_fifo_full(void);

int     hci_rxfifo_is_empty(void);

void 	hci_set_acl_packet_length_limit (int idx, u8 type, int n);

void	hci_txfifo_set_rptr_reference (int idx);
void hci_event_mask_bit_set(u8 evt_code,bool bit_v);
//void uart0_recieve_process(unsigned int len);
void hci_set_rx_acl_flow_control(u8 flow);

u8  hci_get_rx_acl_flow_control(void);
#endif // BT_HCI_H_
