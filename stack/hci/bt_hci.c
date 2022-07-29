/********************************************************************************************************
 * @file     bt_hci.c
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

#include "../drivers.h"
#include "../stack/stack.h"


unsigned char uart_dma_send_flag = 0;

hci_txfifo_read_t	hci_txfifo_r[3] = {{0,0,0,0, 2,0,0,0}, {0,0,0,0, 2,0,0,0}, {0,0,0,0, 2,0,0,0}};

hci2uart_cfg_t	hci2uart = {0, 0};

_attribute_data_retention_ int	hci_txfifo_idx_ref = HCI_IF_HOST;

#ifndef			DUMP_HCI_DATA_ALL_EN
#define			DUMP_HCI_DATA_ALL_EN		0
#endif

////////////////////////////////////////////////////////////////////////////////////
void hci_set_acl_packet_length_limit (int idx, u8 type, int n)
{
	if (idx < (sizeof(hci_txfifo_r) / sizeof(hci_txfifo_r[0]))) {
		hci_txfifo_read_t *ph = &hci_txfifo_r[idx];
		ph->pkt_type = type;
		ph->pkt_len_limit = n;
	}
}

void hci_txfifo_set_rptr_reference (int idx)
{
	hci_txfifo_idx_ref = idx < 3 ? idx : 0;
}

_attribute_retention_code_ int  hci_txfifo_full (void)
{
	if (hci_txfifo_idx_ref < (sizeof(hci_txfifo_r) / sizeof(hci_txfifo_r[0]))) {
		hci_txfifo_read_t *ph = &hci_txfifo_r[hci_txfifo_idx_ref];
		u8 n = hci_txfifo.wptr - ph->rptr;
		return n >= hci_txfifo.num;
	}
	return 0;
}
int  hci_rxfifo_half_full (void)
{
	u8 n = hci_rxfifo.wptr - hci_rxfifo.rptr;
	return n >= (hci_rxfifo.num/2);
}

int  hci_rxfifo_full (void)
{
	u8 n = hci_rxfifo.wptr - hci_rxfifo.rptr;
	return n >= hci_rxfifo.num;
}
#if(ENABLE_PRINT||ENABLE_VCD)
int  hci_print_fifo_full (void)
{
	u8 n = print_fifo.wptr - print_fifo.rptr;
	return n >= print_fifo.num;
}
int  hci_print_fifo_empty (void)
{
	return print_fifo.wptr == print_fifo.rptr;

}
#endif
int hci_rxfifo_is_empty(void)
{
	return hci_rxfifo.wptr == hci_rxfifo.rptr;
}

int hci_txfifo_is_empty(void)
{
	return hci_txfifo.wptr == hci_txfifo.rptr;
}


_attribute_ram_code_ void hci_txfifo_next (int idx)
{
	if (idx < (sizeof(hci_txfifo_r) / sizeof(hci_txfifo_r[0]))) {
		hci_txfifo_read_t *ph = &hci_txfifo_r[idx];
		if (ph->sel)
		{
			ph->rptr_print++;

			if (idx == hci_txfifo_idx_ref)
			{
#if(ENABLE_PRINT||ENABLE_VCD)
				print_fifo.rptr++;
#endif
			}
		}
		else
		{
			ph->rptr++;
			if (idx == hci_txfifo_idx_ref)
			{
				hci_txfifo.rptr++;
			}
		}

		ph->p = 0;
		ph->len = 0;
		ph->left = 0;
	}
}

_attribute_ram_code_ u8*	hci_txfifo_get (int idx, int nsize)
{
	u8 *pd = 0;
	if (idx < (sizeof(hci_txfifo_r) / sizeof(hci_txfifo_r[0]))) {
		hci_txfifo_read_t *ph = &hci_txfifo_r[idx];

		if (!ph->len)
		{
			ph->p = 0;
		}

		while (!ph->p && (hci_txfifo.wptr != ph->rptr))
		{
			u8 *pt =  hci_txfifo.p + (ph->rptr & (hci_txfifo.num - 1)) * hci_txfifo.size;

			int len = pt[0] + pt[1] * 256;

			if (ph->pkt_len_limit && (pt[4] == ph->pkt_type) && len > ph->pkt_len_limit )
			{
				ph->rptr++;
			}
			else
			{
				ph->p = pt + 4;
				ph->left = pt[0] + pt[1] * 256;
				ph->sel = 0;
			}

		}
#if(ENABLE_PRINT||ENABLE_VCD)
		if (!ph->p && (print_fifo.wptr != ph->rptr_print))		//first packet
		{
			u8* pt = print_fifo.p + (ph->rptr_print & (print_fifo.num - 1)) * print_fifo.size;
			ph->p = pt + 4;
			ph->left = pt[0] + pt[1] * 256;
			ph->sel = 1;
		}
#endif
		if (ph->p)
		{
			pd = ph->p;

			ph->len = ph->left > nsize ? nsize : ph->left;

			ph->p += ph->len;

			ph->left -= ph->len;

		}
	}
		return pd;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////

_attribute_data_retention_ struct evt_mask hci_rsvd_evt_msk = {{0x00, 0x60, 0x04, 0x00, 0xF8, 0x07, 0x40, 0x02}};

_attribute_data_retention_ struct evt_mask hci_env_tag = {{0xff,0xff,0xfb,0xff,0xff,0xff,0xff,0xff}}; // default set num of packets complete as 0
_attribute_data_retention_ struct evt_mask hci_env_tag_page_2 = {{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};



void hci_event_mask_bit_set(u8 evt_code,bool bit_v)
{
	if(evt_code < HCI_MAX_EVT_MSK_PAGE_1_CODE)
	{
		u8 index= evt_code- 1;

		u8 Mask = 1<<(index & 0x07);       /* Compute features bit mask                    */


		if(bit_v) {
			BM_SET(hci_env_tag.mask[index>>3],Mask);
			BM_SET(hci_rsvd_evt_msk.mask[index>>3],Mask);
		}

		else   {
			BM_CLR(hci_env_tag.mask[index>>3],Mask);
			BM_CLR(hci_rsvd_evt_msk.mask[index>>3],Mask);

		}


	}
    else if(evt_code < HCI_MAX_EVT_MSK_PAGE_2_CODE)
    {
        // In this area the evt code is in the range [EVT_MASK_CODE_MAX<evt_code<HCI_MAX_EVT_MSK_CODE]
        // The index should be < EVT_MASK_CODE_MAX to avoid evt_msk_page_2.mask array overflow
      //  uint8_t index = evt_code - EVT_MASK_CODE_MAX;


    }

}

_attribute_ram_code_sec_noinline_ u8 * bt_hci_get_event_buffer(void)
{
	if (hci_txfifo_full())
	{
		return 0;
	}

	u8 *p = hci_txfifo.p + (hci_txfifo.wptr & (hci_txfifo.num-1)) * hci_txfifo.size;
	return p + 7;
}

_attribute_ram_code_sec_noinline_ void bt_hci_release_event_buffer(u8 evt, int n)
{
	if (hci_txfifo_full() || n + 5 >= hci_txfifo.size)
	{
	    my_dump_str_data(1, "#err,full, inqury_event_hci_send", 0, 0);
		return ;
	}
	u8 *ps = hci_txfifo.p + (hci_txfifo.wptr & (hci_txfifo.num-1)) * hci_txfifo.size;
	ps[4] = HCI_TYPE_EVENT;
	ps[5] = evt;
	ps[6] = n;
	n += 3;
	ps[0] = n;
	ps[1] = n >> 8;
	hci_txfifo.wptr++;
}
/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check if the event to be sent to the host is masked or not
 *
 * @param[in] msg  Pointer to the message containing the event
 *
 * @return true id the message has to be filtered out, false otherwise
 *****************************************************************************************
 */
static bool hci_evt_mask_check(u32 h)
{
    bool masked = false;
    uint8_t evt_code = (h&0xff);

    // Check if this event is maskable
    if(evt_code < HCI_MAX_EVT_MSK_PAGE_1_CODE)
    {
        uint8_t index = evt_code - 1;
        //Checking if the event is masked or not

        masked = ((hci_env_tag.mask[index>>3] & (1<<(index & 0x07))) == 0x00);

    }
    else if(evt_code < HCI_MAX_EVT_MSK_PAGE_2_CODE)
    {
        // In this area the evt code is in the range [EVT_MASK_CODE_MAX<evt_code<HCI_MAX_EVT_MSK_CODE]
        // The index should be < EVT_MASK_CODE_MAX to avoid evt_msk_page_2.mask array overflow
        uint8_t index = evt_code - EVT_MASK_CODE_MAX;

        //Checking if the event is masked or not
        masked = ((hci_env_tag_page_2.mask[index>>3] & (1<<(index & 0x07))) == 0x00);
    }

    return masked;
}

_attribute_retention_code_ int bt_hci_send_data (u32 h, u8 *para, int n)
{
	if (hci_txfifo_full() || n + 5 >= hci_txfifo.size)
	{
	    my_dump_str_data(1, "#err,full, bt_hci_send_data()", para, n);
	    my_dump_str_u32s(1, "#err,full,", hci_txfifo.wptr,hci_txfifo.rptr, 0,0);
		return -1;
	}

	if (h & HCI_FLAG_EVENT_BT_STD)
	{
        if(hci_evt_mask_check(h))
        {
        	return -1;
        }
        else
        {

        }
	}
    GLOBAL_INT_DISABLE();

	u8 *p = hci_txfifo.p + (hci_txfifo.wptr & (hci_txfifo.num-1)) * hci_txfifo.size;

	int nl = n + 4;

	if (h & HCI_FLAG_EVENT_TLK_MODULE)
	{
		*p++ = nl;
		*p++ = nl >> 8;
		p += 2;
		*p++ = 0xff;
		*p++ = n + 2;
		*p++ = h;
		*p++ = h>>8;
		tmemcpy (p, para, n);
		p += n;
	}
	else if (h & HCI_FLAG_EVENT_BT_STD)
	{
		*p++ = (nl-1)&0xff;        // n+3
		*p++ = ((nl-1)>>8)&0xff;   // 0
		 p=p+2;  // the header is 4 byte length
		*p++ = HCI_TYPE_EVENT;
		*p++ = h;
		*p++ = n;
		tmemcpy (p, para, n);
		p += n;
	}
	else if (h & HCI_FLAG_EVENT_STACK)
	{
		*p++ = 0xff;
		*p++ = h;
		*p++ = h>>8;
		*p++ = n + 2;
		tmemcpy (p, para, n);
		p += n;
	}
	else if (h & HCI_FLAG_ACL_BT_STD)			//ACL data
	{
		n = para[1];
		*p++ = n + 5;
		*p++ = (n + 5) >> 8;
		p += 2;
		*p++ = 0x02;
		*p++ = h;								//handle
		*p++ = h>>8; 							//high 4-bit LLID
		*p++ = n;								//length
		*p++ = n >> 8;
		tmemcpy (p, para, n);
		p += n;
	}
	else if(h & HCI_FLAG_EVENT_PHYTEST_2_WIRE_UART)
	{
		*p++ = n;							//length
		*p++ = n >> 8;
		tmemcpy (p, para, n);
	}
	hci_txfifo.wptr++;
    GLOBAL_INT_RESTORE();
	return 0;
}

u8 * hci_rxfifo_get (void)
{
	if (hci_rxfifo_full())
	{
		return 0;

	}		
	u8 *p = hci_rxfifo.p + (hci_rxfifo.wptr & (hci_rxfifo.num-1)) * hci_rxfifo.size;
	return p;

}


int	bt_ll_hci_acl_audio_pkt = 0;


static u8  app_spp_flow = 1;
void hci_set_rx_acl_flow_control(u8 flow)
{
	app_spp_flow = flow;
	my_dump_str_data (1, "hci_set_controller_to_host_flow******", &app_spp_flow, 1);
}
u8  hci_get_rx_acl_flow_control(void)
{
	//my_dump_str_data (1, "hci_get_rx_acl_flow_control******", &app_spp_flow, 1);
	return app_spp_flow;
}


#if (ENABLE_HCI_TRANSPORT&&(HCI_TRANSPORT_MODE==2))

volatile unsigned int uart_tx_start_tick ;
#define UART_TX_BUSY_TIMEOUT 5*1000 // us
void bt_ll_hci_tx_process(void)
{
	if (hci_txfifo.rptr != hci_txfifo.wptr)
	{
		// process HCI_RX/UART_TX fifo
		uint8_t *p = hci_txfifo.p + (hci_txfifo.rptr & (hci_txfifo.num-1)) * hci_txfifo.size;

		if(uart_dma_send_flag == 1){

			if(clock_time_exceed(uart_tx_start_tick,UART_TX_BUSY_TIMEOUT)){

				transport_uart_dmatx_irq_init();
				uart_dma_send_flag = 0;
				my_dump_str_u32s(1,"ERROR_0X3E",uart_dma_send_flag,0,0,0);

			}
			else return ;
		}
		hci_start_dma_tx(UART_ID,p);
		uart_dma_send_flag = 1;
		uart_tx_start_tick = clock_time();

		int len = p[0]+p[1]*256;
		if(p[4]==HCI_TYPE_EVENT)
		{
			my_dump_str_data (DUMP_ACL_MSG, "HCI-> Event", p+4, len);
		}
		else if(p[4]==HCI_TYPE_ACL_DATA)
		{
			my_dump_str_data (DUMP_ACL_MSG, "HCI-> ACL", p+4, len);
		}
		else if(p[4]==HCI_TYPE_SCO_DATA)
		{
			my_dump_str_data (DUMP_ACL_MSG, "HCI-> SCO", p+4, len);
		}
		//hci_txfifo.rptr++;
		hci_txfifo_next (HCI_IF_UART);
	}

}
#endif

ACL_RX_RAMCODE int hci_controller_to_host(void)
{
	int ret = 0;
	hci_txfifo_read_t *ph = &hci_txfifo_r[HCI_IF_HOST];
	if (ph->rptr != hci_txfifo.wptr)
	{
		u8  app_spp_flow_temp = app_spp_flow;
		// process HCI_RX/UART_TX fifo
		uint8_t *p = hci_txfifo.p + (hci_txfifo.rptr & (hci_txfifo.num-1)) * hci_txfifo.size;
		p += 4;
		int type = p[0];
		if (HCI_TYPE_ACL_DATA == type && app_spp_flow_temp)		// ACL-U
		{
			if (bt_ll_acl_rx_callback)
			{
				int len = p[3] + p[4] * 256;
				int connhdl = p[1];
				int llid = p[2] >> 4;
				if (LLID_START == llid)
				{
					int l2cap_len = p[6]*256 + p[5];
					if (len >= (l2cap_len + 4))
					{
						// skip audio packet
						if (len > 300)
						{
							bt_ll_hci_acl_audio_pkt++;
							if (DUMP_HCI_DATA_ALL_EN || ((bt_ll_hci_acl_audio_pkt & (BIT(20)-1)) == 1))
							{
								my_dump_str_data(DUMP_ACL_MSG, "(ACL-Audio)",  p, 32);
							}
						}
						else
						{
							if (len > 20 && p[9] == 0x80 && p[10] == 0x60)
							{
								// a2dp stream
							}
							else
							{
								//my_dump_str_data(DUMP_ACL_MSG, "(ACL-U<R)", p, len > 64 ? 64 : len + 5);
							}
						}
						#if 0
							////////////// spp stress test: ////////////////////////////////////////////
							u8 *RxBuffer = p + 5;
							int nll = RxBuffer[0] | (RxBuffer[1]<<8);
							nll = nll > (127 + 5) ? RxBuffer[8] | (RxBuffer[9]<<8) : RxBuffer[7] | (RxBuffer[8]<<8);
							log_b16(SL_STACK_ACL_EN, SL16_esco_slot, nll);
						#endif
						ret = bt_ll_acl_rx_callback (connhdl, llid, p + 5, len);
						
						tmemset (p + 5, 0, len);
					}
					else
					{
						unsigned char acl_id = connhdl & 0x0f;
						if (acl_id < MAX_NB_ACTIVE_ACL)
						{
							g_acl_l2cap_buffer[acl_id].len = l2cap_len + 4;
							g_acl_l2cap_buffer[acl_id].cur_idx = len;
							tmemcpy(g_acl_l2cap_buffer[acl_id].acl_buffer_temp, p + 5, len);
						}
					}
				}
				else if (LLID_CONTINUE == llid)
				{
					unsigned char acl_id = connhdl & 0x0f;
					if (acl_id < MAX_NB_ACTIVE_ACL)
					{
						//g_acl_l2cap_buffer[acl_id].len = l2cap_len + 4;
						tmemcpy(g_acl_l2cap_buffer[acl_id].acl_buffer_temp + g_acl_l2cap_buffer[acl_id].cur_idx, p + 5, len);
						g_acl_l2cap_buffer[acl_id].cur_idx = g_acl_l2cap_buffer[acl_id].cur_idx + len;
						if (g_acl_l2cap_buffer[acl_id].cur_idx >= g_acl_l2cap_buffer[acl_id].len) // if the l2cap packet is complete
						{
							// the llid parameter should be LLID_START for rx callback
							if (g_acl_l2cap_buffer[acl_id].len > 300)
							{
								bt_ll_hci_acl_audio_pkt++;
								if ((bt_ll_hci_acl_audio_pkt & (BIT(20)-1)) == 1)
								{
									//my_dump_str_data(DUMP_ACL_MSG, "(ACL-Audio)", g_acl_l2cap_buffer[acl_id].acl_buffer_temp, 32);
								}
							}
							else
							{
								//my_dump_str_data(DUMP_ACL_MSG, "(ACL-U<R)", g_acl_l2cap_buffer[acl_id].acl_buffer_temp, g_acl_l2cap_buffer[acl_id].len);
							}
							ret = bt_ll_acl_rx_callback (connhdl, LLID_START,g_acl_l2cap_buffer[acl_id].acl_buffer_temp, g_acl_l2cap_buffer[acl_id].len);

							tmemset (g_acl_l2cap_buffer[acl_id].acl_buffer_temp, 0, sizeof (g_acl_l2cap_buffer[acl_id].acl_buffer_temp));
						}

					}
				}
			}
		}
		else if (HCI_TYPE_EVENT == type)		// Event
		{
			int ev = p[1];
			int len = p[2];
			if (bt_ll_event_callback)
			{
				ret = bt_ll_event_callback (ev, p + 3, len);
				tmemset (p + 3, 0, len);
			}
		}

		if (HCI_TYPE_ACL_DATA == type)
		{
			if(app_spp_flow_temp)
				hci_txfifo_next (HCI_IF_HOST);
		}
		else
		{
			hci_txfifo_next (HCI_IF_HOST);
		}
	}
	return ret;
}


void hci_host_to_controller(void)
{
	int nt, Lid, llid;
	unsigned short handle;

    if (hci_rxfifo.wptr != hci_rxfifo.rptr)
    {
        u8 *p = hci_rxfifo.p + (hci_rxfifo.rptr & (hci_rxfifo.num-1)) * hci_rxfifo.size;
        int len = p[0] + p[1] * 256;
        p += 4;
        u8 cmd = p[0];
        int send = 1;

        switch(cmd)
        {
            case HCI_TYPE_CMD:
            	{
	                //my_dump_str_data (DUMP_ACL_MSG, "HCI-> COMMAND", p, 16);
	                if(CO_ERROR_NO_ERROR != hci_command_exec(p, len))
	                {
	        			my_dump_str_data (DUMP_HCI_MSG, "UNKNOWN HCI COMMAND", p, len);
	        		    unsigned char status =CO_ERROR_UNKNOWN_HCI_COMMAND ;

	        			//send the Command Status event first
	        			hci_send_command_status_event(status, 1, p+1);
	                }
            	}
                break;
            case HCI_TYPE_ACL_DATA:
            	{
            		handle = p[1] + p[2] * 256;
	            	nt = p[3] + p[4] * 256;
	        		Lid = p[1] & 7;
	        		llid = p[2] >> 4;


	        		/////////   dispatch bt////////////
	        		if (handle&FLAG_BT_ACL_CONNHDL)
	        		{
		        		if (CO_ERROR_NO_ERROR == bt_ll_send_acl(Lid, llid, p + 5, nt))
		        		{
		        			if (DUMP_HCI_DATA_ALL_EN || nt < 300)
		        			{
		        				//my_dump_str_data (DUMP_ACL_MSG, "(ACL-U>t)", p, nt > 32 ? 32 : nt+5);
		        			}
		        		}
		        		else
		        		{
		        			send = 0;
		        		}
	        		}
	        		/////////   dispatch le////////////
	        		else
	        		{
                        #if (ENABLE_LE)
	        			//extern int blc_hci_handler (u8*p,int len);
	        			blc_hci_handler (p,len);// le  acl
	        			//my_dump_str_data (DUMP_ACL_MSG, "(le-ACL-U>t)", p, nt > 32 ? 32 : nt+4);
                        #endif
	        		}

            	}
            	break;
            case HCI_TYPE_SCO_DATA:

            	if(btc_ll_sco_data_tx_cb){
            		btc_ll_sco_data_tx_cb(p);
            	}
            	else{
            		my_dump_str_data (DUMP_SCO_MSG, "undefine :HCI-> SCO", p, 16);
            	}

            	break;
            default:
            	my_dump_str_data(DUMP_ACL_MSG, "HCI-> unknown command",p,16);
            	break;
        }
        hci_rxfifo.rptr += send;
    }

}

void my_hci_to_uart(uart_num_e uart)
{
	if (hci2uart.p)
	{
		if (!uart_tx_is_busy (uart) && !hci2uart.tick)
		{
			hci2uart.tick = clock_time () | 1;
			return ;
		}
		else
		{
			if (hci2uart.tick && clock_time_exceed (hci2uart.tick, 300))
			{
				hci2uart.p = 0;
				hci2uart.tick = 0;
			}
			else
			{
				return;
			}
		}
	}

	hci2uart.p = hci_txfifo_get (HCI_IF_UART, 1024);

	if (hci2uart.p)
	{
		int len = hci_txfifo_r[HCI_IF_UART].len;

		uart_send_dma(uart, hci2uart.p, len);

		hci_txfifo_next (HCI_IF_UART);
	}
}

u8 * my_uart_rx_get_buffer (void)
{
	if (hci_rxfifo_full())
	{
		return 0;
	}
	u8 *p = hci_rxfifo.p + (hci_rxfifo.wptr & (hci_rxfifo.num-1)) * hci_rxfifo.size;
	return p;
}




