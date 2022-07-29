/********************************************************************************************************
 * @file     main.c
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
#include "../../drivers.h"
#include "../../common/usb_dbg/myudb.h"

extern void intcntl_init(void);
extern void debug_init(void);
extern void bt_init(void);
extern void ble_init(void);
extern void schedule_init(void);
extern void user_init();
extern void main_loop (void);

static int test_bug1 = 2;
int stringlen = 3;

void myfunc(int stringlen) {
	stringlen++;
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
	blc_pm_select_internal_32k_crystal();

	sys_init(DCDC_1P4_LDO_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6);

	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV4_TO_CCLK,	CCLK_DIV2_TO_HCLK, HCLK_DIV1_TO_PCLK, CCLK_TO_MSPI_CLK);

	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();  //this is must

	intcntl_init();

	io_debug_init();

#if (ENABLE_PRINT||ENABLE_VCD)
	debug_init();
#endif

#if(ENABLE_BT_CLASSIC||ENABLE_LE)
    hci_transport_init(HCI_TRANSPORT_MODE);
#endif


#if(ENABLE_BT_CLASSIC||ENABLE_LE)
	btble_schedule_init();
#endif

#if ENABLE_LE
	ble_init();
#endif

#if ENABLE_BT_CLASSIC
	bt_init();
#endif

	user_init();
	
	core_enable_interrupt();

	int b;
	int a = 0;
	if (a) {

	}

	myfunc(a);

	char test_buf_overflow[16];
	memset(test_buf_overflow, 0, 20);

	while (1) {
		main_loop();
	}
	return 0;
}


