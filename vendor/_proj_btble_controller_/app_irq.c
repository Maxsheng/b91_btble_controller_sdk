/********************************************************************************************************
 * @file     app_irq.c
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


extern void trap_entry(void);
extern void entry_null(void);
extern void entry_irq1(void);			//stimer
extern void entry_irq5(void);			//dma
extern void entry_irq12(void);			//DM core
extern void entry_irq14(void);			//BT core
extern void entry_irq15(void);			//BLE
extern void entry_irq18(void);			//uart1
extern void entry_irq19(void);			//uart0
extern void entry_irq25(void);			//gpio: TX_EN
extern void entry_irq26(void);			//gpio0: RX_EN
extern void entry_irq27(void);			//gpio1: RX_SYNC

extern void UART_IRQHandler(void);

/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void stimer_irq_handler(void)
{
    systimer_clr_irq_status();

    bt_ll_system_tick_isr();

    ble_ll_system_tick_isr();

}
/**
 * @brief		Baseband interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_retention_code_ void rf_irq_handler(void)
{

#if ENABLE_LE
	ble_sdk_rf_irq_handler();
#endif

}


_attribute_ram_code_ void zb_bt_irq_handler(void)
{
	gpio_set_high_level(GPIO_PC6);
    bt_core_isr();
    gpio_set_low_level(GPIO_PC6);
}

_attribute_ram_code_ void zb_dm_irq_handler(void)
{
	ip_core_isr();
}

typedef void (* intcntl_table_t)(void);

__attribute__((section(".retention_data"))) intcntl_table_t   intcntl_table[64] __attribute__ ((aligned (256))) = {
		trap_entry,
		entry_irq1,
		entry_null,
		entry_null,
		entry_null,
		entry_irq5,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_irq12,
		entry_null,
		entry_irq14,
		entry_irq15,
		entry_null,
		entry_null,
		entry_irq18,
		entry_irq19,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_irq25,
		entry_irq26,
		entry_irq27,
		entry_null,
		entry_null,
		entry_null,
		entry_null,

		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
		entry_null,
};

void intcntl_init(void)
{
	write_csr(NDS_MTVEC, (u32)intcntl_table);
}
