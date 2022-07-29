/********************************************************************************************************
 * @file     bt_dbg.c
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
#include "bt_dbg.h"



#if (BT_BASEBAND_SIGNAL_DBG_EN)

void bt_dbg_init(void)
{
    // radcntl_txen <-> GPIO_PA0
    // bt_dbg_set_pin(BT_DBG1_BB8_B3);
    bt_dbg_set_pin(DEBUG_PORT_TX_EN_PIN);		//PB3
    bt_dbg_set_pin(DEBUG_PORT_RX_EN_PIN);		//PB4
    bt_dbg_set_pin(BT_DBG1_BB10_B5);			//PB5
    bt_dbg_set_pin(DEBUG_PORT_RX_SYNC_PIN);		//PB6
    bt_dbg_set_signal(BT_DBG_SEL_DBG1, 0x03);

    bt_dbg_set_pin(BT_DBG2_BB16_C3);// CLKN<0>  hslot
    bt_dbg_set_pin(BT_DBG2_BB17_C4);// CLKN<1>  slot
    bt_dbg_set_signal(BT_DBG_SEL_DBG2, 0x05);

    bt_dbg_set_pin(GPIO_PA1);// end irq
    bt_dbg_set_pin(GPIO_PB0);
    bt_dbg_set_pin(GPIO_PB1);// error irq
    bt_dbg_set_pin(GPIO_PB2);// skip irq
    bt_dbg_set_signal(BT_DBG_SEL_DBG0, 0x20);

}
#else

void bt_dbg_init(void)
{

    // radcntl_txen <-> GPIO_PA0
    // bt_dbg_set_pin(BT_DBG1_BB8_B3);
    bt_dbg_set_pin(DEBUG_PORT_TX_EN_PIN);		//PB3
    bt_dbg_set_pin(DEBUG_PORT_RX_EN_PIN);		//PB4
    bt_dbg_set_pin(BT_DBG1_BB10_B5);			//PB5
    bt_dbg_set_pin(DEBUG_PORT_RX_SYNC_PIN);		//PB6
    bt_dbg_set_signal(BT_DBG_SEL_DBG1, 0x03);
#if DEBUG_TIMING
    bt_dbg_set_signal(BT_DBG_SEL_DBG2, 0x22);
#endif
}

#endif





#if (BT_BASEBAND_SIGNAL_LOOPBACK_VCD)


_attribute_retention_code_ void gpio_irq_handler(void)
{
	if(gpio_get_level(DEBUG_PORT_TX_EN_PIN))
	{
		log_task(SL_STACK_FRAME_EN,SL01_TX_EN,1);
		BM_SET(reg_gpio_pol(DEBUG_PORT_TX_EN_PIN), DEBUG_PORT_TX_EN_PIN & 0xff);
	}
	else
	{
		log_task(SL_STACK_FRAME_EN,SL01_TX_EN,0);
		BM_CLR(reg_gpio_pol(DEBUG_PORT_TX_EN_PIN), DEBUG_PORT_TX_EN_PIN & 0xff);
	}


	reg_gpio_irq_clr |=FLD_GPIO_IRQ_CLR ;

}


_attribute_retention_code_ void gpio_risc0_irq_handler(void)
{
	if(gpio_get_level(DEBUG_PORT_RX_EN_PIN))
	{
		log_task(SL_STACK_FRAME_EN,SL01_RX_EN,1);
		BM_SET(reg_gpio_pol(DEBUG_PORT_RX_EN_PIN), DEBUG_PORT_RX_EN_PIN & 0xff);
	}
	else
	{
		log_task(SL_STACK_FRAME_EN,SL01_RX_EN,0);
		BM_CLR(reg_gpio_pol(DEBUG_PORT_RX_EN_PIN), DEBUG_PORT_RX_EN_PIN & 0xff);
	}
	reg_gpio_irq_clr |=FLD_GPIO_IRQ_GPIO2RISC0_CLR ;

}


_attribute_retention_code_ void gpio_risc1_irq_handler(void)
{
	reg_gpio_irq_clr |=FLD_GPIO_IRQ_GPIO2RISC1_CLR ;
	log_tick(SL_STACK_FRAME_EN,SLET_RX_SYNC);
	log_event(SL_STACK_FRAME_EN,SLEV_RX_SYNC);
}



void usb_vcd_init(void)
{
    //  initiate gpio interrupt for vcd debug port
    gpio_set_output_en(DEBUG_PORT_TX_EN_PIN, 1);
    gpio_set_input_en(DEBUG_PORT_TX_EN_PIN, 1);
    gpio_irq_en(DEBUG_PORT_TX_EN_PIN);

    gpio_set_irq(DEBUG_PORT_TX_EN_PIN, INTR_RISING_EDGE);
    plic_set_priority(IRQ25_GPIO, 1);
    plic_interrupt_enable(IRQ25_GPIO);

    gpio_set_output_en(DEBUG_PORT_RX_EN_PIN, 1);
    gpio_set_input_en(DEBUG_PORT_RX_EN_PIN, 1);
    gpio_gpio2risc0_irq_en(DEBUG_PORT_RX_EN_PIN);
    gpio_set_gpio2risc0_irq(DEBUG_PORT_RX_EN_PIN, INTR_RISING_EDGE);
    plic_set_priority(IRQ26_GPIO2RISC0, 1);
    plic_interrupt_enable(IRQ26_GPIO2RISC0);

    gpio_set_output_en(DEBUG_PORT_RX_SYNC_PIN, 1); // SLET_RX_SYNC
    gpio_set_input_en(DEBUG_PORT_RX_SYNC_PIN, 1);
    gpio_gpio2risc1_irq_en(DEBUG_PORT_RX_SYNC_PIN);
    gpio_set_gpio2risc1_irq(DEBUG_PORT_RX_SYNC_PIN, INTR_RISING_EDGE);

    plic_set_priority (IRQ27_GPIO2RISC1, 2);
    plic_interrupt_enable(IRQ27_GPIO2RISC1);

}

#else

void usb_vcd_init(void)
{


}

#endif
