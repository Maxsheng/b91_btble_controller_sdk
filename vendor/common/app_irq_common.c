/********************************************************************************************************
 * @file     app_irq_common.c
 *
 * @brief    This is the source file for BTBLE SDK
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

#include "tl_common.h"
#include "drivers.h"

#include "common/usb_dbg/myudb.h"

volatile int dma_irq_cnt=0;
volatile int dma_irq_rx_cnt=0;
volatile int dma_irq_tx_cnt=0;



void dma_irq_handler(void)
{
	dma_irq_cnt++;
	unsigned char uart_dma_irq_src = reg_dma_tc_isr;
	plic_interrupt_complete(IRQ5_DMA);
	if((uart_dma_irq_src & FLD_DMA_CHANNEL4_IRQ))//dma 0
	{
		reg_dma_tc_isr |= FLD_DMA_CHANNEL4_IRQ;
		dma_irq_rx_cnt++;
	}
	if(( uart_dma_irq_src &FLD_DMA_CHANNEL5_IRQ))////dma 1
	{
		reg_dma_tc_isr |= FLD_DMA_CHANNEL5_IRQ;
		bt_ll_hci_tx_buffer_ptr = 0;
		dma_irq_tx_cnt++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		IRQ Table
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// interrupt handle
extern void default_irq_entry(void);
extern void trap_entry(void);

void  entry_null(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_null(void)
{
	trap_entry ();
}

void stimer_irq_handler();
void  entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq1(void)
{
	plic_isr (stimer_irq_handler, IRQ1_SYSTIMER);
}


extern void timer1_irq_handler(void);
void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	plic_isr (timer1_irq_handler, IRQ3_TIMER1);
}

extern void timer0_irq_handler(void);
void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq4(void)
{
	plic_isr (timer0_irq_handler, IRQ4_TIMER0);
}


extern void dma_irq_handler(void);
void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq5(void)
{
	plic_isr (dma_irq_handler, IRQ5_DMA);
}


extern void  usb_endpoint_irq_handler (void);
void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq11(void)
{
	plic_isr (usb_endpoint_irq_handler, IRQ11_USB_ENDPOINT);
}


void zb_dm_irq_handler(void);
void  entry_irq12(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq12(void)
{
	plic_isr (zb_dm_irq_handler, IRQ12_ZB_DM);
}

void zb_bt_irq_handler(void);
void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq14(void)
{
	plic_isr (zb_bt_irq_handler, IRQ14_ZB_BT);

}

void rf_irq_handler(void);
void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq15(void)
{
	plic_isr (rf_irq_handler, IRQ15_ZB_RT);

}

extern void uart1_irq_handler(void);
void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq18(void)
{
	plic_isr (uart1_irq_handler, IRQ18_UART1);

}

extern void uart0_irq_handler(void);
void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq19(void)
{
	plic_isr (uart0_irq_handler, IRQ19_UART0);
}
/**
 * @brief pspi interrupt handler.
 * @return none
 */
extern void pspi_irq_handler(void);
void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq23(void)
{
	plic_isr (pspi_irq_handler, IRQ23_SPI_APB);


}

extern void gpio_irq_handler(void);
void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq25(void)
{
	plic_isr (gpio_irq_handler, IRQ25_GPIO);

}

extern void gpio_risc0_irq_handler(void);
void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq26(void)
{
	plic_isr (gpio_risc0_irq_handler, IRQ26_GPIO2RISC0);

}

extern void gpio_risc1_irq_handler(void);
void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
_attribute_retention_code_ void  entry_irq27(void)
{
	plic_isr (gpio_risc1_irq_handler, IRQ27_GPIO2RISC1);

}

