/********************************************************************************************************
 * @file     app_bt.c
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

MYFIFO_INIT(hci_rxfifo, HCI_RX_FIFO_SIZE, HCI_RX_FIFO_NUM);
MYFIFO_INIT(hci_txfifo, HCI_TX_FIFO_SIZE, HCI_TX_FIFO_NUM);

#if (ENABLE_PRINT||ENABLE_VCD)
MYFIFO_INIT(print_fifo, PRINT_TX_FIFO_SIZE, PRINT_TX_FIFO_NUM);
#endif


btc_afh_t env_afh[MAX_NB_ACTIVE_ACL];
btc_rsw_t env_rsw[MAX_NB_ACTIVE_ACL];
btc_enc_t env_enc[MAX_NB_ACTIVE_ACL];
btc_ssp_t env_ssp[MAX_NB_ACTIVE_ACL];
btc_sniff_t env_sniff[MAX_NB_ACTIVE_ACL];
btc_sco_t env_sco[MAX_NB_SYNC];
btc_test_mode_t env_test_mode[MAX_NB_ACTIVE_ACL-1];


btc_stack_init_t stack_init_default={
	.inq_en             = 0,
	.inq_mode 			= EXTENDED_INQUIRY,
	.inqscan_type 		= STANDARD_SCAN,
	.inqscan_use_fec 	= USE_FEC_RATE,

	.inqscan_inv        = INQSCAN_INR_107MS5,
	.inqscan_win		= INQSCAN_WIN_20MS,

	.page_to			= 0x2000,
	.ext_page_to		= 0,
	.pagescan_type      = STANDARD_SCAN,
	.pagescan_rep_mode  = R1,

	.pagescan_inv		= INQSCAN_INR_107MS5,
	.pagescan_win		= PAGESCAN_WIN_40MS,

	.sniff_inv_max		= 0x320,
	.sniff_inv_min		= 0x0c,

	.inq_lap[0]         = GIAC_LAP_0,
	.inq_lap[1]         = GIAC_LAP_1,
	.inq_lap[2]         = GIAC_LAP_2,
	.max_link_num		= MAX_NB_ACTIVE_ACL,

	.classofdevice      = LOCAL_CLASS_OF_DEVICE,
	.local_bdaddr       = LOCAL_BD_ADDR,
	.local_name       	= LOCAL_BT_NAME,
	.scan_en            = 0,
	.page_en            = 0,
};

void bt_init(void){
	// disable BT core
	bt_rwbtcntl_rwbten_setf(0);
	// reset BT core
	btc_bb_reset_core();
	// initiate BT clock
	btc_bb_clk_init();
	// initiate BT radio/modem module
	btc_bb_radio_init();
	// initiate BT exchange memory
	btc_bb_em_init();
	// initiate BT stack
	btc_stack_init(&stack_init_default);
	// initiate VCD
#if (ENABLE_VCD && ENABLE_BB_SIGNAL)
	bt_dbg_init();
#endif
	// enable BT CORE
	bt_rwbtcntl_rwbten_setf(1);
	// set BT interrupt prioriy
    plic_set_priority(IRQ14_ZB_BT, 2);
    // enable BT interrupt
    plic_interrupt_enable(IRQ14_ZB_BT); //enable BT core irq

    btc_reset_register_cb(&bt_init);

}

void btc_module_init(void)
{
    // AFH
	btc_afh_register_module(&env_afh[0]);
	// AFH CLS
	btc_afh_cls_register_module();
	// ROLE SWITCH
    btc_rsw_register_module(&env_rsw[0]);

    // LEGACY PAIR
    btc_legacy_pair_register_module();
    // SECURE SIMPLE PAIR
    btc_ssp_register_module(&env_ssp[0]);
    // LEGACY AUTHEN
    btc_legacy_authen_register_module();
    // SECURE AUTHEN

    // ENCRYPTION
    btc_enc_register_module(&env_enc[0]);
    // ESCO
//    btc_sco_register_module(&env_sco[0]);
    // SNIFF
//    btc_sniff_register_module(&env_sniff[0]);
    // LOW POWER

    //TEST MODE
//    btc_test_mode_register_module(&env_test_mode[0]);
}


#if(ENABLE_PRINT)
my_dump_str_data_callback_t my_dump_str_data = 0;

my_dump_str_u32s_callback_t my_dump_str_u32s = 0;
#endif

void debug_init(void)
{
#if (((!PRINT_MODE) && ENABLE_PRINT)||((!VCD_MODE) && ENABLE_VCD))
	#if(ENABLE_HCI_TRANSPORT)
		#if(HCI_TRANSPORT_MODE!=HCI_IF_USB)
			myudb_usb_init(DEVICE_BID, NULL, &print_fifo);
			usb_set_pin_en();
			my_dump_str_data = usb_send_str_data_cb;
			my_dump_str_u32s = usb_send_str_u32s_cb;

		#endif
	#else
		myudb_usb_init(DEVICE_BID, NULL, &print_fifo);
		usb_set_pin_en();
		my_dump_str_data = usb_send_str_data_cb;
		my_dump_str_u32s = usb_send_str_u32s_cb;
	#endif
#elif(PRINT_MODE==1)
		uart_dbg_init();
		my_dump_str_data = printf_str_data;
		my_dump_str_u32s = printf_str_u32s;
#elif(PRINT_MODE==2)

#endif

#if ENABLE_VCD
	#if(ENABLE_BB_SIGNAL)//USB VCD
		usb_vcd_init();
	#endif

	#if(VCD_MODE==1)//UART VCD
		uart_vcd_init();
	#endif
#endif
}


void btble_stack_mainloop(void)
{

#if(((VCD_MODE) && ENABLE_VCD))
	static uint32_t t_sync = clock_time();
	if (sys_timeout(t_sync, 200000))			//sync word, 200ms
	{
		t_sync = clock_time();
		log_sync (SL_STACK_VCD_EN);
	}
#endif

#if (((!VCD_MODE) && ENABLE_VCD)||((!PRINT_MODE) && ENABLE_PRINT)||(HCI_TRANSPORT_MODE!=2))
	#if(ENABLE_VCD&&(PRINT_MODE == 0))
		myudb_usb_handle_irq();

	#else
		hci_usb_handle_irq();
	#endif
#endif
#if((PRINT_MODE == 1)&& ENABLE_PRINT)
	printf_process();
#endif


#if(ENABLE_HCI_TRANSPORT&&(HCI_TRANSPORT_MODE==2))
	bt_ll_hci_tx_process();
    hci_uart_rx_process();
#endif

#if(!ENABLE_HCI_TRANSPORT)
    hci_controller_to_host();
    host_task();
#endif

#if(ENABLE_BT_CLASSIC||ENABLE_LE)
    hci_host_to_controller();
#endif

#if(ENABLE_LE)
    app_ble_main_loop();
#endif

#if(ENABLE_BT_CLASSIC)
    bt_ll_main_loop();
#endif

}
