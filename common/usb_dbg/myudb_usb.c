/********************************************************************************************************
 * @file     myudb_usb.c
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

#if(1)

#define			HCI_VCD_EN		0

#include "drivers.h"
#include "stack/hci/bt_hci.h"
#include "myudb.h"
#include "myudb_usbdesc.h"

// set by qipeng 2021.11.12
#ifndef  MYUDB_DISABLE
#ifndef		MYUDB_LOCAL_BUFFER_EN
#define		MYUDB_LOCAL_BUFFER_EN	0
#endif

#ifndef  BUFFUSB_NUM
#define			BUFFUSB_NUM		16
#endif

#ifndef BUFFUSB_SIZE
#define			BUFFUSB_SIZE	280
#endif

typedef struct myudb_cfg
{
	u16		id;
	u16 	response_len;

	u8*		response;
	u32		tick_sync;

	u8 		stall;
	u8		rptr;
	u16		cmd_len;

} myudb_cfg_t;

myudb_cfg_t myudb = {0x120};

static USB_Request_Header_t control_request;


u8 		 	myudb_txfifo_b[BUFFUSB_SIZE * BUFFUSB_NUM];
_attribute_data_retention_ my_fifo_t	myudb_txfifo = {	BUFFUSB_SIZE,
							BUFFUSB_NUM,
							0,
							0,
							myudb_txfifo_b};

#if MYUDB_LOCAL_BUFFER_EN
_attribute_data_retention_ my_fifo_t	*myudb_fifo = &myudb_txfifo;
#else
_attribute_data_retention_ my_fifo_t	*myudb_fifo = 0;
#endif

_attribute_data_retention_ my_fifo_t   *myudb_print_fifo = 0;


#if MYUDB_LOCAL_BUFFER_EN

typedef struct myudb_txfifo_read
{
	u8 *	p;
	u16		left;
	u16		len;
} __attribute__ ((aligned (4))) myudb_txfifo_read_t;


myudb_txfifo_read_t myudb_txfifo_r = {0,0,0};

typedef struct dump2uart_cfg
{
	u8*	p;
	u32	tick;
} dump2uart_cfg_t;

dump2uart_cfg_t	dump2uart = {0, 0};

_attribute_ram_code_ u8*	myudb_txfifo_get (int nsize)
{
	u8 *pd = 0;
	myudb_txfifo_read_t *ph = &myudb_txfifo_r;

	if (!ph->len)
	{
		ph->p = 0;
	}

	if (!ph->p && (myudb_fifo->wptr != myudb_fifo->rptr))		//first packet
	{
		u8* pt = myudb_fifo->p + (myudb_fifo->rptr & (myudb_fifo->num - 1)) * myudb_fifo->size;
		ph->p = pt + 4;
		ph->left = pt[0] + pt[1] * 256;
	}

	if (ph->p)
	{
		pd = ph->p;

		ph->len = ph->left > nsize ? nsize : ph->left;

		ph->p += ph->len;

		ph->left -= ph->len;

	}
	return pd;
}

_attribute_ram_code_ void myudb_txfifo_next ()
{
	myudb_txfifo_r.p = 0;
	myudb_txfifo_r.len = 0;
	myudb_txfifo_r.left = 0;
	myudb_fifo->rptr ++;
}

_attribute_ram_code_ void myudb_dump_to_usb()
{
	if(usbhw_is_ep_busy(MYUDB_EDP_IN_HCI)) return;

	u8 *ps = myudb_txfifo_get (64);
	if (ps)
	{
		int len = myudb_txfifo_r.len;

		usbhw_reset_ep_ptr(MYUDB_EDP_IN_HCI);
		for (int i=0; i<len; i++)
		{
			usbhw_write_ep_data(MYUDB_EDP_IN_HCI, *ps++);
		}

		usbhw_data_ep_ack(MYUDB_EDP_IN_HCI);

		if (len < 64)
		{
			myudb_txfifo_next ();
		}
	}
}

void my_dump_to_uart(uart_num_e uart)
{
	if (dump2uart.p)
	{
		if (!uart_tx_is_busy (uart) && !dump2uart.tick)
		{
			dump2uart.tick = clock_time () | 1;
			return ;
		}
		else
		{
			if (dump2uart.tick && clock_time_exceed (dump2uart.tick, 300))
			{
				dump2uart.p = 0;
				dump2uart.tick = 0;
			}
			else
			{
				return;
			}
		}
	}

	dump2uart.p = myudb_txfifo_get (1024);

	if (dump2uart.p)
	{
		int len = myudb_txfifo_r.len;

		uart_send_dma(uart, dump2uart.p, len);

		myudb_txfifo_next();

	}
}

#define	my_hci_txfifo_next(f)	myudb_txfifo_next()
#define my_hci_txfifo_r			myudb_txfifo_r
#define my_hci_uartfifo_r		myudb_uartfifo_r
#define my_hci_txfifo_get()		myudb_txfifo_get (64)

#else

#define	my_hci_txfifo_next(f)	hci_txfifo_next(f)
#define my_hci_txfifo_r			hci_txfifo_r[HCI_IF_USB]
#define my_hci_txfifo_get()		hci_txfifo_get (HCI_IF_USB, 64)

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		USB device handling
//-----------------------------------------------------------------------------------------
enum {
	MYUDB_USB_IRQ_SETUP_REQ = 0,
	MYUDB_USB_IRQ_DATA_REQ,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------
void myudb_usb_send_response(void) {
	u16 n;
	if (myudb.response_len < 8) {
		n = myudb.response_len;
	} else {
		n = 8;
	}
	myudb.response_len -= n;
	usbhw_reset_ctrl_ep_ptr();
	while (n-- > 0) {
		usbhw_write_ctrl_ep_data(*myudb.response);
		++myudb.response;
	}
}

void myudb_usb_prepare_desc_data(void) {
	u8 value_l = (control_request.wValue) & 0xff;
	u8 value_h = (control_request.wValue >> 8) & 0xff;

	myudb.response = 0;
	myudb.response_len = 0;

	switch (value_h) {
		
	case DTYPE_Device:
		myudb.response = myudb_usbdesc_get_device();
		myudb.response_len = sizeof(USB_Descriptor_Device_t);
		break;

	case DTYPE_Configuration:
		myudb.response = myudb_usbdesc_get_configuration();
		myudb.response_len = sizeof(MYUDB_USB_Descriptor_Configuration_t);
		break;

	case DTYPE_String:
		if (MYUDB_USB_STRING_LANGUAGE == value_l) {
			myudb.response = myudb_usbdesc_get_language();
			myudb.response_len = sizeof(LANGUAGE_ID_ENG);
		} else if (MYUDB_USB_STRING_VENDOR == value_l) {
			myudb.response = myudb_usbdesc_get_vendor();
			myudb.response_len = sizeof (MYUDB_STRING_VENDOR);
		} else if (MYUDB_USB_STRING_PRODUCT == value_l) {
			myudb.response = myudb_usbdesc_get_product();
			myudb.response_len = sizeof(MYUDB_STRING_PRODUCT);
		} else if (MYUDB_USB_STRING_SERIAL == value_l) {
			myudb.response = myudb_usbdesc_get_serial();
			myudb.response_len = sizeof(MYUDB_STRING_SERIAL);
		} else {
			myudb.stall = 1;
		}
		break;

	default:
		myudb.stall = 1;
		break;

	}

	if (control_request.wLength < myudb.response_len) {
		myudb.response_len = control_request.wLength;
	}

	return;
}

void myudb_usb_handle_in_class_intf_req() {
	u8 property = control_request.bRequest;
	switch (property) {
		case 0x00:
			usbhw_write_ctrl_ep_data(0x00);
			break;
		default:
			myudb.stall = 1;
			break;
	}
	
}

void myudb_usb_handle_request(u8 data_request) {
	u8 bmRequestType = control_request.bmRequestType;
	u8 bRequest = control_request.bRequest;

	usbhw_reset_ctrl_ep_ptr();
	switch (bmRequestType) {
	case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE):
		if (REQ_GetDescriptor == bRequest) {
			if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
				myudb_usb_prepare_desc_data();
			}
			myudb_usb_send_response();
		} 
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE):
		myudb_usb_handle_in_class_intf_req();
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_INTERFACE):
		if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
			if (0xc0 == bRequest) {					// Get board version
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(myudb.id);
				usbhw_write_ctrl_ep_data(myudb.id >> 8);
			}
			else
			{
				myudb.stall = 1;
			}
		}
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE):	// 0xc0
		if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
			if (0xc0 == bRequest) {					// Get board version
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(0x40);
				usbhw_write_ctrl_ep_data(0x25);
				usbhw_write_ctrl_ep_data(0x40);
				usbhw_write_ctrl_ep_data(0x05);
				usbhw_write_ctrl_ep_data(0x03);
				usbhw_write_ctrl_ep_data(0x00);
				usbhw_write_ctrl_ep_data(0x01);
				usbhw_write_ctrl_ep_data(0x00);
			}
			else if (0xc6 == bRequest) {			// 
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(0x04);
			}
			else
			{
				myudb.stall = 1;
			}
		}
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE):	// 0x40
		myudb.stall = 1;
		break;
	default:
		myudb.stall = 1;
		break;
	}

	return;
}

void myudb_usb_handle_ctl_ep_setup() {
	usbhw_reset_ctrl_ep_ptr();
	control_request.bmRequestType = usbhw_read_ctrl_ep_data();
	control_request.bRequest = usbhw_read_ctrl_ep_data();
	control_request.wValue = usbhw_read_ctrl_ep_u16();
	control_request.wIndex = usbhw_read_ctrl_ep_u16();
	control_request.wLength = usbhw_read_ctrl_ep_u16();
	myudb.stall = 0;
	myudb_usb_handle_request(MYUDB_USB_IRQ_SETUP_REQ);
	if (myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void myudb_usb_handle_ctl_ep_data(void) {
	usbhw_reset_ctrl_ep_ptr();
	myudb.stall = 0;
	myudb_usb_handle_request(MYUDB_USB_IRQ_DATA_REQ);
	if (myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void myudb_usb_handle_ctl_ep_status() {
	if (myudb.stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_ACK);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void myudb_set_txfifo_local ()
{
	myudb_fifo = &myudb_txfifo;
}

void myudb_set_txfifo (void *p)
{
	myudb_fifo = p;
}

_attribute_retention_code_ int  myudb_print_fifo_full (void)
{
	u8 n = myudb_print_fifo->wptr - myudb_print_fifo->rptr;
	return n >= myudb_print_fifo->num;
}

_attribute_retention_code_ void usb_send_status_pkt(u8 status, u8 buffer_num, u8 *pkt, u16 len)
{
//	if (myudb_print_fifo_full()) return;		//skip if overflow

	u8 *p = myudb_print_fifo->p + (myudb_print_fifo->wptr & (myudb_print_fifo->num - 1)) * myudb_print_fifo->size;
	if (len > 272)
	{
		len = 272;
	}
	*p++ = len + 2;
	*p++ = (len + 2) >> 8;
	 p += 2;
	*p++ = status;
	*p++ = buffer_num;
	while (len--)
	{
		*p++ = *pkt++;
	}
	myudb_print_fifo->wptr++;
}

_attribute_retention_code_ void usb_send_str_data (char *str, u8 *ph, int n)
{
//	if (myudb_print_fifo_full()) return;		//skip if overflow

	GLOBAL_INT_DISABLE ();
	u8 *ps =  myudb_print_fifo->p + (myudb_print_fifo->wptr & (myudb_print_fifo->num - 1)) * myudb_print_fifo->size;;
	u8 *pd = ps;

	int ns;

	if ((g_plic_preempt_en & BIT(1)) && (u32)str >= 0x20000000)
	{
		ns = 0;
	}
	else
	{
		ns = str ? strlen (str) : 0;
	}
	if (ns > myudb_print_fifo->size - 12)
	{
		ns = myudb_print_fifo->size - 12;
		n = 0;
	}
	if (n + ns > myudb_print_fifo->size - 12)
	{
		n = myudb_print_fifo->size - 12 - ns;
	}

	int len = n + ns + 2 + 3;
	*pd++ = len;
	*pd++ = len >> 8;
	*pd++ = 0;
	*pd++ = 0;
	*pd++ = 0x82;
	*pd++ = 8;

	*pd++ = 0x22;
	*pd++ = n;
	*pd++ = n >> 8;
    
	while (n--)
	{
		*pd++ = *ph++;
	}
	while (ns--)
	{
		*pd++ = *str++;
	}
	myudb_print_fifo->wptr++;
	GLOBAL_INT_RESTORE ();
}

_attribute_retention_code_ void usb_send_str_u32s (char *str, u32 d0, u32 d1, u32 d2, u32 d3)
{
	u32 d[4];
	d[0] = d0;
	d[1] = d1;
	d[2] = d2;
	d[3] = d3;
	usb_send_str_data (str, (u8*)d, 16);
}
_attribute_retention_code_ void usb_send_str_data_cb (u8 en, char *str, u8 *ph, int n)
{
	if(!en) return;
	int len = n>64? 64:n;
	usb_send_str_data(str,  ph, len);
}
_attribute_retention_code_ void usb_send_str_u32s_cb (u8 en,char *str, u32 d0, u32 d1, u32 d2, u32 d3)
{
	if(!en) return;
	usb_send_str_u32s(str,  d0,  d1,  d2,  d3);
}

int	usb_send_str_int (char *str,int w)
{
	if (myudb_print_fifo_full())
	{
		return -1;
	}
	int len;

	GLOBAL_INT_DISABLE ();
	char buf[13], tmp, *p;
	int u;
	char symbol = ' ';
	int  int_len=0;
	int n = strlen (str);
	if (n == 0)
	{
		return -2;
	}

	p = buf + 12;
	*p = '\0';
	u = w;
	if(w<0)
	{
		symbol = '-';
		u = -w;
	}
	do{ // at least one time..
	        tmp = u % 10;
			*--p = tmp + '0';
			u /= 10;
			int_len++;
	}while(u);



	u8 *pd = myudb_print_fifo->p + (myudb_print_fifo->wptr & (myudb_print_fifo->num-1)) * myudb_print_fifo->size;
	len = n +  2 + 1 + 1 + int_len; //str_len + '0x20' + symbol + int_len
	*pd++ = len ;
	*pd++ = len >> 8;


	*pd++ = 0;
	*pd++ = 0;
	*pd++ = 0x82;
	*pd++ = 8;

	*pd++ = 0x20;
	while (n--)
	{
		*pd++ = *str++;
	}
	*pd++ = symbol;
	while(int_len--)
	{
		*pd++ = *p++;
	}
	myudb_print_fifo->wptr++;
	GLOBAL_INT_RESTORE ();
	return len;
}

#if MODULE_USB_ENABLE
extern u8 edp_toggle[8];
#endif

_attribute_ram_code_ void myudb_hci_to_usb()
{
	if(usbhw_is_ep_busy(MYUDB_EDP_IN_HCI)) return;

	u8 *ps = my_hci_txfifo_get ();
	if (ps)
	{
		int len = my_hci_txfifo_r.len;

		usbhw_reset_ep_ptr(MYUDB_EDP_IN_HCI);
		for (int i=0; i<len; i++)
		{
			usbhw_write_ep_data(MYUDB_EDP_IN_HCI, *ps++);
		}
#if MODULE_USB_ENABLE
		reg_usb_ep_ctrl(MYUDB_EDP_IN_HCI) = FLD_EP_DAT_ACK | (edp_toggle[MYUDB_EDP_IN_HCI] ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
		edp_toggle[MYUDB_EDP_IN_HCI] ^= 1;
#else
		usbhw_data_ep_ack(MYUDB_EDP_IN_HCI);
#endif


		if (len < 64)
		{
			my_hci_txfifo_next (HCI_IF_USB);
		}
	}
}

#define			USB_ENDPOINT_BULK_OUT_FLAG		(1 << (MYUDB_EDP_OUT_HCI & 7))

_attribute_ram_code_ int myudb_usb_to_hci (u8 *p)
{
	if (reg_usb_ep_irq_status & USB_ENDPOINT_BULK_OUT_FLAG)
	{
		//clear interrupt flag
		reg_usb_ep_irq_status = USB_ENDPOINT_BULK_OUT_FLAG;

		// read data
		int n = reg_usb_ep_ptr(MYUDB_EDP_OUT_HCI);
		reg_usb_ep_ptr(MYUDB_EDP_OUT_HCI) = 0;
		for (int i=0; i<n; i++)
		{
			p[myudb.cmd_len++] = reg_usb_ep_dat(MYUDB_EDP_OUT_HCI);
		}
		reg_usb_ep_ctrl (MYUDB_EDP_OUT_HCI) = 1;		//ACK
		if (n < 64)
		{
			n = myudb.cmd_len;
			myudb.cmd_len = 0;
			return n;
		}
	}
	return 0;
}

func_myudb_hci_cmd_cb_t	myudb_hci_cmd_cb = 0;

void myudb_register_hci_cb (void *p)
{
	myudb_hci_cmd_cb = p;
}

func_myudb_hci_cmd_cb_t	myudb_hci_debug_cb = 0;
func_myudb_hci_cmd_cb_t	myudb_hci_eq_cb = 0;

void myudb_register_hci_debug_cb (void *p)
{
	myudb_hci_debug_cb = p;
}

void myudb_register_hci_eq_cb (void *p)
{
	myudb_hci_eq_cb = p;
}

#define			MYHCI_FW_DOWNLOAD			0xfe
int fw_download = 0;

_attribute_ram_code_ int myudb_mem_cmd (u8 *p, int nbyte)
{
	int len = nbyte;
	int cmd = p[0];
	u8	rsp[280];

	int ret = 0;

	//////////////////////////  Memory Read ////////////////////////////////////
	if (cmd == 0x28 && len >= 8)
	{
		usb_send_status_pkt (0x81, 8, p, 12);
		rsp[0] = 0x29;
		memcpy (rsp + 1, p + 1, 5);
		int type = p[1];
		u32 adr = p[2] | (p[3] << 8) | (p[4] << 16) | (p[5] << 24);
		int n = p[6] | (p[7] << 8);
		if (n > 256)
		{
			n = 256;
		}

		if (type == 0)
		{
			memcpy (rsp + 6, (void *)(adr | 0), n);
		}
		else if (type == 1)
		{
			for (int i=0; i<n; i++)
			{
				rsp[i + 6] = analog_read_reg8 (adr + i);
			}
		}
		else if (type == 2 || type == 3)					//flash
		{
			flash_read_page (adr, n, rsp + 6);
		}

		usb_send_status_pkt (0x82, 8, rsp, n + 6);
	}
	//////////////////////////  Memory Write ////////////////////////////////////
	else if (cmd == 0x2a && len > 6)
	{
		usb_send_status_pkt (0x81, 8, p, 12);
		rsp[0] = 0x2b;
		memcpy (rsp + 1, p + 1, 16);
		u8 type = p[1];
		u32 adr = p[2] | (p[3] << 8) | (p[4] << 16) | (p[5] << 24);
		int n = len - 6;

		if (type == 0)				//RAM
		{
			memcpy ((void *)adr, p + 6, n);
		}
		else if (type == 1)			//Analog Register
		{
			for (int i=0; i<n; i++)
			{
				analog_write_reg8 (adr + i, p[i + 6]);
			}
		}
		else if (type == 2)			// flash
		{
			if (fw_download && (adr & 0xfff) == 0)
			{
				flash_erase_sector (adr);
			}
			flash_write_page (adr, n, p + 6);
		}
		else if (type == 3)
		{
			int nb = p[6];
			if (n > 1)
				nb += p[7] << 8;
			if (n > 2)
				nb += p[8] << 16;
			if (n > 3)
				nb += p[9] << 24;

			if (nb)
			{
				for (int i=0; i<nb; i+=4096)
				{
					flash_erase_sector (adr + i);
				}
			}
			else
			{
				flash_erase_chip ();
			}
		}
		else if (type == MYHCI_FW_DOWNLOAD)
		{
			core_disable_interrupt();

			myudb.rptr = myudb_fifo->wptr;

			myudb_print_fifo->rptr = myudb_print_fifo->wptr;

			//extern void hci_txfifo_set_rptr_reference (int idx);
			//hci_txfifo_set_rptr_reference (1);

			ret = MYHCI_FW_DOWNLOAD;
		}
		usb_send_status_pkt (0x82, 8, rsp, 14);
	}
	else
	{
		ret = 1;
	}
	return ret;
}

u8 buff_usb_cmd[320];
_attribute_ram_code_ int myudb_hci_cmd_from_usb (void)
{
	fw_download = 0;
	do
	{
		int n = myudb_usb_to_hci (buff_usb_cmd);
		if (n)
		{
			int r = myudb_mem_cmd(buff_usb_cmd, n);

			if (r == MYHCI_FW_DOWNLOAD)
			{
				fw_download =  MYHCI_FW_DOWNLOAD;
			}
            else if (0x11 == buff_usb_cmd[0] && myudb_hci_debug_cb && !fw_download)
            {
                myudb_hci_debug_cb (buff_usb_cmd, n);
            }
            else if (buff_usb_cmd[0] >= 0x12 && buff_usb_cmd[0] <= 0x16 && myudb_hci_eq_cb && !fw_download)
            {
                myudb_hci_eq_cb (buff_usb_cmd, n);
            }
			else if (r && myudb_hci_cmd_cb && !fw_download)
			{
				myudb_hci_cmd_cb (buff_usb_cmd, n);
			}
		}
		if (fw_download)
		{
			myudb_hci_to_usb ();
		}
	} while (fw_download);

	return 0;
}
int  g_myudb_usb_handle_irq_en = 1;
void myudb_usb_handle_irq_en(int en)
{
	g_myudb_usb_handle_irq_en = en;
}
/////////////////////////////////////////////////////////////////////////
void myudb_usb_handle_irq(void) {
	if(g_myudb_usb_handle_irq_en){		//  do nothing when in suspend. Maybe not unnecessary
		u32 irq = usbhw_get_ctrl_ep_irq();
		if (irq & FLD_CTRL_EP_IRQ_SETUP) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_SETUP);
			myudb_usb_handle_ctl_ep_setup();
			if (!myudb.tick_sync)
			{
				myudb.tick_sync = stimer_get_tick () | 1;
			}
		}
		if (irq & FLD_CTRL_EP_IRQ_DATA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_DATA);
			myudb_usb_handle_ctl_ep_data();
		}
		if (irq & FLD_CTRL_EP_IRQ_STA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_STA);
			myudb_usb_handle_ctl_ep_status();
		}

		if (reg_usb_irq_mask & FLD_USB_IRQ_RESET_O)
		{
			reg_usb_irq_mask |= FLD_USB_IRQ_RESET_O; 		//Clear USB reset flag
			myudb_usb_bulkout_ready ();
	    }
		myudb.stall = 0;
	}

	myudb_hci_to_usb ();

	myudb_hci_cmd_from_usb ();

	if (myudb.tick_sync && sys_timeout (myudb.tick_sync, 10000))
	{
		myudb.tick_sync = stimer_get_tick () | 1;
		log_sync (SL_STACK_VCD_EN);
		#if (VCD_BLE_EN)
				log_sync_mainloop(SL_STACK_VCD_EN)
		#endif
	}

}

void myudb_usb_bulkout_ready ()
{
	reg_usb_ep_ctrl (MYUDB_EDP_OUT_HCI) = FLD_EP_DAT_ACK;
}

#if VCD_MODE

MYFIFO_INIT(uart_vcd_txfifo, 128, 16);


_attribute_ram_code_sec_  void uart_send_byte_dma(uart_num_e uart_num, unsigned char tx_data)
{
	uart_vcd_txfifo.p[uart_vcd_txfifo.wptr] = tx_data;
	if((uart_vcd_txfifo.wptr%64)==63)
	{
		uart_send_dma(uart_num, uart_vcd_txfifo.p+uart_vcd_txfifo.wptr-63, 64);
	}
	uart_vcd_txfifo.wptr++;
	if(uart_vcd_txfifo.wptr>=uart_vcd_txfifo.size*uart_vcd_txfifo.num)
	{
		uart_vcd_txfifo.wptr=0;
	}
}

#define VCD_UART_DMA_CHANNEL_RX  DMA6
#define VCD_UART_DMA_CHANNEL_TX  DMA7

void uart_vcd_init(void)
{
	// 1.initiate gpio pin for log
	gpio_function_en(GPIO_PE2);
	gpio_output_en(GPIO_PE2); 		//enable output
	gpio_input_dis(GPIO_PE2);	    //disable input

	unsigned short div;
	unsigned char bwpc;

	uart_reset(UART1);

	uart_set_pin(UART1_TX_PE0,UART1_RX_PC7);// UART1_RX_PE2 ->  VCD HW_ref

	uart_cal_div_and_bwpc(921600, sys_clk.pclk*1000*1000, &div, &bwpc);

	uart_set_dma_rx_timeout(UART1, bwpc, 12, UART_BW_MUL2);
	uart_init(UART1, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	uart_set_tx_dma_config(UART1, VCD_UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART1, VCD_UART_DMA_CHANNEL_RX);
	uart_clr_tx_done(UART1);
	dma_clr_irq_mask(VCD_UART_DMA_CHANNEL_TX,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(VCD_UART_DMA_CHANNEL_RX,TC_MASK|ABT_MASK|ERR_MASK);

	uart_set_irq_mask(UART1, UART_RXDONE_MASK);
	uart_set_irq_mask(UART1, UART_TXDONE_MASK);
}
#endif

void myudb_usb_init(u16 id, void *p, void * p_print)
{
	if (!myudb_fifo && p)
	{
		myudb_fifo = p;
	}

	if (!myudb_print_fifo)
	{
		myudb_print_fifo = p_print ? p_print : myudb_fifo;
	}

	myudb_fifo->wptr = myudb_fifo->rptr = 0;

	myudb_print_fifo->wptr = myudb_print_fifo->rptr = 0;

	memset (&myudb, 0, sizeof (myudb));

	myudb.id = id;
	//reg_usb_ep_irq_mask = BIT(7);			//audio in interrupt enable
	//reg_irq_mask |= FLD_IRQ_IRQ4_EN;
	reg_usb_ep_max_size = (128 >> 2);
	reg_usb_ep8_send_thre = 0x40;
	reg_usb_ep8_send_max = 128 >> 3;
	reg_usb_ep_buf_addr (MYUDB_EDP_IN_HCI) = 128;
	reg_usb_ep_buf_addr (MYUDB_EDP_OUT_HCI) = 192;
	reg_usb_ep_buf_addr (MYUDB_EDP_IN_VCD) = 0;
	reg_usb_ep8_fifo_mode = 1;
	reg_usb_mdev &= ~BIT(3);			//vendor command: bRequest[7] = 0

	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC);

	myudb_usb_bulkout_ready ();
}

#endif
#endif
