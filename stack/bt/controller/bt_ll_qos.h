/********************************************************************************************************
 * @file     bt_ll_qos.h
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

#ifndef _BT_LL_QOS_H_
#define _BT_LL_QOS_H_

typedef enum {
    /// idle state
    ST_QOS_SETUP_IDLE = 0x00,
    ST_QOS_SETUP_START,
    ST_QOS_SETUP_WAIT_ACCEPT,
    ST_QOS_SETUP_DONE,
} LMP_QOS_SETUP_STATE;

#define QOS_HL_REQ                  0
#define QOS_PEER_REQ                1
#define QOS_PEER_FORCE              2
#define QOS_PEER_REJECT             3



int  lmp_send_cmd_qos_req(int Lid, unsigned short poll_interval, unsigned char nbc);



/**
 ****************************************************************************************
 * @brief This function allows to start the QoS setup procedure through either HCI 
 *        Command or being invoked directly by host code in SoC architecture.
 * 
 * @param[in] con_hdl    indicates the acl connection needs to perform QoS setup
 *                       
 * @param[in] srv_type   0x00: No Traffic, 0x01: Best Effort, 0x02: Guaranteed
 *
 * @param[in] tkn_rate   Token Rate in octets per second.
 *
 * @param[in] pk_bw      Peak Bandwidth in octets per second.
 *
 * @param[in] latency    Latency in microseconds.
 *
 * @param[in] dly_var    Delay Variation in microseconds.
 *
 * @return CO_ERROR_NO_ERROR if the lmp procedure is started successfully 
 *         otherwise an errorcode is returned.      
 *
 ****************************************************************************************
 */
uint8_t bt_ll_qos_setup(uint16_t con_hdl, uint8_t srv_type, uint32_t tkn_rate, uint32_t pk_bw, uint32_t latency, uint32_t dly_var);

/**
 ****************************************************************************************
 * @brief This function is the state machine function of the QoS setup procedure.
 * 
 * @param[in] p_acl      pointer to the acl connection needs to perform QoS setup
 *                       
 *
 * @param[in] Lid        indicates the acl connection needs to perform QoS setup
 *
 * @param[in] task_id    the id of the role switch task
 *
 * @return the msg type the state machine picks from the msg queue is used by the msg
 *         clean function after the msg is processed.    
 *
 ****************************************************************************************
 */

int32_t btc_task_qos_mainloop(uint8_t link_id,uint8_t task_id,uint8_t msg_id,uint8_t *msg_buf);

int bt_ll_task_lmp_qos_setup(int Lid, int task_id);

uint8_t bt_ll_GetQoSParam(uint8_t Role,            uint8_t *ServiceType,
                       uint32_t *TokenRate,      uint32_t *PeakBandwidth, uint32_t *Latency,
                       uint32_t *DelayVariation, uint16_t *PollInterval,  uint16_t PacketType, uint8_t Request);
uint8_t bt_ll_write_link_supvision_to(uint16_t con_hdl,uint16_t link_supv_to);

#endif /* _BT_LL_QOS_H_ */
