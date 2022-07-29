/********************************************************************************************************
 * @file     bt_clock.c
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

#include "bt_clock.h"


_attribute_retention_code_
void clock_change (sys_pll_div_to_cclk_e cclk_div)
{
	u32 r = core_interrupt_disable ();

	analog_write_reg8 (0x05, analog_read_reg8(0x05) & ~BIT(2));		//turn on 24M¡¡RC

	/////////192M / div   3  2  4  6  8  10 12 14
	int zclk = cclk_div == PLL_DIV2_TO_CCLK ? 5 : cclk_div == PLL_DIV3_TO_CCLK ? 3: cclk_div == PLL_DIV4_TO_CCLK ? 2 : 1;

	REG_ADDR8(0x1401d2) = (REG_ADDR8(0x1401d2) & 0xf0) | zclk;	//baseband clock

	write_reg8(0x1401e8, (read_reg8(0x1401e8) & 0xf0) | cclk_div);

	tick_per_us = sys_clk.cclk = sys_clk.pll_clk / cclk_div;

	if (sys_clk.cclk == 16)
	{
		write_reg8(0x1401d8, read_reg8(0x1401d8) & ~BIT(2));		//hclk = cclk
	}
	else if (sys_clk.cclk != sys_clk.hclk)
	{
		write_reg8(0x1401d8, read_reg8(0x1401d8) | BIT(2));
	}
	core_restore_interrupt (r);
}

