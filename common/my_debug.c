/********************************************************************************************************
 * @file     my_debug.c
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

#if 0
//#include "../vendor/_proj_tws_/app_config.h"
#include "../vendor/common/user_config.h"
#include "usb_dbg/myudb.h"
#include "my_debug.h"
#include "../stack/bt/host/avrcp/avrcp_api.h"
#include "../drivers.h"
#include "stack.h"
#include "incs.h"
#include "co_bt_defines.h"
#include "../stack/bt/host/host.h"
#include "../stack/bt/host/hfp/hfp_ag.h"
#else

#include "../vendor/common/user_config.h"
#include "common/common.h"
#include "drivers.h"
#include "stack.h"
//#include "application.h"
#include "my_debug.h"

#endif


extern void my_msbc_pcm_volume_set(int16_t v);
extern void bt_ll_task_dump(void);

volatile unsigned char uart0_tx_buff_byte[16 ] __attribute__((aligned(4))) ={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

void my_uart0_init(void)
{
	unsigned short div;
	unsigned char bwpc;

	uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2, UART0_RX_PB3 );// uart tx/rx pin set
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_dma_rx_timeout(UART0, bwpc, 12, UART_BW_MUL1);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
}

/**
 * @brief     uart send data function by byte with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x14080 0x14081 0x14082 0x14083 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uart_num - UART0 or UART1.
 * @param[in] tx_data - the data to be send.
 * @return    none
 */
void my_uart0_send_byte(uart_num_e uart_num, unsigned char tx_data)
{
	static unsigned char uart_tx_byte_index[2] = {0};

	while(uart_get_txfifo_num(uart_num)>7);

	reg_uart_data_buf(uart_num, uart_tx_byte_index[uart_num]) = tx_data;
	uart_tx_byte_index[uart_num] ++;
	(uart_tx_byte_index[uart_num]) &= 0x03;
}

void my_debug_init(void *p)
{
	myudb_register_hci_debug_cb(p);

	//my_dump_str_data(MY_DEBUG_DUMP_EN, "my_debug_init()\r\n", my_debug, 4);
}

extern void acl_link_tws_bt_token_debug(uint8_t TXPWRUPCT);
extern void bt_dbg_set_signal(bt_signal_group_e signal_group,u8 signal_byte);
extern void bt_ll_sco_retxnb_set (int retxnb);
extern void	audio_codec_mode_dump(void);
extern void	tmusic_dump (void);
extern void	tcodec_dump (void);

uint16_t conhdl_sniff = 0;


int my_debug(unsigned char *p, int len)
{
	my_dump_str_data(MY_DEBUG_DUMP_EN, "my_debug receive\r\n", p, len);
    //uint16_t u16_data;

    //bd_addr_t addr = {{0x29, 0xdc, 0x1a, 0x89, 0x97, 0x9c}}; // MI Air 2 , ruifeng
    //bd_addr_t addr = {{0xee, 0x29, 0x9c, 0x89, 0x97, 0x9c}}; // MI Air 2s , ruifeng
    //bd_addr_t addr = {{0xe2, 0x60, 0xc8, 0xeb, 0xa7, 0x78}}; // MI Air2
    //bd_addr_t addr = {{0xa2, 0xea, 0xc6, 0x89, 0x97, 0x9c}}; // MI Air 2s
    //bd_addr_t addr = {{0x7e, 0x71, 0xfe, 0x89, 0x97, 0x9c}}; // MI Air 2s sy
    //bd_addr_t addr = {{0x4a, 0x36, 0xa9, 0x6e, 0x59, 0x39}};		//364a 39596ea9
    bd_addr_t addr = {{0x39, 0x59, 0x6e, 0xa9, 0x36, 0x4a}};		//364a 39596ea9
          

    if (p[0] != 0x11)
    {
        return 1;
    }

    switch (p[1])
    {
    case 0:
		//spp_tx_data_test_stress();//temp add for test
        break;
        
    case 1:
        while (len > 0)
        {
          //  dr_putchar(p[len - 1]);
            len--;
        }

        printf("@ my uart\n");
        tmemcpy ((void *)uart0_tx_buff_byte, (void *)p, len=(len > 16)?16:len);

        for (int i = 0; i < 16; i++)
            my_uart0_send_byte(UART0, uart0_tx_buff_byte[i]);
        break;
     case 02:
	 	avrcp_tg_volume_add();
        break;   
     case 03:
	 	avrcp_tg_volume_down();
        break;   
     case 04:
	 	avrcp_send_cmd(0x4b);
        break;   
     case 05:
	 	avrcp_send_cmd(0x4c);
        break; 
#if SSP_PAIRING_TEST
	case 06:
   		app_spp_sdp_test(p[2]);
   	break; 

#endif

 
    case 0x08:
        btlib_hcicmd_write_scan_enable(p[2]);
        break;	
        
    case 0x14:        
        my_dump_str_data(MY_DEBUG_DUMP_EN, "BT_DBG_SEL_DBG0", &p[2], 1);
        bt_dbg_set_signal(BT_DBG_SEL_DBG0, p[2]);
        break;
    
    case 0x15:
        my_dump_str_data(MY_DEBUG_DUMP_EN, "bt_ll_sco_retxnb_set", &p[2], 1);
        bt_ll_sco_retxnb_set(p[2]);
        break;

    case 0x17:
        btlib_hcicmd_inquiry(INQUIRY_LENGTH_12S8, 10);
        break;
    case 0x18:
       // btm_inquiry_result_dump();
        break;
        
    case 0x19:
        break;
    case 0x1a:
        break;
        
    case 0x1b:
        break;
    case 0x1c:
        break;

    case 0x1d:
        /*
        11 1d 00 04
        */
        my_msbc_pcm_volume_set((p[3]<<8) | p[2]);
        break;

    case 0x1e:
    {
    	extern void my_bt_connect_lauch(bd_addr_t *remote);
    	my_bt_connect_lauch(&addr);
    }
        break;
          
    case 0x25:
        audio_codec_mode_dump();
        break;
	
	case 0x27:
		bt_ll_task_dump();
		break;
            
    case 0x31:
        conhdl_sniff = btm_conn_opened_conhdl_get();
        host_sniff_enter(conhdl_sniff);
        break;
    
    case 0x32:
        host_sniff_exit(conhdl_sniff);
        break;
        
    default:
        break;
    }
    
	return 0;
}

#if 0

#define PRINT_BAUD_RATE             		115200//1M baud rate,should Not bigger than 1Mb/s
//#define PRINT_BAUD_RATE             		921600//1M baud rate,should Not bigger than 1Mb/s
//#define DEBUG_INFO_TX_PIN           		GPIO_PA2//
#define TX_PIN_GPIO_EN()					gpio_function_en(DEBUG_INFO_TX_PIN);
#define TX_PIN_PULLUP_1M()					gpio_set_up_down_res(DEBUG_INFO_TX_PIN, GPIO_PIN_PULLUP_1M);
#define TX_PIN_OUTPUT_EN()					gpio_set_output(DEBUG_INFO_TX_PIN, 1)
#define TX_PIN_OUTPUT_REG					(0x140303+((DEBUG_INFO_TX_PIN>>8)<<3))

#ifndef		BIT_INTERVAL
#define		BIT_INTERVAL	(16*1000*1000/PRINT_BAUD_RATE)
#endif



/**
 * @brief      This function serves to foramt string by GPIO simulate uart.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
_attribute_ram_code_  void dr_putchar(unsigned char byte){
	unsigned char j = 0;
	unsigned int t1 = 0,t2 = 0;
	static unsigned char init_flag = 1;
	if(init_flag==1)
	{
		TX_PIN_GPIO_EN();
		TX_PIN_PULLUP_1M();
		TX_PIN_OUTPUT_EN();//Enable output

		init_flag = 0;
	}

	unsigned char tmp_bit0 = read_reg8(TX_PIN_OUTPUT_REG) & (~(DEBUG_INFO_TX_PIN & 0xff));
	unsigned char tmp_bit1 = read_reg8(TX_PIN_OUTPUT_REG) | (DEBUG_INFO_TX_PIN & 0xff);
	unsigned char bit[10] = {0};

	bit[0] = tmp_bit0;
	bit[1] = (byte & 0x01)? tmp_bit1 : tmp_bit0;
	bit[2] = ((byte>>1) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[3] = ((byte>>2) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[4] = ((byte>>3) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[5] = ((byte>>4) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[6] = ((byte>>5) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[7] = ((byte>>6) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[8] = ((byte>>7) & 0x01)? tmp_bit1 : tmp_bit0;
	bit[9] = tmp_bit1;

	t1 = read_reg32(0x140200);//eagle stimer register

    GLOBAL_INT_DISABLE();
	for(j = 0;j<10;j++)
	{
		t2 = t1;
		while(t1 - t2 < BIT_INTERVAL){
			t1  = read_reg32(0x140200);
		}
		write_reg8(TX_PIN_OUTPUT_REG,bit[j]);        //send bit0
	}
    GLOBAL_INT_RESTORE();
}
#endif

