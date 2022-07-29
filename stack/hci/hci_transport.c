/********************************************************************************************************
 * @file     hci_transport.c
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

#include "stack.h"
#include "hci_transport.h"
#include "../../common/usb_dbg/myudb.h"
#include "../../common/usb_dbg/myudb_usbdesc.h"


uart_init_callback_t uart_dmatx_irq_init_cb = NULL;
#if ENABLE_HCI_TRANSPORT

#define HCI_REBUILD_DUMP_EN 1

#define COPY_TIMEOUT_US  1000

unsigned int   data_store_tick = 0;

typedef struct{
    u8 type;
    u8 unuse_l;
    u8 unuse_h;
    u8 len ;

}cmd_sco_data_t;

typedef struct{
    u8 type;
    u8 handle_pbf_bcf_l;
    u8 handle_pbf_bcf_h;
    u8 len_l ;
    u8 len_h ;
}acl_data_t;
u16 last_hci_fifo_offset = 0;
u16 last_hci_fifo_full_len = 0;
u16 get_frame_len(u8 packet_type,u8* data,u16 fram_len)
{
	cmd_sco_data_t *cmd_sco_data;
	acl_data_t *acl_data;
	u16 packet_len;
	if(fram_len == 0) return 0;
	if((packet_type == HCI_TYPE_CMD)||(packet_type == HCI_TYPE_SCO_DATA)){
			if(fram_len >= 4){
			cmd_sco_data = (cmd_sco_data_t*)data;
			packet_len = cmd_sco_data->len + 4;  //cmd format type(1) + opcode(2)+len(1)
			}
			else{
			packet_len = 4;
			}
			}
	else if(packet_type == HCI_TYPE_ACL_DATA){
			if(fram_len >= 5){
			acl_data = (acl_data_t*)(data);
			packet_len = acl_data->len_l+ (acl_data->len_h*256)+ 5; //acl format type(1) + handle(2)+len(2)
			}
			else{
			packet_len = 5;
			}
			}
	else{
		 packet_len=0;
		 my_dump_str_data(HCI_REBUILD_DUMP_EN, "error_come_here",data,fram_len);
		 last_hci_fifo_offset = 0;
		 last_hci_fifo_full_len = 0;
		 my_dump_str_data(HCI_REBUILD_DUMP_EN, "indicate_host hardware_failure_event",0,0);
		 hci_send_hardware_error_event(CO_ERROR_HARDWARE_FAILURE);

		}
	return packet_len;
}

void data_push_controller_hci_fifo_process(u8* pending_buf,u32 pending_len)
{
	u8* data_p = pending_buf;
	u8* data_p_start = data_p;
	u8  hci_data_type = *data_p;
	u8  error_res = 0;
	u32 packet_len = 0;
	u32 pending_buf_len = pending_len;

	if(last_hci_fifo_full_len> PENDING_BUF_LEN/2 ){
		my_dump_str_u32s(HCI_REBUILD_DUMP_EN, "pending_buf_len_over_flow",last_hci_fifo_full_len,pending_buf_len,0,0);
		last_hci_fifo_offset = 0;
		last_hci_fifo_full_len = 0;
		hci_send_hardware_error_event(CO_ERROR_HARDWARE_FAILURE);
		return;
	}
	if(last_hci_fifo_offset){
	   u8* hci_p = my_uart_rx_get_buffer();
	   u16 dif_len = last_hci_fifo_full_len - last_hci_fifo_offset;
	   my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"packet_len0",last_hci_fifo_full_len,last_hci_fifo_offset,dif_len,pending_buf_len);

	   if(dif_len <= pending_buf_len){
         //copy packet to hci fifo directly
		 tmemcpy(&hci_p[4]+last_hci_fifo_offset,data_p,dif_len);
		 data_p = data_p+dif_len;
		 my_dump_str_data(HCI_REBUILD_DUMP_EN, "s0",hci_p,32);
		 last_hci_fifo_offset = last_hci_fifo_offset + dif_len;
		 pending_buf_len = pending_buf_len - dif_len;
		 //refresh paket length
		 packet_len = get_frame_len(hci_p[4],&hci_p[4],last_hci_fifo_offset);
		 my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"packet_len1",packet_len,last_hci_fifo_offset,dif_len,pending_buf_len);

		 if(last_hci_fifo_full_len !=packet_len){
			last_hci_fifo_full_len = packet_len;
			// refresh paket dif_len
			dif_len = last_hci_fifo_full_len - last_hci_fifo_offset;
			my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"packet_len2",dif_len,pending_buf_len,0,0);
			if(dif_len <= pending_buf_len){
			 //case1: 1 packet complete
			 tmemcpy(&hci_p[4]+last_hci_fifo_offset,data_p,dif_len);
			 data_p = data_p+dif_len;
			 pending_buf_len = pending_buf_len - dif_len;
			 last_hci_fifo_offset = 0;
			}
			else{
			//case2: 1 packet not complete ,wait new data
			tmemcpy(&hci_p[4]+last_hci_fifo_offset,data_p,pending_buf_len);
			my_dump_str_data(HCI_REBUILD_DUMP_EN, "s1",hci_p,32);
			last_hci_fifo_offset = last_hci_fifo_offset + pending_buf_len;
			my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"case2",last_hci_fifo_offset,0,0,0);
			return;
			}
		}
		data_p_start = data_p;
		last_hci_fifo_offset = 0;
		tmemcpy(hci_p,(u8 *)&last_hci_fifo_full_len,2);
		hci_rxfifo.wptr++;
		if(pending_buf_len)
		hci_data_type = *data_p;
		else return;
		my_dump_str_data(HCI_REBUILD_DUMP_EN, "last_packet_complete1",hci_p,32);
		}
		else{
			// copy packet to hci fifo directly
			tmemcpy(&hci_p[4]+last_hci_fifo_offset,data_p,pending_buf_len);
			data_p = data_p+pending_buf_len;
			last_hci_fifo_offset = last_hci_fifo_offset + pending_buf_len;
			// need refresh packet length
			if(last_hci_fifo_full_len == last_hci_fifo_offset){
			   packet_len = get_frame_len(hci_p[4],&hci_p[4],last_hci_fifo_offset);

			   if(last_hci_fifo_full_len !=packet_len){
				  // refresh packet length
				  last_hci_fifo_full_len = packet_len;
				  return;
				}

				tmemcpy(hci_p,(u8 *)&last_hci_fifo_full_len,2);
				hci_rxfifo.wptr++;
				my_dump_str_data(HCI_REBUILD_DUMP_EN, "last_packet_complete2",hci_p,32);
				return;
			}
			my_dump_str_data(1, "waitting_packet_complete",hci_p,32);
		   return;
		}
	}
	data_store_tick = clock_time();

	do{
		packet_len = get_frame_len(hci_data_type,data_p,pending_buf_len);

		if(packet_len > pending_buf_len){    // Incomplete data

		if(packet_len > PENDING_BUF_LEN) {
		  break;
		}
		my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"Incomplete_input_rx_done",packet_len,pending_buf_len,(data_p-data_p_start),0);
		error_res = 1;
		last_hci_fifo_full_len = packet_len;
		last_hci_fifo_offset = pending_buf_len;
		u8* hci_p = my_uart_rx_get_buffer();
		tmemcpy(hci_p,(u8 *)&packet_len,4);
		tmemcpy(&hci_p[4],data_p,last_hci_fifo_offset);

		break;
		}
		if(packet_len == 0){
		  break;
		}
		GLOBAL_INT_DISABLE();
		if(packet_len){
		   u8* hci_p = my_uart_rx_get_buffer();
		   tmemcpy(hci_p,(u8 *)&packet_len,4);
		   tmemcpy(&hci_p[4],data_p,packet_len);
		   my_dump_str_data(HCI_REBUILD_DUMP_EN, "fifo_hci_data ",hci_p,64);
		   hci_rxfifo.wptr++;
		   data_p = (data_p + packet_len);
		   pending_buf_len = pending_buf_len - packet_len;

		   my_dump_str_u32s(HCI_REBUILD_DUMP_EN, "packet_len ",packet_len,pending_buf_len,0,0);
		   my_dump_str_data(HCI_REBUILD_DUMP_EN, "fifo_hci_data_p ",data_p,64);
		}
		else{
			data_p++;
			pending_buf_len--;
		}
		GLOBAL_INT_RESTORE();
		hci_data_type = *data_p;


	}while(pending_buf_len);//while((data_p-data_p_start)<pending_buf_len);

	if(!error_res)
	{
		my_dump_str_data(HCI_REBUILD_DUMP_EN, "complete_hci_data from_host",0,0);
	}

}
#endif
#if(ENABLE_HCI_TRANSPORT&&(HCI_TRANSPORT_MODE==2))
extern ev_timer_list_t bt_controller_timer_pending_list;

void uart_dmarx_timeout_disable(uart_num_e uart_num)
{
	u8 temp = (REG_ADDR8(0x140088+uart_num*0x40)&0xf0)|0x80;

	write_reg8(0x140088+uart_num*0x40,temp);
}
void transport_uart_dmatx_irq_init(void)
{
	uart_set_tx_dma_config(UART_ID, UART_DMA_TX);

	uart_clr_tx_done(UART_ID);

	dma_clr_irq_mask(UART_DMA_TX,TC_MASK|ABT_MASK|ERR_MASK);

	uart_set_irq_mask(UART_ID, UART_TXDONE_MASK);

	plic_interrupt_enable(UART_IRQ);
}

void uart_dmatx_init_register_callback(uart_init_callback_t cb)
{
	uart_dmatx_irq_init_cb = cb;
}
int uart_bautrate_change(void* arg)
{
	unsigned short div;
	unsigned char bwpc;

	uart_set_dma_enable(UART_ID,0);

	uart_clr_irq_mask(UART_ID, UART_TXDONE_MASK);
	uart_clr_irq_mask(UART_ID, UART_RXDONE_MASK);

	uart_reset(UART_ID);

	uart_cal_div_and_bwpc(arg, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_dma_rx_timeout(UART_ID, bwpc, 12, UART_BW_MUL1);
	uart_init(UART_ID, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	uart_set_tx_dma_config(UART_ID, UART_DMA_TX);
	uart_set_rx_dma_config(UART_ID, UART_DMA_RX);
	uart_clr_tx_done(UART_ID);
	uart_clr_irq_status(UART_ID,UART_CLR_RX);
	dma_clr_irq_mask(UART_DMA_TX,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(UART_DMA_RX,TC_MASK|ABT_MASK|ERR_MASK);

	uart_set_irq_mask(UART_ID, UART_RXDONE_MASK);
	uart_set_irq_mask(UART_ID, UART_TXDONE_MASK);

	plic_interrupt_enable(UART_IRQ);

	tmemset((u8*)(pending_dma_buf),0,PENDING_BUF_LEN);
	uart_receive_dma(UART_ID,(u8*)(pending_dma_buf), PENDING_BUF_LEN);
	uart_dmarx_timeout_disable(UART_ID);

	uart_dmatx_init_register_callback(transport_uart_dmatx_irq_init);

	return -1;
}

void uart_time_event_cb(uint32_t baudrate)
{
	ev_on_timer(uart_bautrate_change, baudrate, 10*1000, &bt_controller_timer_pending_list);
	return 0;
}

static void rts_io_init(void)
{
    gpio_set_gpio_en(UART_RTS_PIN);
    gpio_output_en(UART_RTS_PIN);         //enable output
    gpio_input_dis(UART_RTS_PIN);        //disable input
}

void hci_uart_init(void)
{
	unsigned short div;
	unsigned char bwpc;

	uart_reset(UART_ID);

	uart_set_pin(UART_TX_PIN,UART_RX_PIN);

	uart_cal_div_and_bwpc(UART_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_dma_rx_timeout(UART_ID, bwpc, 12, UART_BW_MUL1);
	uart_init(UART_ID, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
#if UART_FLOW_CTR
	uart_cts_config(UART_ID, UART_CTS_PIN, CTS_STOP_VOLT);
	uart_set_cts_en(UART_ID);

#endif
	uart_set_tx_dma_config(UART_ID, UART_DMA_TX);
	uart_set_rx_dma_config(UART_ID, UART_DMA_RX);
	uart_clr_tx_done(UART_ID);

	dma_clr_irq_mask(UART_DMA_TX,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(UART_DMA_RX,TC_MASK|ABT_MASK|ERR_MASK);

	uart_set_irq_mask(UART_ID, UART_RXDONE_MASK);
	uart_set_irq_mask(UART_ID, UART_TXDONE_MASK);

	plic_interrupt_enable(UART_IRQ);

	tmemset((u8*)(pending_dma_buf),0,PENDING_BUF_LEN);
	uart_receive_dma(UART_ID,(u8*)(pending_dma_buf), PENDING_BUF_LEN);
	uart_dmarx_timeout_disable(UART_ID);

	if(UART_BAUDRATE_DYNAMIC_ADJUST)
	{
		hci_vendor_spec_register_hci_cmd_callback(uart_time_event_cb);
	}

    rts_io_init();

    uart_dmatx_init_register_callback(transport_uart_dmatx_irq_init);
}

/**
 * @brief		uart0 interrupt handler.
 * @param[in]	none
 * @return      none
 */
extern unsigned char uart_dma_send_flag;
unsigned char  pending_dma_buf[PENDING_BUF_LEN];
_attribute_ram_code_sec_ void UART_IRQHandler(void)
{
    if(uart_get_irq_status(UART_ID,UART_TXDONE))
	{
    	uart_dma_send_flag = 0;
	    uart_clr_tx_done(UART_ID);

	}
}

/*
 * 3M baudrate : read wait 2us ,1bit 330ns,5*330ns
 * 2M baudrate : read wait 3us ,1bit 500ns,5*500ns
 */
#define READ_STATE_WAIT_TICK  sleep_us(2) // 3M
#define RTS_TO_READ_UART  5                //us
/**
 * @brief     This function serves to get uart state is busy or idle.
 * @param[in] uart_num - UART0 or UART1.
 * @return    0 or 1
 */
volatile unsigned int status_read_rts_tick ;

 _attribute_ram_code_sec_noinline_ uint8_t is_uart_rx_done(uart_num_e uart_num)
{
	volatile unsigned char status = 0;
	volatile unsigned int status_read_start_tick = reg_system_tick;
	status = (reg_uart_state(uart_num)&0xf0);
	if (status == 0) {
		READ_STATE_WAIT_TICK;
		status = (reg_uart_state(uart_num)&0xf0);
		if (status == 0) {

			READ_STATE_WAIT_TICK;

			status = (reg_uart_state(uart_num)&0xf0);

			if (status == 0) {

				UART_MANUAL_FLOW_CTR_RTS_STOP;
				status_read_rts_tick = reg_system_tick;
				if(clock_time_exceed(status_read_start_tick,10)){

					return 0;
				}
				status = (reg_uart_state(uart_num)&0xf0);
				if (status == 0){

					return 1;
				}
				else {
					UART_MANUAL_FLOW_CTR_RTS_START;
					return 0;
				}
				return 1;
			}
			return 0;
		 }
	}
	return 0;
}
 /**
  * @brief     This function serves to get uart state is busy or idle soon.
  * @param[in] uart_num - UART0 or UART1.
  * @return    0 or 1
  */
 _attribute_ram_code_sec_noinline_ uint8_t is_uart_rx_done_soon(uart_num_e uart_num)
{

	if(clock_time_exceed(status_read_rts_tick,RTS_TO_READ_UART)){
		return 0;
	}

	volatile uint8_t status = 0;
	status = (reg_uart_state(uart_num)&0xf0);

	if (status == 0) {

		return 1;
	}

	return 0;

}

static unsigned int process_uart_rx_irq_ts;
u8  tail_uart_data[3];
_attribute_ram_code_sec_ u8 uart_dmabuf_uartbuf_process(u32 *dma_size,u8 *buf_cnt)
{
	if(!is_uart_rx_done_soon(UART_ID)) return 0;
	GLOBAL_INT_DISABLE();
	if(!is_uart_rx_done_soon(UART_ID)){
		GLOBAL_INT_RESTORE();
		return 0;
	}
	uart_set_rx_dma_enable(UART_ID,0);
	*dma_size = reg_dma_size(UART_DMA_RX);
    *buf_cnt = reg_uart_buf_cnt(UART_ID)&3;
	uart_set_rx_dma_enable(UART_ID,1);
	GLOBAL_INT_RESTORE();

	u32 read_dma_size= *dma_size;
	u8 read_buf_cnt = *buf_cnt;

	if((UART_DMA_SIZE!=read_dma_size)||read_buf_cnt){
		if(read_buf_cnt){

			uart_set_rx_dma_enable(UART_ID,0);
			for(u8 i=0;i<read_buf_cnt;i++){
				tail_uart_data[i] =	reg_uart_data_buf(UART_ID,i);
			}
			uart_clr_irq_status(UART_ID,UART_CLR_RX);
			uart_set_rx_dma_enable(UART_ID,1);
		}

		return 1;
	}
	else {
			return 0;
	}
}

_attribute_ram_code_sec_ void hci_uart_rx_process(void)
{
	if(is_uart_rx_done(UART_ID)){

		u32 read_dma_size;
		u8  read_buf_cnt;

		if(!uart_dmabuf_uartbuf_process(&read_dma_size,&read_buf_cnt)){
			UART_MANUAL_FLOW_CTR_RTS_START;
		    return;
		}

		u8 *p = pending_dma_buf;
		u16 dma_offset = PENDING_BUF_LEN - read_dma_size*4;
		u16 rx_len = 0 ;

		if(dma_offset+read_buf_cnt){
			my_dump_str_u32s(HCI_REBUILD_DUMP_EN,"dma_offset&readmasize",dma_offset,read_dma_size,read_buf_cnt,0);

			GLOBAL_INT_DISABLE();
			// dma buf + copy tail_uart_data
			if(read_buf_cnt){
			tmemcpy(p+dma_offset,tail_uart_data,read_buf_cnt);
			}
			// length in pending buf
			rx_len = dma_offset+read_buf_cnt;

			GLOBAL_INT_RESTORE();

	   }

		data_push_controller_hci_fifo_process(pending_dma_buf,rx_len);
		//reset dma&uart
		GLOBAL_INT_DISABLE();
		tmemset(pending_dma_buf,0,8);
		uart_receive_dma(UART_ID,(u8 *)(pending_dma_buf), PENDING_BUF_LEN);
		GLOBAL_INT_RESTORE();
		//release RTS
		UART_MANUAL_FLOW_CTR_RTS_START;
	}

}
void hci_start_dma_tx(uart_num_e uart_num,unsigned char * addr)
{
	int len = addr[0]+addr[1]*256;
	uart_send_dma(uart_num,addr+4,len);
}
#elif(ENABLE_HCI_TRANSPORT&&(!ENABLE_VCD)&&(HCI_TRANSPORT_MODE==1))


static HCI_USB_Request_Header_t control_request;

typedef struct myudb_cfg
{
	u16 	response_len;
	u8*		response;
	u8 		stall;

} myudb_cfg_t;

myudb_cfg_t hci_myudb = {0};


#define	my_hci_txfifo_next(f)	hci_txfifo_next(f)
#define my_hci_txfifo_r			hci_txfifo_r[HCI_IF_USB]
#define my_hci_txfifo_get()		hci_txfifo_get (HCI_IF_USB, 64)

////////////////////////////////////////////////////////////////////////////////////////////////////////
//		USB device handling
//-----------------------------------------------------------------------------------------
enum {
	HCI_USB_IRQ_SETUP_REQ = 0,
	HCI_USB_IRQ_DATA_REQ,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
void hci_usb_send_response(void) {
	u16 n;
	if (hci_myudb.response_len < 8) {
		n = hci_myudb.response_len;
	} else {
		n = 8;
	}
	hci_myudb.response_len -= n;
	usbhw_reset_ctrl_ep_ptr();
	while (n-- > 0) {
		usbhw_write_ctrl_ep_data(*hci_myudb.response);
		++hci_myudb.response;
	}
}

void hci_usb_prepare_desc_data(void) {
	u8 value_l = (control_request.wValue) & 0xff;
	u8 value_h = (control_request.wValue >> 8) & 0xff;

	hci_myudb.response = 0;
	hci_myudb.response_len = 0;

	switch (value_h) {

	case DTYPE_Device:
		hci_myudb.response = hci_usbdesc_get_device();
		hci_myudb.response_len = sizeof(HCI_USB_Descriptor_Device_t);
		break;

	case DTYPE_Configuration:
		hci_myudb.response = hci_usbdesc_get_configuration();
		hci_myudb.response_len = sizeof(HCI_USB_Descriptor_Configuration_t);
		break;

	case DTYPE_String:
		if (HCI_USB_STRING_LANGUAGE_ID == value_l) {
			hci_myudb.response = hci_usbdesc_get_language();
			hci_myudb.response_len = sizeof(LANGUAGE_ID_ENG);
		} else if (HCI_USB_STRING_VENDOR_ID == value_l) {
			hci_myudb.response = hci_usbdesc_get_vendor();
			hci_myudb.response_len = sizeof (HCI_USB_STRING_VENDOR);
		} else if (HCI_USB_STRING_PRODUCT_ID == value_l) {
			hci_myudb.response = hci_usbdesc_get_product();
			hci_myudb.response_len = sizeof(HCI_USB_STRING_PRODUCT);
		} else if (HCI_USB_STRING_SERIAL_ID == value_l) {
			hci_myudb.response = hci_usbdesc_get_serial();
			hci_myudb.response_len = sizeof(HCI_USB_STRING_SERIAL);
		} else {
			hci_myudb.stall = 1;
		}
		break;

	default:
		hci_myudb.stall = 1;
		break;

	}

	if (control_request.wLength < hci_myudb.response_len) {
		hci_myudb.response_len = control_request.wLength;
	}

	return;
}

void hci_usb_handle_in_class_intf_req() {
	u8 property = control_request.bRequest;
	switch (property) {
		case 0x00:
			usbhw_write_ctrl_ep_data(0x00);
			break;
		default:
			hci_myudb.stall = 1;
			break;
	}

}
typedef struct hci_usb_data_st
{
	u16		total_len;
	u8		r_len;
	u8		pending;
	u8		data[64];

} hci_usb_data_st_t;

volatile hci_usb_data_st_t hci_usb_comm_pram = {0};

volatile u8 iso_epd_size = 0x00;
volatile u8 iso_buff_size[6] = {0x00, 	//default
								0x09,	//alternate 1 buffsize
								0x11,	//alternate 2 buffsize
								0x19,	//alternate 3 buffsize
								0x21,	//alternate 4 buffsize
								0x3f};	//alternate 5 buffsize
void hci_usb_handle_request(u8 data_request) {
	u8 bmRequestType = control_request.bmRequestType;
	u8 bRequest = control_request.bRequest;
	usbhw_reset_ctrl_ep_ptr();
	switch (bmRequestType) {
	case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE):
		if (REQ_GetDescriptor == bRequest) {
			if (HCI_USB_IRQ_SETUP_REQ == data_request) {
				hci_usb_prepare_desc_data();
			}
			hci_usb_send_response();
		}
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE):
		hci_usb_handle_in_class_intf_req();
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_INTERFACE):
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE):	// 0xc0
		hci_myudb.stall = 1;
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE):	// 0x40
		hci_myudb.stall = 1;
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_DEVICE):		//0x20
	case (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE):	// 0x21 HCI command
			if(HCI_USB_IRQ_SETUP_REQ == data_request){
				//check command
				if((!bRequest) && (!control_request.wValue)&&(!control_request.wIndex)){
					hci_usb_comm_pram.pending = true;
					hci_usb_comm_pram.total_len = control_request.wLength;
					hci_usb_comm_pram.r_len = 0x00;
					hci_usb_comm_pram.data[hci_usb_comm_pram.r_len++] = HCI_TYPE_CMD;
				}
			}
			else{//MYUDB_USB_IRQ_DATA_REQ
				if(hci_usb_comm_pram.pending){//command is ok
					u8 i= 0;
					for(i = 0;i < HCI_USB_CONTROL_SIZE; i ++){ //edp0 size is 8

						hci_usb_comm_pram.data[hci_usb_comm_pram.r_len++]  = usbhw_read_ctrl_ep_data();
						hci_usb_comm_pram.total_len --;
						if(!hci_usb_comm_pram.total_len){
							hci_usb_comm_pram.pending = false;
							break;
						}
					}
					data_push_controller_hci_fifo_process(hci_usb_comm_pram.data,hci_usb_comm_pram.r_len);
					hci_usb_comm_pram.r_len = 0;
				}
			}
			break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE)://01
		if(REQ_SetInterface == data_request){//iso interface is 0x01
			if((control_request.wIndex == 0x01)&&(control_request.wLength == 0x00)){
				iso_epd_size = iso_buff_size[control_request.wValue];
			}
		}
			break;
	default:
		hci_myudb.stall = 1;
		break;
	}

	return;
}

void hci_usb_handle_ctl_ep_setup() {
	usbhw_reset_ctrl_ep_ptr();
	control_request.bmRequestType = usbhw_read_ctrl_ep_data();
	control_request.bRequest = usbhw_read_ctrl_ep_data();
	control_request.wValue = usbhw_read_ctrl_ep_u16();
	control_request.wIndex = usbhw_read_ctrl_ep_u16();
	control_request.wLength = usbhw_read_ctrl_ep_u16();
	hci_myudb.stall = 0;
	hci_usb_handle_request(HCI_USB_IRQ_SETUP_REQ);
	if (hci_myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void hci_usb_handle_ctl_ep_data(void) {
	usbhw_reset_ctrl_ep_ptr();
	hci_myudb.stall = 0;
	hci_usb_handle_request(HCI_USB_IRQ_DATA_REQ);
	if (hci_myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void hci_usb_handle_ctl_ep_status() {
	if (hci_myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_ACK);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

_attribute_ram_code_ void hci_event_to_usb(){
	static u32 event_len = 0;
	static u32 event_rptr;
	static uint8_t *p;
	static u8 edp_data_toggle;
	if(usbhw_is_ep_busy(HCI_USB_EDP_IN_INTERRUPT))
			return;
	if ((hci_txfifo.rptr != hci_txfifo.wptr) && ( !event_len)){
		p = hci_txfifo.p + (hci_txfifo.rptr & (hci_txfifo.num-1)) * hci_txfifo.size;
		if(p[4] == HCI_TYPE_EVENT){
			event_len = p[0]+p[1]*256 - 1; //event packet lenth , uart packet type:p+4 ;event packet start addr :p+5
			event_rptr = 0;
		}
	}
	if(event_len){
		usbhw_reset_ep_ptr(HCI_USB_EDP_IN_INTERRUPT);
		for (int i = 0; i < HCI_USB_INTERRUPT_SIZE; i ++){
			usbhw_write_ep_data(HCI_USB_EDP_IN_INTERRUPT,p[event_rptr + 5]);
			event_rptr ++;
			if(event_rptr >= event_len){
				hci_txfifo_next(HCI_IF_USB);
				event_len = 0;
				break;
			}
		}
		reg_usb_ep_ctrl(HCI_USB_EDP_IN_INTERRUPT) = FLD_EP_DAT_ACK | (edp_data_toggle ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
		edp_data_toggle ^= 1;
	}
}

volatile hci_usb_data_st_t hci_usb_acl_pram = {0};

#define			USB_ENDPOINT_BULK_OUT_FLAG		(1 << (HCI_USB_EDP_OUT_BULK & 7))
_attribute_ram_code_ void hci_acl_data_from_usb (void)
{
	if (reg_usb_ep_irq_status & USB_ENDPOINT_BULK_OUT_FLAG){
		//clear interrupt flag
		reg_usb_ep_irq_status = USB_ENDPOINT_BULK_OUT_FLAG;
		// read data len
		int n = reg_usb_ep_ptr(HCI_USB_EDP_OUT_BULK);
		reg_usb_ep_ptr(HCI_USB_EDP_OUT_BULK) = 0;
		int i;
		if(!hci_usb_acl_pram.pending){
			if(n < 4){
				reg_usb_ep_ctrl (HCI_USB_EDP_OUT_BULK) = 1;		//ACK
				return ;
			}
			hci_usb_acl_pram.r_len = 0;
			hci_usb_acl_pram.data[hci_usb_acl_pram.r_len ++] = HCI_TYPE_ACL_DATA;
			for ( i = 0; i < n; i ++){
				hci_usb_acl_pram.data[hci_usb_acl_pram.r_len ++] = reg_usb_ep_dat(HCI_USB_EDP_OUT_BULK);
			}
			hci_usb_acl_pram.total_len = hci_usb_acl_pram.data[3] | (hci_usb_acl_pram.data[4] << 8);
			data_push_controller_hci_fifo_process(hci_usb_acl_pram.data,hci_usb_acl_pram.r_len);
			hci_usb_acl_pram.r_len = 0;
			if(hci_usb_acl_pram.total_len > (HCI_USB_BULK_SIZE - 4)){
				hci_usb_acl_pram.pending = 1;
				hci_usb_acl_pram.total_len -= (HCI_USB_BULK_SIZE - 4);
			}
		}
		else{
			for ( i = 0; i < n; i ++){
				hci_usb_acl_pram.data[hci_usb_acl_pram.r_len ++] = reg_usb_ep_dat(HCI_USB_EDP_OUT_BULK);
			}
			data_push_controller_hci_fifo_process(hci_usb_acl_pram.data,hci_usb_acl_pram.r_len);
			hci_usb_acl_pram.total_len = (hci_usb_acl_pram.total_len > hci_usb_acl_pram.r_len) ?(hci_usb_acl_pram.total_len - hci_usb_acl_pram.r_len): 0;
			hci_usb_acl_pram.r_len = 0;
			if(!hci_usb_acl_pram.total_len ){
				hci_usb_acl_pram.pending = 0;
			}
		}
		reg_usb_ep_ctrl (HCI_USB_EDP_OUT_BULK) = 1;		//ACK
	}
}


_attribute_ram_code_ void  hci_acl_data_to_usb (void)
{
	static u16 acl_data_len = 0;
	static u16 acl_data_rtpr = 0;
	static uint8_t *p;
	static u8 edp_data_toggle = 0;
	int i = 0;
	if(usbhw_is_ep_busy(HCI_USB_EDP_IN_BULK)){
		return ;
	}
	if ((hci_txfifo.rptr != hci_txfifo.wptr) && ( !acl_data_len)){
		p = hci_txfifo.p + (hci_txfifo.rptr & (hci_txfifo.num-1)) * hci_txfifo.size;
		if(p[4]==HCI_TYPE_ACL_DATA){
			acl_data_len = p[0]+p[1]*256 -1; //event packet lenth ,event packet start addr :p + 4
			acl_data_rtpr = 0;
		}
	}
	if(acl_data_len){
		usbhw_reset_ep_ptr(HCI_USB_EDP_IN_BULK);
		for ( i = 0; i < HCI_USB_BULK_SIZE; i ++){
			usbhw_write_ep_data(HCI_USB_EDP_IN_BULK,p[acl_data_rtpr + 5]);
			acl_data_rtpr ++;
			if(acl_data_rtpr >= acl_data_len){
				hci_txfifo_next(HCI_IF_USB);
				acl_data_len = 0;
				break;
			}
		}
		reg_usb_ep_ctrl(HCI_USB_EDP_IN_BULK) = FLD_EP_DAT_ACK | (edp_data_toggle ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
		edp_data_toggle ^= 1;
	}
}

volatile hci_usb_data_st_t hci_usb_sco_pram = {0};

#define USB_ENDPOINT_ISO_OUT_FLAG		(1 << (HCI_USB_EDP_OUT_ISO & 7))
_attribute_ram_code_ void hci_sco_data_from_usb (void)
{
	if (reg_usb_ep_irq_status & USB_ENDPOINT_ISO_OUT_FLAG){
		//clear interrupt flag
		reg_usb_ep_irq_status = USB_ENDPOINT_ISO_OUT_FLAG;
		// read data len
		u8 n = reg_usb_ep_ptr(HCI_USB_EDP_OUT_ISO);
		reg_usb_ep_ptr(HCI_USB_EDP_OUT_ISO) = 0;
		u8 i;
		if(!hci_usb_sco_pram.pending){
			if(n <= 4)
				return ;
			hci_usb_sco_pram.data[hci_usb_sco_pram.r_len ++] = HCI_TYPE_SCO_DATA;
			for(i = 0; i < n ;i++){
				hci_usb_sco_pram.data[hci_usb_sco_pram.r_len ++] = reg_usb_ep_dat(HCI_USB_EDP_OUT_ISO);
			}
			data_push_controller_hci_fifo_process(hci_usb_sco_pram.data,hci_usb_sco_pram.r_len);
			hci_usb_sco_pram.total_len = hci_usb_sco_pram.data[3];
			hci_usb_sco_pram.r_len = 0;
			if(hci_usb_sco_pram.total_len > (iso_epd_size - 3)){
				hci_usb_sco_pram.pending = 1;
				hci_usb_sco_pram.total_len -= (iso_epd_size - 3);
			}

		}
		else{
			for(i = 0; i < n ;i++)
				hci_usb_sco_pram.data[hci_usb_sco_pram.r_len ++] = reg_usb_ep_dat(HCI_USB_EDP_OUT_ISO);
			data_push_controller_hci_fifo_process(hci_usb_sco_pram.data,hci_usb_sco_pram.r_len);
			hci_usb_sco_pram.total_len = (hci_usb_sco_pram.total_len > hci_usb_sco_pram.r_len) ?(hci_usb_sco_pram.total_len - hci_usb_sco_pram.r_len): 0;
			hci_usb_sco_pram.r_len = 0;
			if(!(hci_usb_sco_pram.total_len)){
				hci_usb_sco_pram.pending = 0;
			}
		}
		reg_usb_ep_ctrl (HCI_USB_EDP_OUT_ISO) = 1;		//ACK
	}
}

#define USB_ENDPOINT_ISO_IN_FLAG		(1 << (HCI_USB_EDP_IN_ISO & 7))
_attribute_ram_code_ void hci_sco_data_to_usb (void)
{
	static u16 sco_data_rtpr = 0;
	static u16 sco_data_len = 0;
//	static u8 sco_edp_toggle = 0;
	static uint8_t *p;
	if(usbhw_get_eps_irq()& USB_ENDPOINT_ISO_IN_FLAG){
		usbhw_clr_eps_irq(USB_ENDPOINT_ISO_IN_FLAG);	//clear interrupt flag of endpoint 7
		usbhw_reset_ep_ptr(HCI_USB_EDP_IN_ISO);
		if((hci_txfifo.rptr != hci_txfifo.wptr) && ( !sco_data_len)){
			p = hci_txfifo.p + (hci_txfifo.rptr & (hci_txfifo.num-1)) * hci_txfifo.size;
			if(p[4] == HCI_TYPE_SCO_DATA){
				sco_data_len = p[0] - 1 - 5; //event packet lenth ,event packet start addr :p + 4
				sco_data_rtpr = 0;
			}
		}
		if(sco_data_len){
			usbhw_reset_ep_ptr(HCI_USB_EDP_IN_ISO);
			for (int i = 0; i < iso_epd_size; i++){
				usbhw_write_ep_data(HCI_USB_EDP_IN_ISO,p[sco_data_rtpr + 5]);
				sco_data_rtpr ++;
				if(sco_data_rtpr >= sco_data_len){
					hci_txfifo_next(HCI_IF_USB);
					sco_data_len = 0;
					break;
				}
			}
		}
		//reg_usb_ep_ctrl(HCI_USB_EDP_IN_ISO) = FLD_EP_DAT_ACK | (sco_edp_toggle ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
		//sco_edp_toggle ^= 1;
		reg_usb_ep_ctrl(HCI_USB_EDP_IN_ISO) = FLD_EP_DAT_ACK;
	}
}

_attribute_ram_code_ void hci_usb_process()
{
	hci_event_to_usb();
	hci_acl_data_from_usb();
	hci_acl_data_to_usb();
	hci_sco_data_from_usb();
	hci_sco_data_to_usb();
}
///////////////////////////////////////////////////////////////////
_attribute_ram_code_ void hci_usb_bulkout_ready ()
{
	reg_usb_ep_ctrl (HCI_USB_EDP_OUT_BULK) = FLD_EP_DAT_ACK;
}

int  g_hci_usb_handle_irq_en = 1;
void hci_usb_handle_irq_en(int en)
{
	g_hci_usb_handle_irq_en = en;
}
/////////////////////////////////////////////////////////////////////////
void hci_usb_handle_irq(void) {
	if(g_hci_usb_handle_irq_en){		//  do nothing when in suspend. Maybe not unnecessary
		u32 irq = usbhw_get_ctrl_ep_irq();
		if (irq & FLD_CTRL_EP_IRQ_SETUP) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_SETUP);
			hci_usb_handle_ctl_ep_setup();
		}
		if (irq & FLD_CTRL_EP_IRQ_DATA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_DATA);
			hci_usb_handle_ctl_ep_data();
		}
		if (irq & FLD_CTRL_EP_IRQ_STA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_STA);
			hci_usb_handle_ctl_ep_status();
		}
		if (reg_usb_irq_mask & FLD_USB_IRQ_RESET_O)
		{
			reg_usb_irq_mask |= FLD_USB_IRQ_RESET_O; 		//Clear USB reset flag
			hci_usb_bulkout_ready ();
	    }
		hci_myudb.stall = 0;
	}
	hci_usb_process();

}

void hci_usb_init(void)
{
	reg_usb_ep_max_size = 192 >> 3;

	reg_usb_ep_buf_addr(HCI_USB_EDP_IN_INTERRUPT) = 0;

	reg_usb_ep_buf_addr(HCI_USB_EDP_IN_BULK) = 32;
	reg_usb_ep_buf_addr(HCI_USB_EDP_OUT_ISO) = 64;
	reg_usb_ep_buf_addr(HCI_USB_EDP_IN_ISO) = 128;
	reg_usb_ep_buf_addr(HCI_USB_EDP_OUT_BULK) = 192;
	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC);
	hci_usb_bulkout_ready ();
	usb_set_pin_en();
}
#endif


#if (ENABLE_PRINT)&&(PRINT_MODE == 1)
volatile u8 uart_dbg_send_flag = 1;
void uart_dbg_init(void)
{
	u16 div;
	u8 bwpc;
	uart_reset(UART_PRINTF_ID);
	uart_set_pin(UART_PRINTF_IO_TX,UART_PRINTF_IO_RX);
	uart_cal_div_and_bwpc(UART_PRINTF_BAUDRATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART_PRINTF_ID, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	uart_set_tx_dma_config(UART_PRINTF_ID, DMA4);
	uart_clr_tx_done(UART_PRINTF_ID);

}
void printf_str_data(u8 en,char *str, u8 *ph, u16 n)
{
	if(!en) return;
	u16 ns;
	u8 ph_tmp;
	u8 hex_c = 0;
	if(hci_print_fifo_full())	return;
	u8 *ps = print_fifo.p + (print_fifo.wptr & (print_fifo.num-1)) * print_fifo.size;
	u8 *pd = ps + 4;
	ns = str ? strlen (str) : 0;
	if(( ns + 2) > (PRINT_TX_FIFO_SIZE - 4)){
		ns = PRINT_TX_FIFO_SIZE -4 -2;
		n = 0;
	}
	if((n * 3 + ns + 1)> PRINT_TX_FIFO_SIZE-4){
		n = (PRINT_TX_FIFO_SIZE -4 - ns - 2) / 3;
	}
	tmemcpy(pd,str,ns);// copy string code
	pd = pd + ns;
	*pd++ = ':';
    ns= ns+1;
	for(u16 i=0;i<n;i++){
		ph_tmp = (*ph++);
		hex_c = ph_tmp >> 4;
		*pd++ = (hex_c > 9)?(hex_c + 87):(hex_c + 48);

		hex_c = ph_tmp & 0x0f;
		*pd++ = (hex_c > 9)?(hex_c + 87):(hex_c + 48);

		if(i<(n-1)){
			*pd++ = ' ';
		}
		else{
			*pd++ = '\n';
		}
	}
	if(n == 0) {
		 n=1;
		*pd++ = '\n';
	}
	else{
		n= 3*n;
	}
	u16 len = ns + n;
	*ps++ = len&0xff;
	*ps++ = (len>>8)&0xff;

	print_fifo.wptr++;
}
void printf_str_u32s (u8 en,char *str, u32 d0, u32 d1, u32 d2, u32 d3)
{
	if(!en) return;
	u32 d[4];
	d[0] = d0;
	d[1] = d1;
	d[2] = d2;
	d[3] = d3;
	printf_str_data(en ,str, (u8*)d, 16);
}
void printf_process(void)
{
	if(hci_print_fifo_empty()) return;
    u8 *p = print_fifo.p + (print_fifo.rptr & (print_fifo.num-1)) * print_fifo.size;
    u16 len = p[0]+p[1]*256;

	if(uart_dbg_send_flag){
	   uart_send_dma(UART_PRINTF_ID, p+4, len);
	   uart_dbg_send_flag = 0;
	}
	if(uart_get_irq_status(UART_PRINTF_ID,UART_TXDONE)){
	   uart_dbg_send_flag = 1;
	   uart_clr_tx_done(UART_PRINTF_ID);
	   print_fifo.rptr++;
	}
}
#endif
#if (HCI_TRANSPORT_MODE != 0)
#if((ENABLE_PRINT == 0)||(HCI_TRANSPORT_MODE ==1))
void myudb_usb_handle_irq(void)
{

}
#endif
#endif
int my_hci_rx_callback(unsigned char *p, int len)
{
	uint8_t *pc = my_fifo_wptr (&hci_rxfifo);
	if (pc)
	{
		pc[0] = len;
		pc[1] = len >> 8;
		memcpy (pc + 4, p, len);
		my_fifo_next (&hci_rxfifo);
		return 0;
	}
	return 1;
}

void hci_transport_init(uint8_t transport_mode)
{
#if ENABLE_HCI_TRANSPORT

#if(HCI_TRANSPORT_MODE!=HCI_IF_UART)
	#if(((!ENABLE_VCD)|VCD_MODE))
		hci_usb_init();
	#else
		#if ((ENABLE_PRINT&&(!PRINT_MODE)) || (ENABLE_VCD&&(!VCD_MODE)))
			myudb_usb_init(DEVICE_BID, &hci_txfifo, &print_fifo);
		#else
			myudb_usb_init(DEVICE_BID, &hci_txfifo, NULL);
		#endif
		myudb_register_hci_cb(my_hci_rx_callback);
		usb_set_pin_en();
	#endif
#elif(HCI_TRANSPORT_MODE==HCI_IF_UART)
	hci_uart_init();
#endif
	hci_txfifo_set_rptr_reference (transport_mode);
#else
	hci_txfifo_set_rptr_reference (HCI_IF_HOST);
#endif

}
