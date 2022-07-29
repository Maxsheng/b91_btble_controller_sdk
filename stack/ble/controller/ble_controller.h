/********************************************************************************************************
 * @file     ble_controller.h
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

#ifndef BLE_CONTROLLER_H_
#define BLE_CONTROLLER_H_



//#include "stack/btble_ll/btble_scheduler.h"
#include "stack/btble/btble.h"
#include "stack/btble/btble_pm.h"

#include "stack/ble/ble_config.h"
#include "stack/ble/ble_common.h"
#include "stack/ble/ble_format.h"


#include "stack/hci/hci.h"
#include "stack/hci/hci_const.h"
#include "stack/hci/hci_cmd.h"
#include "stack/hci/hci_event.h"
#include "stack/ble/hci/blehci.h"
#include "stack/ble/hci/blehci_event.h"


#include "stack/ble/controller/ll/ll.h"
#include "stack/ble/controller/ll/ll_pm.h"

#include "stack/ble/controller/ll/acl_conn/acl_conn.h"
#include "stack/ble/controller/ll/acl_conn/acl_slave.h"
#include "stack/ble/controller/ll/acl_conn/acl_master.h"


#include "stack/ble/controller/ll/adv/adv.h"
#include "stack/ble/controller/ll/adv/leg_adv.h"
#include "stack/ble/controller/ll/adv/ext_adv.h"

#include "stack/ble/controller/ll/scan/scan.h"
#include "stack/ble/controller/ll/scan/leg_scan.h"
#include "stack/ble/controller/ll/scan/ext_scan.h"


#include "stack/ble/controller/ll/init/init.h"
#include "stack/ble/controller/ll/init/leg_init.h"
#include "stack/ble/controller/ll/init/ext_init.h"




#include "stack/ble/controller/whitelist/whitelist.h"
#include "stack/ble/controller/whitelist/resolvlist.h"

#include "stack/ble/controller/csa/csa.h"

#include "stack/ble/controller/phy/phy.h"
#include "stack/ble/controller/phy/phy_test.h"


#include "algorithm/ecc/ecc_ll.h"
#include "algorithm/aes_ccm/aes_ccm.h"
#include "algorithm/crypto/crypto_alg.h"


/*********************************************************/
//Remove when file merge to SDK //
#include "stack/btble/btble_stack.h"

#include "stack/ble/ble_stack.h"
#include "stack/ble/debug.h"
#include "stack/ble/trace.h"

#include "stack/ble/controller/ll/ll_stack.h"
#include "stack/ble/controller/ll/acl_conn/acl_stack.h"
#include "stack/ble/controller/ll/adv/adv_stack.h"
#include "stack/ble/controller/ll/scan/scan_stack.h"
#include "stack/ble/controller/ll/init/init_stack.h"
#include "stack/ble/controller/whitelist/whitelist_stack.h"
#include "stack/ble/controller/csa/csa_stack.h"
#include "stack/ble/controller/phy/phy_stack.h"
#include "stack/ble/hci/hci_stack.h"
/*********************************************************/

#endif /* BLE_H_ */
