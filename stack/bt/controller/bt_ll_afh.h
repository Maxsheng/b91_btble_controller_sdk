/********************************************************************************************************
 * @file     bt_ll_afh.h
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

#ifndef _BT_LL_AFH_H_
#define _BT_LL_AFH_H_

#include <stdint.h>
#include "bt_ll_lmp_map.h"


typedef struct BTC_AFH{

    uint32_t afh_instant;
    uint32_t afh_refresh_ts;
    uint32_t afh_refresh_to;
    /// AFH reporting interval (in slots)
    uint32_t reporting_interval;
    uint32_t reporting_ts;

    /// Temporary storage of new AFH maps, 10byte
    struct chnl_map afh_map;
    uint8_t afh_step;
    uint8_t afh_en;

    /// AFH channel classification - it will be sent to master as slave
    struct chnl_map ch_class;
    /// AFH reporting enable
    uint8_t reporting_en;
    uint8_t reporting_en_bkup;

    /// Channel classification from peers
    struct chnl_map peer_ch_class;

}btc_afh_t;

extern btc_afh_t* env_afh_ptr;

static inline btc_afh_t * btc_afh_get_env(uint8_t link_id)
{
	return (btc_afh_t*)(env_afh_ptr? (env_afh_ptr+link_id):0);
}

static inline void btc_afh_env_init(btc_afh_t * env_afh)
{
	if(env_afh)
	{
		env_afh_ptr = env_afh;
	}
}


enum{
	AFH_MAINLOOP_TASK_ID_IDLE,
	AFH_MAINLOOP_TASK_ID_START_AFH,
	AFH_MAINLOOP_TASK_ID_HCI_RD_AFH_CH_MAP,
	AFH_MAINLOOP_TASK_ID_HCI_WR_AFH_CH_ASSESS_MODE,
	AFH_MAINLOOP_TASK_ID_HCI_RD_AFH_CH_ASSESS_MODE,
	AFH_MAINLOOP_TASK_ID_HCI_SET_AFH_HOST_CH_CLASS,
	AFH_MAINLOOP_TASK_ID_DISABLE_CLS,
	AFH_MAINLOOP_TASK_ID_RESTORE_CLS,
	AFH_MAINLOOP_TASK_ID_CLOSE_AFH,
	AFH_MAINLOOP_TASK_ID_WAIT_BB_ACK,
	AFH_MAINLOOP_TASK_ID_LMP_SET_AFH,
};


enum{
	AFH_IRQ_TASK_ID_IDLE,
	AFH_IRQ_TASK_ID_AFH_INIT,
	AFH_IRQ_TASK_ID_REFRESH_LEVEL,
};

typedef int32_t (*btc_afh_task_callback_t)(uint8_t link_id,uint8_t* p,uint8_t len);

btc_afh_task_callback_t btc_afh_mainloop_task_callback;
btc_afh_task_callback_t btc_afh_irq_task_callback;
void btc_afh_register_module(btc_afh_t* env_afh);


#endif /* _BT_LL_AFH_H_ */
