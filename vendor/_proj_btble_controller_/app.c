/********************************************************************************************************
 * @file     app.c
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

#include <vendor/_proj_btble_controller_/app_config.h>
#include "stack.h"

extern void app_btc_set_local_name(char* name);
extern void app_btc_set_bd_addr(uint8_t *bdaddr);
extern void btble_stack_mainloop(void);

#define GPIO_C GPIO_PC4|GPIO_PC5|GPIO_PC6|GPIO_PC7
#define GPIO_D GPIO_PD2|GPIO_PD3|GPIO_PD7

static int test_unusedstaticvar = 2;

void io_debug_init(void)
{
    gpio_set_gpio_en(GPIO_C);
    gpio_output_en(GPIO_C);         //enable output
    gpio_input_dis(GPIO_C);        //disable input

    gpio_set_gpio_en(GPIO_D);
    gpio_output_en(GPIO_D);         //enable output
    gpio_input_dis(GPIO_D);        //disable input


}
void my_vs_test_func(u8 *para)
{
	gpio_toggle(GPIO_D);
}


void user_init(void)
{
	hci_vendor_spec_register_cb_port(my_vs_test_func,0x08);
}
/**
 * @brief     BTBLE SDK main loop
 * @param[in]  none.
 * @return     none.
 */
void main_loop(void)
{
	btble_stack_mainloop();
}

