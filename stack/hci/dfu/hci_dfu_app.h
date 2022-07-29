/********************************************************************************************************
 * @file     hci_dfu_app.h
 *
 * @brief    This is the header file for BTBLE SDK
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

#ifndef HCI_DFU_APP_H_
#define HCI_DFU_APP_H_

#include "common/types.h"


#define BYTES_TO_UINT32(n, p)     {n = ((u32)(p)[0] + ((u32)(p)[1] << 8) + \
                                       ((u32)(p)[2] << 16) + ((u32)(p)[3] << 24));}

#define BSTREAM_TO_UINT8(n, p)    {n = (uint8_t)(*(p)++);}

#define BSTREAM_TO_UINT16(n, p)   {n = ((uint16_t)(p)[0] + ((uint16_t)(p)[1]<<8)); p+=2;}

#define BSTREAM_TO_UINT32(n, p)   {n = ((uint32_t)(p)[0] + ((uint32_t)(p)[1]<<8) + ((uint32_t)(p)[2]<<16) + ((uint32_t)(p)[3]<<24)); p+=4;}


/*! Firmware reboot address. */
#define DFU_FW_REBOOT_ADDR         0x140104

/*! BootFlag. */
#define BOOT_FLAG_OFFSET         0X20

/*! Firmware version. */
#define DFU_FW_VERSION         0x0015

/*! New firmware start address. */
#define DFU_NEW_FW_ADDR_BASE   (0x40000)

/*! Max size of new firmware. */
#define DFU_NEW_FW_MAX_SIZE    (0x40000)//128K - 4K (4K is invalid)

/*! Max length of payload. */
#define DFU_MAX_PAYLOAD        0x280

/*! Firmware cheack enable. */
#define DFU_CHEAK_ENABLE        1

/*! HCI VS error code. */
enum{
	HCI_SUCCESS           = 0x00,
	HCI_ERR_FW_VER        = 0xA0,
	HCI_ERR_FW_SIZE       = 0xA1,
	HCI_ERR_DFU_DISABLED  = 0xA2,
	HCI_ERR_DFU_ENABLEED  = 0xA3,
	HCI_ERR_FW_CHECKSUM   = 0xA4,
	HCI_ERR_FW_CRC        = 0xA5,
	HCI_ERR_FW_INCOMPLETE = 0xA6,
	HCI_ERR_DATA_LENGTH   = 0xA7,
    HCI_ERR_INVALID_PARAM = 0xA8,
};

#define DFU_ST_END_TER_TO                10     // MS
#define DFU_WAIT_DATA_TO                 1*1000     // MS
void DFU_Init(void);
int DFU_TaskStart_cb(void *arg);
int DFU_WAIT_DATA_TO_cb(void *arg);

u8 DFU_isEnable(void);

/**
 * @Name: HCI Opcode define.
 */
/*! HCI VS Command opcode define. */
#define HCI_OPCODE_VS_START_DFU         0xff00//HCI_OPCODE(HCI_OGF_VS, HCI_OCF_VS_START_DFU)
#define HCI_OPCODE_VS_END_DFU           0xff01//HCI_OPCODE(HCI_OGF_VS, HCI_OCF_VS_END_DFU)
#define HCI_OPCODE_VS_FW_DATA           0xff02//HCI_OPCODE(HCI_OGF_VS, HCI_OCF_VS_FW_DATA)

//////////////////////////////////////////////////////

#define DFU_TRACK_INFO(format, ...) if(1){my_dump_str_data(1,format,0,0);}

//////////////////////////////////////////////////////



/*! DFU Mode define. */
enum{
	DFU_MODE_NONE           = 0x00,
	DFU_MODE_FW_UPD         = 0x01,
	DFU_MODE_FW_UPD_NEWER  =  0x02,
	DFU_MODE_FW_UPD_OLDER   = 0x03,
};

/*! DFU End mode define. */
enum{
	DFU_END_MODE_FW_UPD   = 0x00,
	DFU_END_MODE_TERM_UPD = 0x01,
};

/*! DFU Status define. */
enum{
	DFU_STA_NONE         = 0x00,
	DFU_STA_START        = 0x01,
	DFU_STA_END          = 0x02,
	DFU_STA_TERM         = 0x03,
};

/*! DFU structure define. */
typedef struct{
	u32  w2flash_len;
	u32  newFwAddrStart;
	u32  maxFwSize;
	u32  nextFwAddrStart;

	u32  newFwSize;
	u32  fwOffset;
    u32  curCrc;          /*<! use for overall firmware check. */

	u32  timer;           /*<! timeout timer. */
	u8   status;
	u8   resendCnt;       /*<! re-send counter. */
	u32  rxCnt;       /*<! re-send counter. */

}DfuCb_t;

u32 DFU_Crc32Calc(u32 crcInit, u8 *pdata, u32 len);

void DFU_Reset(void);
void DFU_EraseNewFwArea(void);
extern void HCI_VS_DFU_CmdHandler(u8 *pHciPkt, u32 len);


#endif /* UART_DFU_H_ */

