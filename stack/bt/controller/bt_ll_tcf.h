/********************************************************************************************************
 * @file     bt_ll_tcf.h
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

#ifndef _BT_LL_TCF_H_
#define _BT_LL_TCF_H_

typedef struct bt_ll_tick_clkn_fcnt {
    uint32_t tick; 
    uint32_t clkn; //unit: half slot, i.e. 312.5 us
    uint32_t fcnt; //unit: half us
} bt_ll_tick_clkn_fcnt_t;

_attribute_ram_code_ uint32_t bt_hw_get_clkn_fcnt(uint16_t *fcnt);

_attribute_ram_code_ uint32_t bt_update_clkn_fcnt(bt_ll_tick_clkn_fcnt_t *p, uint32_t t);

_attribute_ram_code_ void bt_hw_update_clkn_fcnt(bt_ll_tick_clkn_fcnt_t *p, int reset);

_attribute_bt_retention_code_ uint8_t bt_hw_reset_bt_core(void);

_attribute_ram_code_ uint32_t bt_hw_set_next_fcnt_irq(bt_ll_tick_clkn_fcnt_t *p, uint16_t fcnt);

_attribute_ram_code_ uint32_t bt_hw_get_tick_from_clkn_fcnt(bt_ll_tick_clkn_fcnt_t *p, uint32_t clkn, uint16_t fcnt);

#endif /* _BT_LL_TCF_H_ */
