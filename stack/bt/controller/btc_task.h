/********************************************************************************************************
 * @file     btc_task.h
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

#ifndef _BTC_TASK_H_
#define _BTC_TASK_H_

/*
 * task id
 */
enum{
	/* link setup */
	BTC_TASK_ID_CONNECTION_SETUP, //done
	BTC_TASK_ID_ROLE_SWITCH,//done
	BTC_TASK_ID_AFH_SETUP,
	BTC_TASK_ID_AFH_CLASSIFICATION,
	BTC_TASK_ID_QOS,
	BTC_TASK_ID_REM_NAME_REQ,
	BTC_TASK_ID_DETACH,//done
	/* secure link */
	BTC_TASK_ID_LEGACY_AUTHEN,//done
	BTC_TASK_ID_SECURE_AUTHEN,//done
	BTC_TASK_ID_LEGACY_PAIRING,//done
	BTC_TASK_ID_SECURE_SIMPLE_PAIRING,//done
	BTC_TASK_ID_ENCRYPTION,//done
	/* packet type */
	BTC_TASK_ID_PACKET_TYPE,
	/* sniff mode*/
	BTC_TASK_ID_SNIFF,
	/* sco mode*/
	BTC_TASK_ID_SCO,
	BTC_TASK_ID_MAX=32,/* all task id must be less than 32*/
};
/*
 * message id
 */

enum{
	BTC_MSG_ID_IDLE,
	BTC_MSG_ID_HCI,
	BTC_MSG_ID_LMP,
	BTC_MSG_ID_MID,
};


typedef int32_t (*btc_task_func_cb_t)(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

btc_task_func_cb_t btc_task_conn_setup_cb;

btc_task_func_cb_t btc_task_legacy_pair_cb;
btc_task_func_cb_t btc_task_ssp_cb;
btc_task_func_cb_t btc_task_legacy_authen_cb;
btc_task_func_cb_t btc_task_secure_authen_cb;
btc_task_func_cb_t btc_task_encrypt_decrypt_cb;
btc_task_func_cb_t btc_task_disconnect_cb;
btc_task_func_cb_t btc_task_qos_cb;

btc_task_func_cb_t btc_task_rem_name_req_cb;
btc_task_func_cb_t btc_task_packet_type_cb;


/* register call back for all tasks
 *
 * */

static inline void btc_task_conn_setup_register(btc_task_func_cb_t conn_setup_func)
{
	if(conn_setup_func)
	{
		btc_task_conn_setup_cb = conn_setup_func;
	}
}


static inline void btc_task_disconnect_register(btc_task_func_cb_t disconnect_func)
{
	if(disconnect_func)
	{
		btc_task_disconnect_cb = disconnect_func;
	}
}
static inline void btc_task_qos_register(btc_task_func_cb_t qos_func)
{
	if(qos_func)
	{
		btc_task_qos_cb = qos_func;
	}
}


static inline void btc_task_rem_name_req_register(btc_task_func_cb_t remote_name_func)
{
	if(remote_name_func)
	{
		btc_task_rem_name_req_cb = remote_name_func;
	}
}

static inline void btc_task_packet_type_register(btc_task_func_cb_t packet_type_func)
{
	if(packet_type_func)
	{
		btc_task_packet_type_cb = packet_type_func;
	}
}



/* unregister call back for all tasks
 *
 * */

static inline void btc_task_conn_setup_unregister(void)
{
	btc_task_conn_setup_cb = 0;
}


static inline void btc_task_disconnect_unregister(void)
{
	btc_task_disconnect_cb = 0;
}

static inline void btc_task_qos_unregister(void)
{
	btc_task_qos_cb = 0;
}


static inline void btc_task_rem_name_req_unregister(void)
{
	btc_task_rem_name_req_cb = 0;
}

static inline void btc_task_packet_type_unregister(void)
{
	btc_task_packet_type_cb = 0;
}


#define MSG_BUF_LEN   4

///LMP msg
typedef struct task_data {
    // Link identifier
    uint8_t  link_id;
    // Task identifier
    uint8_t  task_id;
    // Message identifier
    uint8_t  msg_id;
    // Message identifier
    uint8_t  rsvd;
    ///buffer of the LMP PDU
    uint8_t  msg_buf[MSG_BUF_LEN];
    // register callback
    btc_task_func_cb_t call_back;
} __attribute__ ((aligned (4))) task_data_t;

typedef struct env_task{
	uint32_t task_bit_map;

}__attribute__ ((aligned (4))) env_task_t;

env_task_t btc_env_task[MAX_NB_ACTIVE_ACL];

static inline uint8_t btc_task_is_pending(void)
{
	for(uint8_t link_id=0;link_id<MAX_NB_ACTIVE_ACL;link_id++)
	{
		if(btc_env_task[link_id].task_bit_map)
		{
			return 1;
		}
	}
	return 0;
}

static inline uint8_t btc_task_sleep_is_allowed(void)
{
	for(uint8_t link_id=0;link_id<MAX_NB_ACTIVE_ACL;link_id++)
	{
		if(btc_env_task[link_id].task_bit_map&(~((1<<BTC_TASK_ID_AFH_SETUP)|(1<<BTC_TASK_ID_AFH_CLASSIFICATION))))
		{
			return 1;
		}
	}
	return 0;
}

static inline uint8_t btc_task_is_busy(uint32_t task_id, bt_ll_acl_env_t *acl_par)
{
	return acl_par->task_state[task_id];
}

/* push message into the task buffer
 *
 * */
int32_t btc_task_push_message(uint8_t link_id, uint8_t task_id, uint8_t msg_id,uint8_t *p, uint8_t len, btc_task_func_cb_t callback);

/* clear task state and task bit map
 *
 * */
int32_t btc_task_clear(uint8_t link_id);

/* register task id into the task bit map
 *
 * */

int32_t btc_task_register(uint8_t link_id, uint8_t task_id,uint8_t task_state);

/* unregister task id into the task bit map
 *
 * */
int32_t btc_task_unregister(uint8_t link_id, uint8_t task_id);

typedef int32_t (*btc_task_mainloop_cb_t)(void);

/* define callback for main loop task
 *
 * */
btc_task_mainloop_cb_t btc_task_mainloop_cb;


/* process message from the task buffer
 *
 * */
int32_t btc_task_mainloop(void);

static inline void btc_task_mainloop_register(btc_task_mainloop_cb_t mainloop_func)
{
	if(mainloop_func)
	{
		btc_task_mainloop_cb = mainloop_func;
	}
}


#endif /* _BTC_TASK_H_ */
