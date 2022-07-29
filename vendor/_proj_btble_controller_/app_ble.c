/********************************************************************************************************
 * @file     app_ble.c
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

#include "drivers.h"
#include "stack/ble/ble.h"
#include "vendor/common/blt_common.h"
#include "fifo.h"




/**
 * @brief	connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_RX_OCTETS			27


/**
 * @brief	connMaxTxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 *  in this SDK, we separate this value into 2 parts: slaveMaxTxOctets and masterMaxTxOctets,
 *  for purpose to save some SRAM costed by when slave and master use different connMaxTxOctets.
 * usage limitation:
 * 1. slaveMaxTxOctets and masterMaxTxOctets should be in range of 27 ~ 251
 */
#define ACL_MASTER_MAX_TX_OCTETS		27
#define ACL_SLAVE_MAX_TX_OCTETS			27




/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/
#define ACL_RX_FIFO_SIZE		64
#define ACL_RX_FIFO_NUM			8

//_attribute_ble_data_retention_
_attribute_data_no_init_		u8	app_acl_rxfifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};


#define ACL_TX_FIFO_SIZE		48
#define ACL_TX_FIFO_NUM			9 //only 9 and 17 can be used


_attribute_ble_data_retention_	u8	app_acl_slaveTxfifo [ACL_TX_FIFO_SIZE * ACL_TX_FIFO_NUM * SLAVE_MAX_NUM] = {0};

_attribute_ble_data_retention_  u8	app_acl_masterTxfifo[ACL_TX_FIFO_SIZE * ACL_TX_FIFO_NUM * MASTER_MAX_NUM] = {0};

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/


/***HCI ACL buffer define***/
#define HCI_ACL_DATA_LEN      28
#define HCI_ACL_BUF_SIZE      (4 + HCI_ACL_DATA_LEN)
#define HCI_ACL_BUF_NUM       8
_attribute_ble_data_retention_ u8 bleHciAclBuf[HCI_ACL_BUF_SIZE * HCI_ACL_BUF_NUM];




/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, Begin ********************************/

#define	MTU_M_BUFF_SIZE_MAX			ATT_ALLIGN4_DMA_BUFF(23)

/** if support LE Secure Connections, L2CAP buffer must >= 76.[65+10,align 4bytes] */
#define MTU_S_BUFF_SIZE_MAX			ATT_ALLIGN4_DMA_BUFF(23)


_attribute_ble_data_retention_	u8 mtu_s_rx_fifo[SLAVE_MAX_NUM * MTU_S_BUFF_SIZE_MAX];
_attribute_ble_data_retention_	u8 mtu_s_tx_fifo[SLAVE_MAX_NUM * MTU_S_BUFF_SIZE_MAX];
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, End **********************************/




#if 0
/**
 * @brief	BLE Advertising data
 */
const u8	tbl_advData[] = {
	 9,  DT_COMPLETE_LOCAL_NAME, 				'T','L','K','W','A','T','C','H',
	 2,	 DT_FLAGS, 								0x05, 					// BLE limited discoverable mode and BR/EDR not supported
	 3,  DT_APPEARANCE, 						0x80, 0x01, 			// 384, Generic Remote Control, Generic category
	 5,  DT_INCOMPLT_LIST_16BIT_SERVICE_UUID,	0x12, 0x18, 0x0F, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
};


/**
 * @brief	BLE Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
	 9,  DT_COMPLETE_LOCAL_NAME, 				'T','L','K','W','A','T','C','H',
};
#endif


/**
 * @brief      BLE Adv report event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_adv_report_event_handle(u8 *p)
{
	return 0;
}



//data
#define APP_CONNECT_COMPLETE_EVT_DELAY_US   150000
#define APP_EVT_MAX_NUM    4
#define APP_EVT_MAX_SIZE   36 //(8 + 5 + 65)
my_fifo_t app_ble_txfifo;
//MYFIFO_INIT(app_ble_txfifo, APP_EVT_MAX_SIZE, APP_EVT_MAX_NUM);

//flag
struct APP_BLE_EVT_C_T {
   bool  Discon_flag;	//Mark disconnect timeout
   bool  write_flag;     //After writing, clean it manually  (true and false)
   u32   tick;           //Record the current time
   u32   delay_us;       //set Size of time delay value
   u8    p_buf[APP_EVT_MAX_NUM*APP_EVT_MAX_SIZE];
} app_ble_evt_c;        //Control related data and marks


int app_controller_event_cback(u32 h, u8 *p, int n)
{
	int re_val = 0;

	if(h &HCI_FLAG_EVENT_BT_STD)
	{
		u8 evtCode = h & 0xff;
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			if (HCI_ERR_CONN_TIMEOUT == p[3]) {
				app_ble_evt_c.Discon_flag = true;
				my_dump_str_data(BLE_ACL_CONN_DBG_EN, "#timeout 0x08", NULL, 0);
			}
			else if (HCI_ERR_CONN_FAILED_TO_ESTABLISH == p[3]) {
				my_dump_str_data(BLE_ACL_CONN_DBG_EN, "#connect fail 0x3E#", NULL, 0);
			}
			if (app_ble_evt_c.tick > 0) {
				app_ble_evt_c.write_flag = true;
			}
		}
		/* LE Event Handle */
		else if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvtCode = p[0];
			switch(subEvtCode)
			{
			case HCI_SUB_EVT_LE_ADVERTISING_REPORT:
			{
				break;
			}
			case HCI_SUB_EVT_LE_CONNECTION_COMPLETE:
			{
				if (app_ble_evt_c.Discon_flag == true) {
					app_ble_evt_c.Discon_flag = false;// clear flag
					app_ble_evt_c.write_flag = true;
					my_dump_str_data(BLE_ACL_CONN_DBG_EN, "#connect start#", NULL, 0);
					app_ble_evt_c.tick = clock_time() | 1;
				}
			}

			default:
				break;
			}
		}
	}

	if (app_ble_evt_c.write_flag == true)
	{
		u8 *app_p = my_fifo_wptr (&app_ble_txfifo);
		*app_p++ = h;
		*app_p++ = h >> 8;
		*app_p++ = h >> 16;
		*app_p++ = h >> 24;
		*app_p++ = n;
		*app_p++ = n >> 8;
		*app_p++ = n >> 16;
		*app_p++ = n >> 24;
		 smemcpy (app_p, p, n);
		 app_p += n;
		 my_dump_str_data(BLE_ACL_CONN_DBG_EN, "#start h = #", app_ble_txfifo.p, 128);
		 app_ble_txfifo.wptr++;
		 app_ble_evt_c.write_flag = false;
		 re_val = 0;
	}
	else
	{
		re_val = blc_hci_send_data(h, p, n);
	}

	return re_val;
}


/**
 * @brief		user initialization for BLE mode when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void app_ble_init(void)
{

//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

	/* for 1M Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M Flash, flash_sector_mac_address equals to 0x1FF000 */
	u8  mac_public[6] = LOCAL_BD_ADDR;//{0x66,0x66,0x66,0x66,0x66,0x66};
	//u8  mac_random_static[6];
	//blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);
	//bt_ll_get_bd_addr(mac_public);

	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);		//mandatory

	blc_ll_initLegacyAdvertising_module();		//mandatory for BLE slave

	blc_ll_initLegacyScanning_module(); 		//scan module: 		 mandatory for BLE master

	blc_ll_initLegacyInitiating_module();		//initiate module: 	 mandatory for BLE master

	blc_ll_initAclConnection_module();			//mandatory for BLE slave & master

	blc_ll_initAclSlaveRole_module();			//mandatory for BLE slave

	blc_ll_initAclMasterRole_module();			//mandatory for BLE master

	//blc_ll_init2MPhyCodedPhy_feature();		//This feature is not supported by default

	blc_ll_disableDefaultInitTimeout(); //disable default initiator timeout

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);

	/* ACL Slave TX FIFO */
	blc_ll_initAclConnSlaveTxFifo(app_acl_slaveTxfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM, SLAVE_MAX_NUM);

	/* ACL Master TX FIFO */
	blc_ll_initAclConnMasterTxFifo(app_acl_masterTxfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM, MASTER_MAX_NUM);

	/* HCI ACL buffer config. */
	blc_ll_initHciAclDataFifo(bleHciAclBuf, HCI_ACL_BUF_SIZE, HCI_ACL_BUF_NUM);


	#if( (MASTER_MAX_NUM + SLAVE_MAX_NUM ) > MAX_BLE_LINK)
		#error MAX BLE lINK not enough
	#endif

	blc_ll_setMaxConnectionNumber( MASTER_MAX_NUM, SLAVE_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_MASTER_MAX_TX_OCTETS, ACL_SLAVE_MAX_TX_OCTETS);

	blc_ll_setAclMasterConnectionInterval(CONN_INTERVAL_7P5MS);
	//////////// Controller Initialization  End /////////////////////////

	//////////// HCI Initialization  Begin /////////////////////////
	blc_hci_registerControllerDataHandler (blc_hci_sendACLData2Host);
	//blc_hci_registerControllerEventHandler(blc_hci_send_data);
	blc_hci_registerControllerEventHandler(app_controller_event_cback); //controller hci event to host all processed in this func

	/* bluetooth event */
	blc_hci_setEventMask_cmd (0xffffffff);

	/* bluetooth low energy(LE) event */
	blc_hci_le_setEventMask_cmd(0xffffffff);  //connection establish: telink private event   TODO: delete this cause we will support standard HCI
	//////////// HCI Initialization  End /////////////////////////

	u8 check_status = blc_controller_check_appBufferInitialization();
	if(check_status != BLE_SUCCESS){
		/* here user should set some log to know which application buffer incorrect*/
		write_log32(0x88880000 | check_status);
		my_dump_str_data(1, "application buffer error", &check_status, 1);
		while(1);;
	}
//////////////////////////// BLE stack Initialization  End //////////////////////////////////


//////////////////////////// ADV DEBUG ////////////////////////////
#if 0
	blc_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	blc_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));
	//TODO: debug channel 37 change to channel all
	blc_ll_setAdvParam(ADV_INTERVAL_50MS, ADV_INTERVAL_50MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_37, ADV_FP_NONE);

	blc_ll_setAdvEnable(BLC_ADV_ENABLE);

#endif

////////////////////////////////PAGE DEBUG//////////////////////////////////////////////
#if 0

	blc_ll_setScanParameter(SCAN_TYPE_ACTIVE, SCAN_INTERVAL_100MS, SCAN_WINDOW_30MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);

	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

#endif
}


void ble_init(void)
{
	rf_drv_ble_init();
	app_ble_init();

	app_ble_evt_c.Discon_flag = false;
	app_ble_evt_c.tick = 0;
	app_ble_evt_c.delay_us = APP_CONNECT_COMPLETE_EVT_DELAY_US;
	app_ble_evt_c.write_flag = false;
	my_fifo_init (&app_ble_txfifo, APP_EVT_MAX_SIZE, APP_EVT_MAX_NUM, app_ble_evt_c.p_buf);
}

void app_ble_main_loop(void)
{
	if (app_ble_evt_c.tick	&& clock_time_exceed(app_ble_evt_c.tick, app_ble_evt_c.delay_us))
	{
		app_ble_evt_c.tick = 0;
        u8 num;
        u32 h,n;
        num = 0;
		while (app_ble_txfifo.rptr != app_ble_txfifo.wptr)
		{
			// process HCI_RX/UART_TX fifo
			uint8_t *main_p = app_ble_txfifo.p + (app_ble_txfifo.rptr & (app_ble_txfifo.num-1)) * app_ble_txfifo.size;
			h = main_p[0]|(main_p[1]<<8)|(main_p[2]<<16)|(main_p[3]<<24);
			n = main_p[4]|(main_p[5]<<8)|(main_p[6]<<16)|(main_p[7]<<24);

			blc_hci_send_data(h, &main_p[8],n);

			app_ble_txfifo.rptr++;

			num++;
			if(num >= APP_EVT_MAX_NUM)
			{
				my_dump_str_data(BLE_ACL_CONN_DBG_EN, "#main loop overflow#", NULL, 0);
				break;
			}
		}
	}

    ble_sdk_main_loop();//sdk mainloop
}

