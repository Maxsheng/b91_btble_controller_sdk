/********************************************************************************************************
 * @file     bt_rf.c
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

#include "../drivers.h"
#include "../../common/string.h"


/**********************************************************************************************************************
 * RF FLAG BEGIN:
 *********************************************************************************************************************/
_attribute_data_retention_sec_ u32 rf_flag = RF_FLG_IQ_PWR_MANUAL_EN;

void rf_flag_set (unsigned int flag, unsigned int en)
{
	u32 r = core_disable_interrupt ();
	if (en)
	{
		rf_flag |= flag;
	}
	else
	{
		rf_flag &= ~flag;
	}
	core_restore_interrupt (r);
}

/**********************************************************************************************************************
 * RF MANUAL MODE IN PWR ADJUST SETTING BEGIN:
 *********************************************************************************************************************/

const rf_tx_pwr_t rf_pwr_ctrl_tp[10] = {

		[0].slice = BIT(7) | 2,//RF_TP_POWER_INDEX_N10p44dBm,
		[0].scale = 00,

		[1].slice = BIT(7) | 4,//RF_TP_POWER_INDEX_N7p44dBm,
		[1].scale = 00,

		[2].slice = BIT(7) | 6,//RF_TP_POWER_INDEX_N4p77dBm,
		[2].scale = 00,

		[3].slice = BIT(7) | 9,//RF_TP_POWER_INDEX_N1p37dBm
		[3].scale = 00,

		[4].slice = BIT(7) | 13,//RF_TP_POWER_INDEX_P2p72dBm,
		[4].scale = 00,

		[5].slice = 20,     //RF_TP_POWER_INDEX_P5P0_dBm,
		[5].scale = 00,

		[6].slice = 63,     //RF_TP_POWER_INDEX_P9P0_dBm,
		[6].scale = 00,
};

const rf_tx_pwr_t rf_pwr_ctrl_iq[10] = {


	    [0].slice = 23,  //RF_IQ_POWER_INDEX_N10P0_dBm,
	    [0].scale = 8,

	    [1].slice = 37,  //RF_IQ_POWER_INDEX_N7P04_dBm,
	    [1].scale = 8,

	    [2].slice = 56,  //RF_IQ_POWER_INDEX_N4P09_dBm,
	    [2].scale = 8,

	    [3].slice = 59,  //RF_IQ_POWER_INDEX_N1P05_dBm,
	    [3].scale = 11,

	    [4].slice = 63,  //RF_IQ_POWER_INDEX_P1P50_dBm,
	    [4].scale = 16,

	    [5].slice = 59,  //RF_IQ_POWER_INDEX_P1P50_dBm,
	    [5].scale = 16,

	    [6].slice = 63,  //RF_IQ_POWER_INDEX_P1P50_dBm,
	    [6].scale = 16,

};

/**********************************************************************************************************************
 * RF PWR NO ADJUST SETTING BEGIN:
 *********************************************************************************************************************/

_attribute_data_retention_sec_ u8 rf_tx_iq_scale = 16;
_attribute_data_retention_sec_ u8 rf_tx_iq_slice = 63;
_attribute_data_retention_sec_ u8 rf_tx_tp_slice = (BIT(7) | 26);


const rf_tx_pwr_t rf_pwr_iq[20] = {

	    [0].slice = 23,  //
	    [0].scale = 8,

	    [1].slice = 26,
	    [1].scale = 8,

	    [2].slice = 31,
	    [2].scale = 8,

	    [3].slice = 37,
	    [3].scale = 8,

	    [4].slice = 42,
	    [4].scale = 8,

	    [5].slice = 49,
	    [5].scale = 8,

	    [6].slice = 56,
	    [6].scale = 8,

	    [7].slice = 56,
	    [7].scale = 9,

	    [8].slice = 57,
	    [8].scale = 10,

	    [9].slice = 59,
	    [9].scale = 11,

	    [10].slice = 63,
	    [10].scale = 12,

	    [11].slice = 60,
	    [11].scale = 14,

	    [12].slice = 63,
	    [12].scale = 16,

		/**  For external PA (gain= +26dbm)**/

	    [14].slice = 20,
	    [14].scale = 4,

	    [15].slice = 20,
	    [15].scale = 5,

	    [16].slice = 20,
	    [16].scale = 6,

	    [17].slice = 20,
	    [17].scale = 7,

	    [18].slice = 20,
	    [18].scale = 8,
};

const rf_tx_pwr_t rf_pwr_tp[13] = {

		[0].slice = BIT(7) | 6,
		[0].scale = 00,

		[1].slice = BIT(7) | 8,
		[1].scale = 00,

		[2].slice = BIT(7) | 10,
		[2].scale = 00,

		[3].slice = BIT(7) | 12,
		[3].scale = 00,

		[4].slice = BIT(7) | 14,
		[4].scale = 00,

		[5].slice = BIT(7) | 16,
		[5].scale = 00,

		[6].slice = BIT(7) | 18,
		[6].scale = 00,

		[7].slice = BIT(7) | 20,
		[7].scale = 00,

		[8].slice = BIT(7) | 22,
		[8].scale = 00,

		[9].slice = BIT(7) | 26,
		[9].scale = 00,

		[10].slice = BIT(7) | 32,
		[10].scale = 00,

		[11].slice = BIT(7) | 36,
		[11].scale = 00,

		[12].slice = BIT(7) | 26,
		[12].scale = 00,
};

void rf_set_tx_iq_scale_slice (u8 scale, u8 slice)
{
	rf_tx_iq_scale = scale;
	rf_tx_iq_slice = slice;
}

void rf_set_tx_tp_slice (u8 slice)
{
	rf_tx_tp_slice = slice;
}

void rf_set_tx_iq_power (rf_pwr_iq_index index)
{
	if(rf_flag_get(RF_FLG_TXPWR_ADJUST_EN)|rf_flag_get(RF_FLG_TXPWR_ADJUST_APP_EN))
	{
		rf_tx_iq_scale = rf_pwr_ctrl_iq[index].scale;
		rf_tx_iq_slice = rf_pwr_ctrl_iq[index].slice;
	}
	else
	{
		rf_tx_iq_scale = rf_pwr_iq[index].scale;
		rf_tx_iq_slice = rf_pwr_iq[index].slice;
	}
	rf_tx_pwr_cur_level = index;
}

_attribute_retention_code_ void rf_set_tx_tp_power (rf_pwr_tp_index index)
{
	if(rf_flag_get(RF_FLG_TXPWR_ADJUST_EN)|rf_flag_get(RF_FLG_TXPWR_ADJUST_APP_EN))
	{
		rf_tx_tp_slice = rf_pwr_ctrl_tp[index].slice;
	}
	else
	{
		rf_tx_tp_slice = rf_pwr_tp[index].slice;
	}
	rf_tx_pwr_cur_level = index;
}



/* default value 0x00
 * for BLE: 0x00, for BT: 0xff */
#define reg_pdzb_ntl_cv         REG_ADDR8(0x140b00)

/* core_140c20 default value 0xc4, BIT(2) is 1, BIT(4) is 0, same as tbl_rf_1m
 * for BLE: 0xc4, for BT: 0xd0
 * core_140c21 default value 0x00 */
#define reg_modem_mode_rx_mz    REG_ADDR16(0x140c20)

/* default value 0x0026
 * for BLE: 0x0126, for BT: 0x0026 */
#define reg_burst_cfg_txrx_mz	REG_ADDR16(0x80140e28)

/* default value 0x80
 * for BLE: 0x80, for BT: 0x00 */
#define reg_ct_dint_care_mz		REG_ADDR8(0x80140e79)

/* default value 0x80 */
#define reg_ntl_cv_mask			REG_ADDR8(0x80140b00)

#define	FLAG_MODEM_NTL_CV					BIT(4)
#define FLAG_CT_FE_CTRIM_SEL				BIT(7)
#define FLAG_BURST_CFG_CHANNEL_NUM_LL_SEL	BIT(8)
#define	FLAG_RF_MODEM_BLE					BIT(2)

#define reg_modem_tx_tl_tctrl       REG_ADDR8(0x140c9a)
#define reg_radio_tx_pa             REG_ADDR16(0x140e26)
#define reg_radio_cfg_txrx          REG_ADDR32(0x140e3c)
#define reg_radio_tx_pa_mode        REG_ADDR8(0x140e3d)
#define reg_radio_modulation_idx    REG_ADDR8(0x140e3e)

_attribute_data_retention_	u8 rf_mode_curr = 0xff;

_attribute_retention_code_
void rf_tx_iq_tp_by_txtype_set(Radio_TxModeDef mode)
{
	if (rf_mode_curr != mode)
	{
		rf_mode_curr = mode;

		if (RADIO_TX_MODE_BLE == mode) // BLE, RF mode: two point
		{
			reg_modem_mode_rx_mz |= FLAG_RF_MODEM_BLE;
			reg_pdzb_ntl_cv = 0x00;
			reg_modem_mode_rx_mz &= ~(FLAG_MODEM_NTL_CV);
			reg_burst_cfg_txrx_mz |= FLAG_BURST_CFG_CHANNEL_NUM_LL_SEL;
			reg_ct_dint_care_mz   |= FLAG_CT_FE_CTRIM_SEL;

			reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07b) | ((rf_tx_tp_slice & 0x3f) << 7);	//BIT(2) = 0: disable IQ; BIT(12:7): power
			reg_rf_mode_cfg_tx3_0 = ( (reg_rf_mode_cfg_tx3_0 & (~BIT(6))) |  (((rf_tx_tp_slice >> 7) & 0x01) << 6) );	//set vbat or vant
			reg_radio_tx_pa_mode  &= ~BIT(4);		// select manual mode to control power
			reg_modem_tx_tl_tctrl &= ~BIT(3);		// disable tx tp align

			reg_radio_modulation_idx = 0x40;		// modulation index h40-0.50

		}
		else if (RADIO_TX_MODE_TP == mode) // BT BR, RF mode: two point
		{
			reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07b) | ((rf_tx_tp_slice & 0x3f) << 7);	//BIT(2) = 0: disable IQ; BIT(12:7): power
			reg_rf_mode_cfg_tx3_0 = ( (reg_rf_mode_cfg_tx3_0 & (~BIT(6))) |  (((rf_tx_tp_slice >> 7) & 0x01) << 6) );	//set vbat or vant
			reg_radio_tx_pa_mode  &= ~BIT(4);		// select manual mode to control power
			reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align to make sure frequency accuracy

			reg_radio_modulation_idx = 0x29;		// modulation index h29-0.32

			rf_set_bt_tail_delay(5);

            bt_radiotxrxtim_pack(BT_RADIO_SYNC_OFFSET, BT_RADIO_RX_EIR_ADJ_EN, BT_RADIO_RX_PATH_DELAY, BT_RADIO_TP_TX_PATH_DELAY);
            bt_radiopwrupdn_pack(BT_RADIO_RX_POWER_UP, BT_RADIO_TP_POWER_DOWN, BT_RADIO_TP_TX_POWER_UP);

            log_b8(SL_STACK_RADIO_EN,SL08_tx_path_delay,bt_radiotxrxtim_txpathdly_getf());

            write_reg8(0x140e3f, (read_reg8(0x140e3f)|0x80));			//fast settle enable
            write_reg8(0x140e84, 0x62);

			write_reg32(0x140e9c, 0x1b090900);
			write_reg16(0x140ea0, 0x3232);
			write_reg16(0x140e96, 0x0900);
			write_reg32(0x140e98, 0x30343130);
		}
		else if (RADIO_TX_MODE_IQ == mode) // BT EDR, RF mode: iq
		{
			reg_modem_mode_rx_mz &= ~FLAG_RF_MODEM_BLE;
			reg_pdzb_ntl_cv = 0xff;
			reg_modem_mode_rx_mz |= FLAG_MODEM_NTL_CV;
			reg_burst_cfg_txrx_mz &= ~FLAG_BURST_CFG_CHANNEL_NUM_LL_SEL;
			reg_ct_dint_care_mz   &= ~FLAG_CT_FE_CTRIM_SEL;
			reg_rf_mode_cfg_tx3_0 &= ~BIT(6);		//clear bit(6) to set to vbat mode

			rf_set_bt_tail_delay(2);

            bt_radiotxrxtim_pack(BT_RADIO_SYNC_OFFSET, BT_RADIO_RX_EIR_ADJ_EN, BT_RADIO_RX_PATH_DELAY, BT_RADIO_IQ_TX_PATH_DELAY);
            bt_radiopwrupdn_pack(BT_RADIO_RX_POWER_UP, BT_RADIO_IQ_POWER_DOWN, BT_RADIO_IQ_TX_POWER_UP);
            log_b8(SL_STACK_RADIO_EN,SL08_tx_path_delay,bt_radiotxrxtim_txpathdly_getf());

            if (rf_flag_get(RF_FLG_IQ_PWR_MANUAL_EN))
            {
                reg_radio_tx_pa_mode  &= ~BIT(4);       // select manual mode to control power
                reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07f) | BIT(2) | (rf_tx_iq_slice << 7);  //BIT(2) = 1: enable IQ; BIT(12:7): power, br_mdm_slice
                write_reg8(0x140c91, ((read_reg8(0x140c91)&0xe0)|0x20|rf_tx_iq_scale)); 		//br_mdm_scale open and set value
        		reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align
            }
            else
            {
        		reg_radio_tx_pa       |= BIT(2);	    // BIT(2) = 1: enable bt iq
                write_reg8(0x140c91, ((read_reg8(0x140c91)&0xe0)|0x20|rf_tx_iq_scale)); // br_mdm_scale open and set fix value
        		reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align
        		reg_radio_tx_pa_mode  |= BIT(4);	    // select auto mode to control power by power index in cs-structure ,auto set slice
                REG_ADDR8(0x140c9c)    = 0x0a;          // tx modulation index of I/Q mode:BT
                REG_ADDR8(0x140c9d)    = (REG_ADDR8(0x140c9d)&0xe0)|0x07;
            }
        }
	}
}

bool rf_txpwr_inc(uint8_t link_id)
{
	unsigned char rf_tx_pwr_set_inc_en = 0;
	unsigned char rf_tx_pwr_level_max = 0;
	if(rf_flag_get(RF_FLG_TXPWR_ADJUST_EN))
	{
		rf_tx_pwr_set_inc_en = 1;
		rf_tx_pwr_level_max = RF_TXPWR_LEVEL_MAX;
	}
	else if(rf_flag_get(RF_FLG_TXPWR_ADJUST_APP_EN))
	{
		rf_tx_pwr_set_inc_en = 1;
		rf_tx_pwr_level_max = RF_TXPWR_LEVEL_APP_MAX;
	}
	if(rf_tx_pwr_set_inc_en){
		if (rf_tx_pwr_cur_level < rf_tx_pwr_level_max)
		{
			rf_tx_pwr_cur_level++;

			rf_tx_pwr_cur = rf_pwr_ctrl_iq[rf_tx_pwr_cur_level];

			if(rf_flag_get(RF_FLG_IQ_PWR_MANUAL_EN))
			{
				rf_set_tx_iq_scale_slice (rf_tx_pwr_cur.scale, rf_tx_pwr_cur.slice);

				if(!rf_flag_get(RF_FLG_IQ_TP_SWITCH_EN))
				{
		            reg_radio_tx_pa_mode  &= ~BIT(4);       // select manual mode to control power
		            reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07f) | BIT(2) | (rf_tx_iq_slice << 7);  //BIT(2) = 1: enable IQ; BIT(12:7): power, br_mdm_slice
		            write_reg8(0x140c91, ((read_reg8(0x140c91)&0xe0)|0x20|rf_tx_iq_scale)); 		//br_mdm_scale open and set value

		            reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align
				}
				else
				{
					rf_set_tx_tp_slice (rf_pwr_ctrl_tp[rf_tx_pwr_cur_level].slice);
					rf_mode_curr = 0xff;
				}
			}
			else
			{
				em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), rf_tx_pwr_cur.slice); // slice
			}

			return false;
		}

	}
	return true;
}

bool rf_txpwr_dec(uint8_t link_id)
{
	if(rf_flag_get(RF_FLG_TXPWR_ADJUST_EN)|rf_flag_get(RF_FLG_TXPWR_ADJUST_APP_EN))
	{
		if (rf_tx_pwr_cur_level > RF_TXPWR_LEVEL_MIN)//default: 0-> maxpower
		{
			rf_tx_pwr_cur_level--;

			rf_tx_pwr_cur = rf_pwr_ctrl_iq[rf_tx_pwr_cur_level];

			if(rf_flag_get(RF_FLG_IQ_PWR_MANUAL_EN))
			{
				rf_set_tx_iq_scale_slice (rf_tx_pwr_cur.scale, rf_tx_pwr_cur.slice);

				if(!rf_flag_get(RF_FLG_IQ_TP_SWITCH_EN))
				{
		            reg_radio_tx_pa_mode  &= ~BIT(4);       // select manual mode to control power
		            reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07f) | BIT(2) | (rf_tx_iq_slice << 7);  //BIT(2) = 1: enable IQ; BIT(12:7): power, br_mdm_slice
		            write_reg8(0x140c91, ((read_reg8(0x140c91)&0xe0)|0x20|rf_tx_iq_scale)); 		//br_mdm_scale open and set value
		    		reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align
				}
				else
				{
					rf_set_tx_tp_slice (rf_pwr_ctrl_tp[rf_tx_pwr_cur_level].slice);
					rf_mode_curr = 0xff;
				}
			}
			else
			{
				em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), rf_tx_pwr_cur.slice); // slice
			}
			return false;
		}
	}

	return true;
}

void txpwr_max_set(uint8_t link_id)
{
	unsigned char rf_tx_pwr_set_max_en = 0;

	if(rf_flag_get(RF_FLG_TXPWR_ADJUST_EN))
	{
		rf_tx_pwr_set_max_en = 1;
		rf_tx_pwr_cur_level = RF_TXPWR_LEVEL_MAX;
	}
	else if(rf_flag_get(RF_FLG_TXPWR_ADJUST_APP_EN))
	{
		rf_tx_pwr_set_max_en = 1;
		rf_tx_pwr_cur_level = RF_TXPWR_LEVEL_APP_MAX;
	}

	if(rf_tx_pwr_set_max_en)
	{
		rf_tx_pwr_cur = rf_pwr_ctrl_iq[rf_tx_pwr_cur_level];

		if(rf_flag_get(RF_FLG_IQ_PWR_MANUAL_EN))
		{
			rf_set_tx_iq_scale_slice (rf_tx_pwr_cur.scale, rf_tx_pwr_cur.slice);

			if(!rf_flag_get(RF_FLG_IQ_TP_SWITCH_EN))
			{
	            reg_radio_tx_pa_mode  &= ~BIT(4);       // select manual mode to control power
	            reg_radio_tx_pa = (reg_radio_tx_pa & 0xe07f) | BIT(2) | (rf_tx_iq_slice << 7);  //BIT(2) = 1: enable IQ; BIT(12:7): power, br_mdm_slice
	            write_reg8(0x140c91, ((read_reg8(0x140c91)&0xe0)|0x20|rf_tx_iq_scale)); 		//br_mdm_scale open and set value
	    		reg_modem_tx_tl_tctrl |= BIT(3);		// enable tx tp align
			}
			else
			{
				rf_set_tx_tp_slice (rf_pwr_ctrl_tp[rf_tx_pwr_cur_level].slice);
			}
		}
		else
		{
			//Increase the TX power value
		    em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), rf_tx_pwr_cur.slice);
		}
	}
}



/**********************************************************************************************************************
 * RF RADIO/MODEM SETTING BEGIN:
 *********************************************************************************************************************/

/***********************************
* add power ramp up time  ||from rf.c
*  0x140e24[1:0]
*  b01: 250ns/step, 8*250ns = 2us
*  b10: 500ns/step, 8*500ns = 4us
*  b11: 1us/step , 8*1us = 8us
***********************************/
static inline void rf_bt_add_power_ramp_up_time(u8 val)
{
	write_reg8(0x140e24,((read_reg8(0x140e24) & 0xfc) | (val & 0x03)) );
}

static inline void rf_init_ntl_cv(void)
{
    reg_pdzb_cv = 0xffffffff;
}

static inline void rf_set_em_base_address(uint32_t addr )
{
    reg_pdzb_em_base_address = addr;
}


static inline void rf_set_modem_mode(uint8_t mode)
{
    reg_modem_mode_rx &= (uint8_t)(~FLD_MODEM_BLE_BT_SEL);
    reg_modem_mode_rx |= (mode<<2);
}

static inline void rf_set_modem_ntl_cv_en(void)
{
    reg_modem_mode_rx|=FLD_MODEM_NTL_CV;
}

static inline void rf_set_freq_chn_auto(void)
{
    reg_burst_cfg_txrx &= ~FLD_BURST_CFG_CHANNEL_NUM_LL_SEL;
    reg_ct_dint_care   &= ~FLD_CT_FE_CTRIM_SEL;
    reg_chnl_freq_direct &= ~FLD_TXRX_DBG3_0_CHNL_FREQ_DIRECT;
}

static inline void rf_set_power_auto(void)
{
    // set power by power index in BR
    reg_mode_cfg_tx3 |= FLD_MODE_CFG_TX3_TXC_PWR_SRL;
}

static inline void rf_set_auto_mode(int v)
{
    if(v)
    {
        reg_modem_mode_rx |= FLD_MODEM_BLE_BT_AUTO_SEL_EN;
    }
    else
    {
        reg_modem_mode_rx &= ~FLD_MODEM_BLE_BT_AUTO_SEL_EN;
    }
}

/***********************************
* add data delay after data on.note :PLL settle time need to add same time.
*  0x140c80[6:5]
*  00 : delay = 0;
*  01:  delay = 2us
*  10:  delay = 4us
***********************************/
void rf_bt_add_tx_header_delay_time(u8 val)
{
	write_reg8(0x140c80,((read_reg8(0x140c80) & 0x9f) | ((val & 0x03)<<5)) );
}

void rf_set_bt_tail_delay(int v)
{
    reg_modem_mode_trx1 &=~FLD_TX_TAIL_DELAY_BT;
    reg_modem_mode_trx1 |= FLD_TX_TAIL_DELAY_BT&(v<<4);
}

void rf_radio_init(void)
{
	write_reg8(RADIOADDR+0x25, read_reg8(RADIOADDR+0x25)&0xbf); //MODE BT  VBAT TX , 0: vbat, 1: vant;
	write_reg8(RADIOADDR+0x3c, read_reg8(RADIOADDR+0x3c)&0xbf); //MODE BT  VBAT TX , 0: vbat, 1: vant;

    //BT/BLE:To modify DCOC parameters
	write_reg8(RADIOADDR+0xd2, ((read_reg8(RADIOADDR+0xd2) & 0x0f) | 0x90) ); //(4)DCOC_SFIIP:1 (5)DCOC_SFQQP:0 (7:6)DCOC_SFII_L:2
	write_reg8(RADIOADDR+0xd3, ((read_reg8(RADIOADDR+0xd3) & 0xc0) | 0x19) ); //(1:0)DCOC_SFII_H:1  (5:2)DCOC_SFQQ:6

    //BLE:Setting for blanking
	write_reg8(RADIOADDR+0x7b, (read_reg8(RADIOADDR+0x7b) | 0x0c) );	//(3:2)BLANK_WINDOW:3

    //BT/BLE:To set AGC thresholds
	write_reg8(RADIOADDR+0x4a, 0x0e); //(6:0)POW_000_001:0x0e  (7)POW_001_010_L:0
	write_reg8(RADIOADDR+0x4b, ((read_reg8(RADIOADDR+0x4b) & 0xc0) | 0x09) ); //(5:0)POW_001_010_H:0x09
	write_reg8(RADIOADDR+0x4e, 0x09); //(6:0)POW_100_101:0x09  (7)POW_101_100_L:0
	write_reg8(RADIOADDR+0x4f, ((read_reg8(RADIOADDR+0x4f) & 0xc0) | 0x0f) ); //(5:0)POW_101_100_H:0x0f
	write_reg8(RADIOADDR+0x54, 0x0e); //(6:0)POW_000_001:0x0e  (7)POW_001_010_L:0
	write_reg8(RADIOADDR+0x55, ((read_reg8(RADIOADDR+0x55) & 0xc0) | 0x09) ); //(5:0)POW_001_010_H:0x09
	write_reg8(RADIOADDR+0x56, 0x0c); //(6, 0)POW_010_011:0x0c  (7)POW_011_100_L:0
	write_reg8(RADIOADDR+0x57, ((read_reg8(RADIOADDR+0x57) & 0xc0) | 0x08) ); //(5:0)POW_011_100_H:0x08
	write_reg8(RADIOADDR+0x58, 0x09); // (6:0)POW_100_101:0x09  (7)POW_101_100_L:0
	write_reg8(RADIOADDR+0x59, ((read_reg8(RADIOADDR+0x59) & 0xc0) | 0x0f) ); //(5:0)POW_101_100_H:0x0f

    //BLE:For optimum preamble detection
	write_reg16(MODEMADDR+0x76,0x7350); //RX_PE_DET_MIN_THRESH
	write_reg8(MODEMADDR+0x91, ((read_reg8(MODEMADDR+0x91) & 0xe0) | 0x36) ); //(5)scale en:1  (4:0)scale value 32PIN:0x16

	//sometimes there is a sudden frequency offset in BT ,close this bit can solve it.but never
	//found err in previous chip. So the real reason need to be analyzed later.
	analog_write_reg8(0x8b, 0x04);	//0x04 ? default 0x06 , bit(1)reg_xo_dyn_cap_ana turn to 0

	write_reg32(0x160070,0);
	write_reg32(0x160074,0x4020);
	write_reg32(0x160078,0x4070100);
	write_reg32(0x16007c,0x39003900);
	//BT: ceva ip guard time settting
    //cv guard time 6us
	write_reg32(0x160428,0x1a000612);
	write_reg32(0x1604e0,0x1900140);

	//BT: GUARD TIME delay (decrease 11->0.5us (24->1us))
	write_reg8(0x140c84,0x4b);		//fixed the issue that cann't send 2DH1 packet to the MEILAN note2 phone.
	rf_bt_add_power_ramp_up_time(1);//fix sync err during tx mode in CMW500

    //2019/10/29 Junwen add, to fix per floor
    //2020/04/26 Qiangkai remove, move this setting to aura_1m script
    //sub_wr(MODEMADDR+0x79, 1, 3, 3) //RX_DIS_PDET_BLANK

// IQ.  RF_TX_FAST_SETTLE_ID enable.
    if (rf_flag_get(RF_FLG_IQ_ID_FASTSETTLE_EN))
    {
        write_reg8(RADIOADDR+0x3f, read_reg8(RADIOADDR+0x3f) | 0x80); //(7):1
        write_reg8(RADIOADDR+0x84, read_reg8(RADIOADDR+0x84) & 0x44);	//(7):0  (5:3):0  (1:0):0
        write_reg8(RADIOADDR+0x9d, 0x09);
        write_reg8(RADIOADDR+0x9e, 0x09);
        write_reg8(RADIOADDR+0x9f, 0x0e);
        write_reg8(RADIOADDR+0xa0, 0x25);
        write_reg8(RADIOADDR+0xa1, 0x25);
        write_reg8(RADIOADDR+0xa9, 0x09);
        write_reg8(RADIOADDR+0xaa, 0x25);
        write_reg8(RADIOADDR+0xab, 0x28);
        write_reg8(RADIOADDR+0xad, 0x2b);
    }

// IQ.  RF_TX_FAST_SETTLE_NON_ID enable.
    if (rf_flag_get(RF_FLG_IQ_NONID_FASTSETTLE_EN))
    {
        write_reg8(RADIOADDR+0x3f, read_reg8(RADIOADDR+0x3f) | 0xc0); //(7):1  (6):1
        write_reg8(RADIOADDR+0x85, ((read_reg8(RADIOADDR+0x85) & 0x44) | 0x23) ); //(7):0  (5):1  (4:3):0  (1:0):3
	    write_reg8(RADIOADDR+0xb7, 0x09);
	    write_reg8(RADIOADDR+0xb8, 0x09);
	    write_reg8(RADIOADDR+0xb9, 0x0e);
	    write_reg8(RADIOADDR+0xba, 0x25);
        write_reg8(RADIOADDR+0xbb, 0x25);
        write_reg8(RADIOADDR+0xb1, 0x09);
        write_reg8(RADIOADDR+0xb2, 0x25);
        write_reg8(RADIOADDR+0xb3, 0x28);
        write_reg8(RADIOADDR+0xb5, 0x2b);
    }


// TP.  RF_TX_FAST_SETTLE_ID enable.
    if (rf_flag_get(RF_FLG_TP_ID_FASTSETTLE_EN))
    {
    	write_reg8(RADIOADDR+0x97, 0x09);
        write_reg8(RADIOADDR+0x98, 0x25);
        write_reg8(RADIOADDR+0x99, 0x28);
        write_reg8(RADIOADDR+0x9B, 0x2b);
    }

// TP.  RF_TX_FAST_SETTLE_NON_ID enable.
    if (rf_flag_get(RF_FLG_TP_NONID_FASTSETTLE_EN))
    {

    }

// 9518, rf PA enable.
    if (rf_flag_get(RF_FLG_PA_MANUAL_EN|RF_FLG_PA_AUTO_EN))
    {
        //MODE BT  VBAT TX , 0: vbat, 1: vant;
    	write_reg8(RADIOADDR+0x25,(read_reg8(RADIOADDR+0x25) & (~BIT(6))) );
    	if(rf_flag_get(RF_FLG_PA_MANUAL_EN))
    	{
        	gpio_function_en(PA_TX_EN_PIN); // simulate PA TX EN
        	gpio_set_output(PA_TX_EN_PIN,1);
    		gpio_set_low_level(PA_TX_EN_PIN);

    		gpio_function_en(PA_RX_EN_PIN); // simulate PA TX EN
    		gpio_set_output(PA_RX_EN_PIN,1);
    		gpio_set_low_level(PA_RX_EN_PIN);
    	}
    	else
    	{
    		rf_set_rffe_pin(PA_TX_EN_PIN, PA_RX_EN_PIN);// for sip package
    	}
    }

    rf_tx_mode_reset();
}

void rf_br_manual_init(void)
{
    rf_set_modem_ntl_cv_en();
    rf_set_bt_tail_delay(2);        // for tx setting : this setting is necessary for tail decode error
    reg_modem_pdet_thre = 0x1e;     // for rx sync: preamble sync threshold
    reg_modem_sync_thre_bt = 0x1e;  // for rx sync: bt sync thre
    rf_init_ntl_cv();
    rf_set_auto_mode(0);            // open manual mode
    rf_set_modem_mode(RF_MODEM_BT); // mode
}

void btc_radio_modem_init(void)
{
    //channel control mode
    rf_set_freq_chn_auto();
    //power contrl mode
    rf_set_power_auto();

    rf_radio_init();

    rf_br_manual_init();
}

void async_rf_fast_settle_en (int en)
{
	if (en)
	{
	  	reg_rf_ll_rxstl_l = 48;
	   	reg_rf_ll_tx_stl = 48;
		REG_ADDR8(RADIOADDR+0x29) |= BIT(3) | BIT(4);
	}
	else
	{
	  	reg_rf_ll_rxstl_l = 80;
	   	reg_rf_ll_tx_stl = 100;
		REG_ADDR8(RADIOADDR+0x29) &= ~(BIT(3) | BIT(4));
	}
}

void async_rf_fast_settle_init (void)
{
   	reg_rf_ll_rxstl_l = 0x80;
   	reg_rf_ll_tx_stl = 0x80;					// tx settle
   	reg_rf_preamble_trail = 0x43;			// preamble

//    REG_ADDR8(RADIOADDR+0x84) = 0x73;				//{rx_dcoc,rx_rccal,rx_fcal,rx_ldo,  tx_dcoc,tx_hpmc,tx_fcal,tx_ldo}
    REG_ADDR8(RADIOADDR+0x84) = 0x62;

    REG_ADDR32(RADIOADDR+0x9c) = 0x1b090900;		//RX
   	REG_ADDR16(RADIOADDR+0xa0) = 0x3030;			// 32 32

   	REG_ADDR16(RADIOADDR+0x96) = 0x0900;			//TX 0x0d00  0x32393635  0x2b292825  0x30343130
   	REG_ADDR32(RADIOADDR+0x98) = 0x30343130;
 #if 0

    //sub_wr(RADIOADDR+0x3f, 0x01, 7, 7); cv id
    sub_wr(RADIOADDR+0x29, 0x01, 3, 3);//manual rx
    sub_wr(RADIOADDR+0x29, 0x01, 4, 4); /manual tx
    //disable

   sub_wr(RADIOADDR+0x84, 0x00, 0, 0); //tx ldo trim
   sub_wr(RADIOADDR+0x84, 0x01, 1, 1); //tx fcal
   sub_wr(RADIOADDR+0x84, 0x00, 2, 2); //tx hpmc
   sub_wr(RADIOADDR+0x84, 0x00, 3, 3); //tx dcoc
   sub_wr(RADIOADDR+0x84, 0x00, 4, 4); //rx ldo trim
   sub_wr(RADIOADDR+0x84, 0x01, 5, 5); //rx fcal
   sub_wr(RADIOADDR+0x84, 0x01, 6, 6); //rx rccal
   sub_wr(RADIOADDR+0x84, 0x00, 7, 7); //rx dcoc

    //rx
   sub_wr(RADIOADDR+0x9c, 0x00, 7, 0);
   sub_wr(RADIOADDR+0x9d, 0x09, 7, 0);
   sub_wr(RADIOADDR+0x9e, 0x09, 7, 0);
   sub_wr(RADIOADDR+0x9f, 0x1b, 7, 0);
   sub_wr(RADIOADDR+0xa0, 0x32, 7, 0);
   sub_wr(RADIOADDR+0xa1, 0x32, 7, 0);

    //tx
   sub_wr(RADIOADDR+0x96, 0x00, 7, 0);
   sub_wr(RADIOADDR+0x97, 0x09, 7, 0);
   sub_wr(RADIOADDR+0x98, 0x30, 7, 0);
   sub_wr(RADIOADDR+0x99, 0x31, 7, 0);
   sub_wr(RADIOADDR+0x9a, 0x34, 7, 0);
   sub_wr(RADIOADDR+0x9b, 0x30, 7, 0);
#endif
}

/// wait clear
void bt_ip_clk_reg_reset(void)
{
	write_reg32(0x1401e0,0xf9ffffff);
	write_reg32(0x1401e0,0xffffffff);
	bt_rwbtcntl_reg_soft_rst_setf(1);
	while(bt_rwbtcntl_reg_soft_rst_getf());
}

void rf_set_br_clk(void)
{
    reg_clk_en3 &= ~FLD_CLK3_ZB_MSTCLK_EN;  //turn off master clock
    ip_rwdmcntl_master_tgsoft_rst_setf(1);  //reset master_clock domain
//    clock_init(SYSCLK_CLK_DIV,10);          //hclk is generated from PLL divider
    //caution: fpga fixed = 24M
#if 1

   	u32 *pd = (u32*) REG_EM_BASE_ADDR;
    for (int i=0; i<EM_SIZE_IN_U32; i++)
    {
    	pd[i] = 0;
    }

    em_wr32p(0x100,0x06040200);
    em_wr32p(0x104,0x0e0c0a08);
    em_wr32p(0x108,0x16141210);
    em_wr32p(0x10c,0x1e1c1a18);
    em_wr32p(0x110,0x26242220);
    em_wr32p(0x114,0x2e2c2a28);
    em_wr32p(0x118,0x36343230);
    em_wr32p(0x11c,0x3e3c3a38);
    em_wr32p(0x120,0x46444240);
    em_wr32p(0x124,0x4e4c4a48);
    em_wr32p(0x128,0x07050301);
    em_wr32p(0x12c,0x0f0d0b09);
    em_wr32p(0x130,0x17151311);
    em_wr32p(0x134,0x1f1d1b19);
    em_wr32p(0x138,0x27252321);
    em_wr32p(0x13c,0x2f2d2b29);
    em_wr32p(0x140,0x37353331);
    em_wr32p(0x144,0x3f3d3b39);
    em_wr32p(0x148,0x47454341);
    em_wr32p(0x14c,0x4f4d4b49);
#endif
    reg_clk_en3 |= FLD_CLK3_ZB_MSTCLK_EN;
}

void rf_br_init(void)
{
    rf_set_freq_chn_auto();     // channel controlled by BR
    rf_set_power_auto();        // power controlled by power index in BR

    rf_radio_init();

    rf_br_manual_init();

    bt_radiocntl2_freqtable_ptr_setf(EM_FT_OFFSET);
}
