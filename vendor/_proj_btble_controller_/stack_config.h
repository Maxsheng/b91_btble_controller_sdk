/********************************************************************************************************
 * @file     stack_config.h
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

#ifndef STACK_CONFIG_H_
#define STACK_CONFIG_H_

/* HCI TRANSPORT*/
#define HCI_RX_FIFO_SIZE   			696
#define HCI_RX_FIFO_NUM    			16
#define HCI_TX_FIFO_SIZE   			696
#define HCI_TX_FIFO_NUM    			16


#define ENABLE_HCI_TRANSPORT        1
#define HCI_TRANSPORT_MODE          2//1-usb bulk,2-uart
/* HCI DEBUG*/
#define PRINT_TX_FIFO_SIZE 			288
#define PRINT_TX_FIFO_NUM  			32

#define ENABLE_PRINT                0

#if (ENABLE_PRINT == 0)&&(HCI_TRANSPORT_MODE == 2)
#define MYUDB_DISABLE              //disable usb vcd/print
#endif

#if (HCI_TRANSPORT_MODE == 1)

#define MYUDB_DISABLE              //disable usb vcd/print
#define PRINT_MODE                  1

// debug log
#else

#ifndef			PRINT_MODE
#define 		PRINT_MODE          0// 0->USB,1->UART
#endif

#endif

#if ((ENABLE_PRINT==1)&&(PRINT_MODE == 1))

#define UART_PRINTF_BAUDRATE        2000000//2M

#define UART_PRINTF_ID              UART0

#define UART_PRINTF_IO_TX           UART0_TX_PA3

#define UART_PRINTF_IO_RX           UART0_RX_PA4

#define ENABLE_VCD                  0
#elif((ENABLE_PRINT)&&(PRINT_MODE != 1))
#define ENABLE_VCD                  1
#else
#define ENABLE_VCD                  0
#endif

#define VCD_MODE                    0// 0->USB,1->UART defeaut usb vcd

#define ENABLE_BB_SIGNAL            1
#define DEVICE_BID                  0x120


#ifndef  CONTROLLER_DFU_SUPPORT
#define  CONTROLLER_DFU_SUPPORT 1
#endif
/* CLASSIC BLUETOOTH*/
#define ENABLE_BT_CLASSIC           1

#define	BT_ACL_LINK_NUM				1


/* LE */
#define ENABLE_LE                   1

#define MASTER_MAX_NUM				2
#define SLAVE_MAX_NUM				1
#define BLE_MAX_CONN_NUM			(MASTER_MAX_NUM + SLAVE_MAX_NUM)

#define	MAX_BLE_LINK				(BLE_MAX_CONN_NUM+1)//3				//BLE LINK: adv & acl

#define BLE_SLAVE_SMP_ENABLE		1

#define BLMS_MAX_CONN_MASTER_NUM	MASTER_MAX_NUM
#define	BLMS_MAX_CONN_SLAVE_NUM		SLAVE_MAX_NUM

/* temporary macro*/

/* User defined : local name and local bd_addr*/

#define         LOCAL_BT_NAME       "TLK9513B_AML"

#define         LOCAL_BD_ADDR       {0x2c, 0xc0, 0xc3, 0x6f, 0xb6, 0x01}

#endif /* STACK_CONFIG_H_ */
