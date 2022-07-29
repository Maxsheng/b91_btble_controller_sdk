/********************************************************************************************************
 * @file     printf.c
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

#include "printf.h"
#include "drivers.h"
#if(DEBUG_MODE==1)

#if (DEBUG_BUS==USB_PRINT_DEBUG_ENABLE)

__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{
	(void) fd;

    int    i;
    for (i = 0; i < size; i++)
    {
    	#if(BLOCK_MODE)
    		while (USBFIFO& 0x02);
    	#endif
    		EDPS_DAT= buf[i];
     }

    return i;
}

#elif (DEBUG_BUS==UART_PRINT_DEBUG_ENABLE)

/**
 * @brief      This function serves to foramt string by GPIO simulate uart.
 * @param[in]  byte  -  a byte need to print
 * @return     none.
 */
#define HIGH_UART_SPEED 0
void dr_putchar(unsigned char byte){

	static unsigned char init_flag = 1;
	if(init_flag==1)
	{
		gpio_function_en(DEBUG_INFO_TX_PIN);
		gpio_set_up_down_res(DEBUG_INFO_TX_PIN, GPIO_PIN_PULLUP_1M);
		gpio_output_en(DEBUG_INFO_TX_PIN);
		gpio_set_high_level(DEBUG_INFO_TX_PIN);
		init_flag = 0;
	}

	unsigned char tmp_bit0 = TX_PIN_OUTPUT_REG & (~(DEBUG_INFO_TX_PIN & 0xff));
	unsigned char tmp_bit1 = TX_PIN_OUTPUT_REG | (DEBUG_INFO_TX_PIN & 0xff);
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
	
	GLOBAL_INT_DISABLE();

#if HIGH_UART_SPEED
	unsigned char *tbit;
	tbit = bit;
	unsigned char j = 0;
	for(j = 0;j<10;j++)
	{
		TX_PIN_OUTPUT_REG=*(tbit++);
	}
#else
	unsigned char j = 0;
	unsigned int t1 = 0;
	unsigned int t2 = 0;
	t1 = stimer_get_tick();// stimer register
	for(j = 0;j<10;j++)
	{
		t2=t1;
		while(t1 - t2 < BIT_INTERVAL){
			t1  = stimer_get_tick();
		}
		TX_PIN_OUTPUT_REG=bit[j];        //send bit0
	}
#endif
	GLOBAL_INT_RESTORE();
}

__attribute__((used)) int _write(int fd, const unsigned char *buf, int size)
{
    (void) fd;
    int    i;
    for (i = 0; i < size; i++){
    	dr_putchar(buf[i]);
    }
    return i;
}
#endif

#endif




