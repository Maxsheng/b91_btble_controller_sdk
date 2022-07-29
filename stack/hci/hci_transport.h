/********************************************************************************************************
 * @file     hci_transport.h
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

#ifndef HCI_TRANSPORT_H_
#define HCI_TRANSPORT_H_

typedef int (*uart_init_callback_t)(void);
extern uart_init_callback_t uart_dmatx_irq_init_cb;
#define PENDING_BUF_LEN 4096  //  4K
#if(ENABLE_HCI_TRANSPORT&&(HCI_TRANSPORT_MODE==2))

#define UART_DMA_SIZE  PENDING_BUF_LEN/4//  0x400

extern  unsigned char  pending_dma_buf[PENDING_BUF_LEN];

#define UART_BAUDRATE_DYNAMIC_ADJUST 1

/////////////////////  FLOW CONTROL //////////////////////////////////
#define	UART_FLOW_CTR				0
#define CTS_STOP_VOLT               1 //0 :Low level stops TX.  1 :High level stops TX.
#define	UART_MANUAL_FLOW_CTR_RTS_STOP				gpio_set_high_level(UART_RTS_PIN)
#define	UART_MANUAL_FLOW_CTR_RTS_START				gpio_set_low_level(UART_RTS_PIN)

//baudrate of UART
#define UART_BAUDRATE   115200
/*! HCI Transport configuration  */
#if 0//UART0
#define UART_ID         UART0
#define UART_TX_PIN     UART0_TX_PD2
#define UART_RX_PIN     UART0_RX_PD3
#define UART_CTS_PIN    UART0_CTS_PA1
#define UART_RTS_PIN    UART0_RTS_PA2
#define UART_IRQ        IRQ19_UART0
#define UART_IRQHandler uart0_irq_handler

#else
#define UART_ID         UART1
#define UART_TX_PIN     UART1_TX_PE0
#define UART_RX_PIN     UART1_RX_PE2
#define UART_CTS_PIN    UART1_CTS_PE1
#define UART_RTS_PIN    UART1_RTS_PE3
#define UART_IRQ        IRQ18_UART1
#define UART_IRQHandler uart1_irq_handler

#endif
#define UART_DMA_TX     DMA3
#define UART_DMA_RX     DMA2
/**
 * @brief		dma tx init when reset controller pending tx irq.
 * @param[in]	none
 * @return      none
 */
extern void transport_uart_dmatx_irq_init(void);


#elif(ENABLE_HCI_TRANSPORT&&(HCI_TRANSPORT_MODE==1))

void hci_usb_handle_irq(void);
void hci_usb_init(void);


#endif
#if ((PRINT_MODE == 1)&&(ENABLE_PRINT))
/**
 * @brief		uart log init when reset usb log disable
 * @param[in]	none
 * @return      none
 */
extern void uart_dbg_init(void);
/**
 * @brief		printf string data
 * @param[in]	[*str][*ph][n]
 * @return      none
 */
extern void printf_str_data(u8 en,char *str, u8 *ph, u16 n);
/**
 * @brief		printf string data
 * @param[in]	[*str][d0][d1][d2][d3]
 * @return      none
 */
extern void printf_str_u32s (u8 en,char *str, u32 d0, u32 d1, u32 d2, u32 d3);
/**
 * @brief		loop output uart log
 * @param[in]	none
 * @return      none
 */
extern void printf_process(void);
#endif
#endif /* HCI_TRANSPORT_H_ */
