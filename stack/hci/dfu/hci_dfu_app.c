/********************************************************************************************************
 * @file     hci_dfu_app.c
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

#include "hci_dfu_app.h"

#include "drivers.h"

#include "stack.h"
enum{
	START_TO,
	WAITE_TO,
	END_TO,
	MAX_NUM_DFU_TO,
};
int DFU_Wait_Data_To_Cb(void *arg);
int DFU_TaskStart_Cb(void *arg);
struct ev_timer_event *dfu_timer_evts[MAX_NUM_DFU_TO];

/* Variable declare ***********************************************************/
DfuCb_t dfuCb;
#define DFU_MAX_LENGTH 4*1024
u8  dfu_pending_buf[DFU_MAX_LENGTH];

void HCI_VS_StartDfuCmdHandler(u8 *pParam, u32 len)
{
	u8 *p = pParam;
	u16 opcode  = HCI_OPCODE_VS_START_DFU;
	u8  dfuMode = 0;
	u16 fwVer   = 0;
	u32 fwSize  = 0;
	u8 status = 0;
	BSTREAM_TO_UINT8(dfuMode, p);
	BSTREAM_TO_UINT16(fwVer,  p);
	BSTREAM_TO_UINT32(fwSize, p);

	if(dfuCb.status == DFU_STA_START){

	    status = HCI_ERR_DFU_ENABLEED;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		return;
	}
	//cheack pass
	DFU_Init();
	//
	if(fwSize > dfuCb.maxFwSize){
		DFU_Reset();
		DFU_TRACK_INFO("maxsize_overflow\n");

		status = HCI_ERR_INVALID_PARAM;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		return;
	}
	switch(dfuMode)
	{

	case DFU_MODE_FW_UPD:
	{
		goto DFU_START;
	}
	case DFU_MODE_FW_UPD_NEWER:
	{
		u16 localVer = HCI_REVISION;
		if(fwVer <= localVer){

			status = HCI_ERR_FW_VER;

			hci_send_command_complete_event(1, &opcode, &status, 1);
		}else{
			goto DFU_START;
		}
		break;
	}
	case DFU_MODE_FW_UPD_OLDER:
	{
		u16 localVer = HCI_REVISION;
		if(fwVer >= localVer){

			status = HCI_ERR_FW_VER;

			hci_send_command_complete_event(1, &opcode, &status, 1);
		}else{
			goto DFU_START;
		}
		break;
	}
	default:

		status = HCI_ERR_INVALID_PARAM;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		break;
	}
	return;//[!!!important]

DFU_START:
	DFU_Reset();
	dfuCb.status    = DFU_STA_START;
	dfuCb.newFwSize = fwSize;
	dfuCb.timer     = clock_time()|1;//start timer

	u8 param[3] = {0};
	param[0] = HCI_SUCCESS; //status
	param[1] = DFU_MAX_PAYLOAD&0xff; //max payload length. TODO: according to HCI ACL buffer size
	param[2] = (DFU_MAX_PAYLOAD>>8)&0xff;

	hci_send_command_complete_event(1, &opcode, param, sizeof(param));

	dfu_timer_evts[START_TO] = ev_on_timer(DFU_Wait_Data_To_Cb, 0, DFU_WAIT_DATA_TO*1000, &bt_controller_timer_pending_list);
}

void HCI_VS_EndDfuCmdHandler(u8 *pParam, u32 len)
{
	u8 *p = pParam;
	u16 opcode  = HCI_OPCODE_VS_END_DFU;
	u8  endMode = 0;
	u8 status = 0;
	BSTREAM_TO_UINT8(endMode, p);

	dfuCb.timer = clock_time()|1;//reset timer

	switch(endMode)
	{
	case DFU_END_MODE_FW_UPD:

		 if(dfuCb.w2flash_len){
			//Write data to flash.
			flash_write_page(dfuCb.nextFwAddrStart+dfuCb.fwOffset, dfuCb.w2flash_len ,dfu_pending_buf );
			dfuCb.fwOffset += dfuCb.w2flash_len;
			dfuCb.w2flash_len = 0;
			my_dump_str_u32s(1, "FW_END", dfuCb.fwOffset, dfuCb.newFwSize, dfuCb.w2flash_len, 0);
			//last write end!
			}
			if(dfuCb.fwOffset != dfuCb.newFwSize){
			    status = HCI_ERR_FW_INCOMPLETE;
				hci_send_command_complete_event(1, &opcode, &status, 1);
				dfuCb.status = DFU_STA_TERM;
			}
			else{

			u8 bootFlag = 0x4b;
			u8 buf[256];
			u32 baswAddr = dfuCb.nextFwAddrStart;

			flash_read_page(baswAddr, sizeof(buf), buf);
			buf[BOOT_FLAG_OFFSET] = bootFlag;
			flash_erase_page(baswAddr);

			flash_write_page(baswAddr, sizeof(buf) ,buf );

			bootFlag = 0x00;
			baswAddr = dfuCb.nextFwAddrStart==0 ? dfuCb.newFwAddrStart:0;
			flash_read_page(baswAddr, sizeof(buf), buf);
			buf[BOOT_FLAG_OFFSET] = bootFlag;
			flash_erase_page(baswAddr);

			flash_write_page(baswAddr, sizeof(buf), buf);
			my_dump_str_u32s(1, "BOOT_ON", dfuCb.nextFwAddrStart, baswAddr, 0, 0);
			status = HCI_SUCCESS;
			hci_send_command_complete_event(1, &opcode, &status, 1);

			dfuCb.status = DFU_STA_END;

		}

		dfu_timer_evts[START_TO] = ev_on_timer(DFU_TaskStart_Cb, 0, DFU_ST_END_TER_TO*1000, &bt_controller_timer_pending_list);
		break;

	case DFU_END_MODE_TERM_UPD:
		dfuCb.status = DFU_STA_TERM;
		status = HCI_SUCCESS;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		dfu_timer_evts[START_TO] = ev_on_timer(DFU_TaskStart_Cb, 0, DFU_ST_END_TER_TO*1000, &bt_controller_timer_pending_list);
		DFU_TRACK_INFO("DFU_Term...\n");
		break;

	default:
		status = HCI_ERR_INVALID_PARAM;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		break;
	}
}

void HCI_VS_FwDataCmdHandler(u8 *pParam, u32 len)
{
	u16 opcode  = HCI_OPCODE_VS_FW_DATA;
	u16 dataLen = len - 4;//4=DFU_CHECKSUM_LEN
	u32 checkSum = 0;
	u8 status;
	BYTES_TO_UINT32(checkSum, pParam + dataLen);

	ev_on_timer(DFU_Wait_Data_To_Cb, 0, DFU_WAIT_DATA_TO*1000, &bt_controller_timer_pending_list);
	//TODO:
	/*! DFU re-send number. */
	if(dfuCb.resendCnt >= 3){
		return;
	}

	dfuCb.timer = clock_time()|1;//reset timer

	/* Check if DFU is started. */
	if(dfuCb.status != DFU_STA_START){

		status = HCI_ERR_DFU_DISABLED;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		return;
	}

	/* Check if payload length is valid. */
	if(dataLen > DFU_MAX_PAYLOAD){

		status = HCI_ERR_DATA_LENGTH;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		return;
	}
	gpio_set_high_level(GPIO_PC5);
	u32 rx_checkSum = DFU_Crc32Calc(0xFFFFFFFF, pParam, dataLen);//0xFFFFFFFF = DFU_CRC_INIT_VALUE
	gpio_set_high_level(GPIO_PC5);
	/* Check if CRC is correct. */
	if(rx_checkSum != checkSum){

		status = HCI_ERR_FW_CHECKSUM;

		hci_send_command_complete_event(1, &opcode, &status, 1);

		my_dump_str_u32s(1, "FW_CHECKSUM,err", checkSum, rx_checkSum, dataLen, 0);
	}
	else{

		dfuCb.rxCnt++;

		dfuCb.resendCnt = 0;

		if(dfuCb.fwOffset < dfuCb.newFwSize)
		{
			u8 bootFlag = 0;
			if(dfuCb.fwOffset == 0 && (dfuCb.w2flash_len == 0)){//TODO: max_payload must >= 8 Bytes
				bootFlag = pParam[BOOT_FLAG_OFFSET];
				pParam[BOOT_FLAG_OFFSET] = 0xff;
			}

			u32 buf_len = dfuCb.w2flash_len + dataLen;

			if(buf_len>DFU_MAX_LENGTH){
			   //*Write data to flash
			   flash_write_page(dfuCb.nextFwAddrStart+dfuCb.fwOffset , dfuCb.w2flash_len,dfu_pending_buf);

			   dfuCb.fwOffset += dfuCb.w2flash_len;
			   dfuCb.w2flash_len = 0;
			   //last write end!

			   GLOBAL_INT_DISABLE();
			   tmemcpy(dfu_pending_buf+dfuCb.w2flash_len,pParam,dataLen);
			   GLOBAL_INT_RESTORE();
			   dfuCb.w2flash_len = dataLen;

			}
			else{
				GLOBAL_INT_DISABLE();
				tmemcpy(dfu_pending_buf+dfuCb.w2flash_len,pParam,dataLen);
				dfuCb.w2flash_len = buf_len;
				GLOBAL_INT_RESTORE();

			}

			status = HCI_SUCCESS;

			hci_send_command_complete_event(1, &opcode, &status, 1);
		}
		else{

			status = HCI_ERR_FW_INCOMPLETE;

			hci_send_command_complete_event(1, &opcode, &status, 1);

		}
	}
}

void HCI_VS_DFU_CmdHandler(u8 *pHciTrPkt, u32 len)
{

	u16 opcode;
	u16 paramLen;
    u8 status;
	u8 *pPkt = pHciTrPkt;
	pPkt++;//skip HCI type;

	BSTREAM_TO_UINT16(opcode, pPkt);

	switch(opcode)
	{
	case HCI_OPCODE_VS_START_DFU:
		DFU_TRACK_INFO("Rx HCI_Start_Dfu_Cmd...\n");
		BSTREAM_TO_UINT8(paramLen, pPkt);
		HCI_VS_StartDfuCmdHandler(pPkt, paramLen);
		break;

	case HCI_OPCODE_VS_FW_DATA:
		DFU_TRACK_INFO("Rx HCI_Fw_Data_Cmd...\n");
		BSTREAM_TO_UINT8(paramLen, pPkt);
		HCI_VS_FwDataCmdHandler(pPkt, paramLen);
		break;

	case HCI_OPCODE_VS_END_DFU:
		DFU_TRACK_INFO("Rx HCI_End_Dfu_Cmd...\n");
		BSTREAM_TO_UINT8(paramLen, pPkt);
		HCI_VS_EndDfuCmdHandler(pPkt, paramLen);
		break;


	default:

		status = CO_ERROR_COMMAND_DISALLOWED;

		hci_send_command_complete_event(1, &opcode, &status, 1);
		break;
	}
}

void DFU_EraseNewFwArea(void)
{
	u32 tmp1 = 0;
	u32 tmp2 = 0;
	u32 tmp3 = 0;

	flash_read_page(dfuCb.nextFwAddrStart,4, (u8*)&tmp1);
	flash_read_page(dfuCb.nextFwAddrStart + (dfuCb.maxFwSize/0x1000/2)*0x1000, 4,(u8*)&tmp2);
	flash_read_page(dfuCb.nextFwAddrStart + dfuCb.maxFwSize - 4, 4,(u8*)&tmp3);

	if(tmp1 != 0xFFFFFFFF || tmp2 != 0xFFFFFFFF || tmp3 != 0xFFFFFFFF)
	{
		for(int i=0; i<dfuCb.maxFwSize; i += 0x8000)
		{
			flash_erase_32kblock(dfuCb.nextFwAddrStart + i);
		}
	}
}

u8 DFU_isEnable(void)
{
	return (dfuCb.status == DFU_STA_START ? true:false);
}

void DFU_Reset(void)
{
	dfuCb.newFwSize = 0;
	dfuCb.fwOffset  = 0;
	dfuCb.curCrc    = 0xFFFFFFFF;// 0xFFFFFFFF = DFU_CRC_INIT_VALUE

	dfuCb.timer     = 0;
	dfuCb.status    = 0;
	dfuCb.resendCnt = 0;
}

void DFU_Init(void)
{
	memset((u8*)&dfuCb, 0, sizeof(DfuCb_t));

	dfuCb.newFwAddrStart = DFU_NEW_FW_ADDR_BASE;
	dfuCb.maxFwSize      = DFU_NEW_FW_MAX_SIZE;//400K - 4K (4K is invalid)

	dfuCb.nextFwAddrStart = DFU_NEW_FW_ADDR_BASE;
	u8 bootFlag = REG_ADDR8(DFU_FW_REBOOT_ADDR);
	if (bootFlag){
		dfuCb.nextFwAddrStart = 0;
	}

	DFU_EraseNewFwArea();
}

int DFU_TaskStart_Cb(void *arg)
{

	ev_unon_timer(dfu_timer_evts[START_TO],&bt_controller_timer_pending_list);
	// Wait for HCI_Cmd_Cmpl_Evt sent
	if(dfuCb.status == DFU_STA_END )
	{
		DFU_TRACK_INFO("DFU_END...\n");
		dfuCb.status = DFU_STA_NONE;
		start_reboot();
	}

	if(dfuCb.status == DFU_STA_TERM )
	{
		dfuCb.status = DFU_STA_NONE;
		DFU_TRACK_INFO("DFU_Term exec...\n");
		DFU_EraseNewFwArea();
		DFU_Reset();
	}
	return -1;
}
int DFU_Wait_Data_To_Cb(void *arg)
{
	DFU_TRACK_INFO("DFU_WAIT_TO...\n");
	DFU_EraseNewFwArea();
	DFU_Reset();

	return -1;
}

//x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x+ 1
//0x04C11DB7
u32 DFU_Crc32Calc(u32 crc, u8* data, u32 len)
{
    while(len--)
    {
        crc ^= *data++;

        for(u8 i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}


