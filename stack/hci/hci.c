/********************************************************************************************************
 * @file     hci.c
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

#include "tl_common.h"
#include "drivers.h"
#include "hci.h"


_attribute_ble_data_retention_	hci_event_handler_t		hci_event_handler_cb = NULL;




void blc_hci_registerControllerEventHandler (hci_event_handler_t  handler)
{
	hci_event_handler_cb = handler;
}



int blc_hci_send_event (u32 h, u8 *para, int n)
{
	if (hci_event_handler_cb)
	{
		return hci_event_handler_cb (h, para, n);
	}
	return 1;
}
