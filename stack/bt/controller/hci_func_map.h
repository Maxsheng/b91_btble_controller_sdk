/********************************************************************************************************
 * @file     hci_func_map.h
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

#ifndef STACK_BT_CONTROLLER_HCI_FUNC_MAP_H_
#define STACK_BT_CONTROLLER_HCI_FUNC_MAP_H_

#include <stdint.h>
#include "fifo.h"
#include "uart.h"


typedef 	unsigned char (*le_hci_cmd_callback_t)(unsigned short, unsigned char *, int len);
_attribute_data_retention_	le_hci_cmd_callback_t 	ble_hci_cmd_cb;
void ble_register_hci_cmd_callback(le_hci_cmd_callback_t cb);

typedef int (*hci_cmd_vendor_spec_callback_t)(void *);
/**
 * @brief		register vs hci cmd.
 * @param[in]	vs callback
 * @return      register result
 */
extern void hci_vendor_spec_register_hci_cmd_callback(hci_cmd_vendor_spec_callback_t cb);
/**
 * @brief		register vs hci cmd.
 * @param[in]	vs callback ,opcode num
 * @return      register result
 */
extern  int hci_vendor_spec_register_cb_port(hci_cmd_vendor_spec_callback_t cb,u8 vs_opcode_num);


void hci_send_inquiry_complete_event(uint8_t status);
void hci_send_rd_clock_offset_complete_event(uint8_t status, uint16_t connection_handle,
        uint8_t *p, uint8_t len);
void hci_send_rd_rem_verinfo_complete_event(uint8_t status, uint16_t connection_handle,
        uint8_t *p, uint8_t len);
void hci_send_connection_complete_event(uint8_t status, uint16_t connection_handle, uint8_t *bd_addr,
		                                uint8_t link_type, uint8_t encryption_enabled);

void hci_connection_packet_type_changed_event(uint8_t status, uint16_t con_hdl,uint16_t Packet_Type );
_attribute_ram_code_
void hci_send_num_of_commplete_event(uint8_t num_handle, uint16_t connection_handle,uint16_t num_complete_pkt);
void hci_send_flush_occurred_event(uint16_t connection_handle);
void hci_send_command_complete_event(uint8_t num_hci_cmd, uint8_t* op_code, uint8_t *ret_para, uint8_t ret_para_len);
void hci_send_qos_setup_complete_event(uint8_t status, uint16_t con_hdl, uint8_t srv_type, unsigned int tkn_rate, unsigned int pk_bw, unsigned int latency, unsigned int dly_var);
_attribute_retention_code_ void hci_send_acl_establish_event(uint8_t status, uint16_t con_hdl, uint8_t *bd_addr, uint8_t role, uint8_t *remote_cod);
void hci_max_slots_change_event(uint16_t con_hdl,uint8_t max_slots);
void hci_send_connection_request_event(uint8_t *bd_addr,uint8_t*class_of_device,uint8_t link_type);
void hci_send_command_status_event(uint8_t status, uint8_t num_hci_cmd, uint8_t* op_code);
void hci_send_link_supervision_timeout_change_event(uint16_t connection_handle, uint16_t ls_to);
void hci_send_rd_rem_ext_features_complete_event(uint8_t status, uint16_t connection_handle,
        uint8_t *p, uint8_t len);
void hci_send_rd_rem_features_complete_event(uint8_t status, uint16_t connection_handle,
        uint8_t *p, uint8_t len)  ;
void hci_send_rd_rem_host_sup_features_notification_event(uint8_t status, uint8_t *bd_addr,
        uint8_t *ext_feature);
void hci_send_role_change_event(uint8_t status, uint8_t *bd_addr, uint8_t new_role);
void hci_send_sync_conn_changed_event(uint8_t status,
                                     uint16_t connection_handle,
                                     uint8_t transmission_interval,
                                     uint8_t retransmission_window,
                                     uint8_t rx_packet_length,
                                     uint8_t tx_packet_length,
									 uint8_t sync_linkid);

void hci_send_sync_conn_complete_event(uint8_t status,
                                      uint16_t connection_handle,
                                      uint8_t *bd_addr,
                                      uint8_t link_type,
                                      uint8_t transmission_interval,
                                      uint8_t retransmission_window,
                                      uint16_t rx_packet_length,
                                      uint16_t tx_packet_length,
                                      uint8_t air_mode);
/**
 * @brief		hardware error.
 * @param[in]	hardware code
 * @return      none
 */
extern void hci_send_hardware_error_event(uint8_t error_code);


#endif /* STACK_BT_CONTROLLER_HCI_FUNC_MAP_H_ */
