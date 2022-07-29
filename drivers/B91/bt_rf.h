/********************************************************************************************************
 * @file     bt_rf.h
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

#ifndef _BT_RF_H_
#define _BT_RF_H_

#include "rf.h"

/**********************************************************************************************************************
 * RF FLAG BEGIN:
 *********************************************************************************************************************/
#define RF_FLG_ALL_MSK                              0xFFFFFFFFUL

#define RF_FLG_IQ_ID_FASTSETTLE_EN                  (0x01UL << 0)   // IQ fast settle en.
#define RF_FLG_IQ_NONID_FASTSETTLE_EN               (0x01UL << 1)
#define RF_FLG_TP_ID_FASTSETTLE_EN                  (0x01UL << 2)   // TP fast settle en.
#define RF_FLG_TP_NONID_FASTSETTLE_EN               (0x01UL << 3)
#define RF_FLG_IQ_TP_SWITCH_EN                      (0x01UL << 4)
#define RF_FLG_IQ_ONLY_EN                           (0x01UL << 5)
#define RF_FLG_IQ_PWR_MANUAL_EN                     (0x01UL << 6)
#define RF_FLG_PA_MANUAL_EN                         (0x01UL << 7)
#define RF_FLG_PA_AUTO_EN                           (0x01UL << 8)
#define RF_FLG_TXPWR_ADJUST_EN						(0x01UL << 9)
#define RF_FLG_TXPWR_ADJUST_APP_EN					(0x01UL << 10)

extern u32 rf_flag;
extern void rf_flag_set(u32 flag, u32 en);

static inline u32 rf_flag_get(u32 flag)
{
	return (rf_flag&(flag));
}

/**********************************************************************************************************************
 * RF MANUAL MODE IN PWR ADJUST SETTING BEGIN:
 *********************************************************************************************************************/

#define         RF_TXPWR_LEVEL_MAX          4
#define         RF_TXPWR_LEVEL_APP_MAX      6
#define         RF_TXPWR_LEVEL_MIN          0

typedef struct _rf_tx_pwr {
	unsigned char slice;
	unsigned char scale;
} __attribute__ ((__packed__)) rf_tx_pwr_t;

rf_tx_pwr_t rf_tx_pwr_cur = {
    .slice = 63,
    .scale = 16, // 1.5dBm
};

u8 rf_tx_pwr_cur_level = RF_TXPWR_LEVEL_MAX; // default max txpwr

extern rf_tx_pwr_t rf_tx_pwr_cur;

bool rf_txpwr_dec(uint8_t link_id);
bool rf_txpwr_inc(uint8_t link_id);
void txpwr_max_set(uint8_t link_id);

/**********************************************************************************************************************
 * RF AUTO MODE IN PWR ADJUST SETTING BEGIN:
 *********************************************************************************************************************/

#ifndef	BT_AUTO_PWR_DFT
#define BT_AUTO_PWR_DFT    0x28        // slice
#endif

/**********************************************************************************************************************
 * RF PWR NO ADJUST SETTING BEGIN:
 *********************************************************************************************************************/

/*  IQ modulation : VABT mode
 *
 * */
typedef enum {
	RF_IQ_POWER_INDEX_N10P0_dBm,
	RF_IQ_POWER_INDEX_N9P01_dBm,
	RF_IQ_POWER_INDEX_N8P00_dBm,
	RF_IQ_POWER_INDEX_N7P04_dBm,
	RF_IQ_POWER_INDEX_N6P03_dBm,
	RF_IQ_POWER_INDEX_N5P00_dBm,
	RF_IQ_POWER_INDEX_N4P09_dBm,
	RF_IQ_POWER_INDEX_N3P09_dBm,
	RF_IQ_POWER_INDEX_N2P08_dBm,
	RF_IQ_POWER_INDEX_N1P05_dBm,
	RF_IQ_POWER_INDEX_N0P01_dBm,
	RF_IQ_POWER_INDEX_P1P00_dBm,
	RF_IQ_POWER_INDEX_P1P50_dBm,
	RF_IQ_POWER_INDEX_MAX,
	/**  For external PA (gain= +26dbm)**/
	RF_IQ_PA_POWER_INDEX_P08P05_dBm,
	RF_IQ_PA_POWER_INDEX_P10P93_dBm,
	RF_IQ_PA_POWER_INDEX_P12P82_dBm,
	RF_IQ_PA_POWER_INDEX_P14P27_dBm,
	RF_IQ_PA_POWER_INDEX_P15P39_dBm,
}rf_pwr_iq_index;

/*  TP modulation : VABT mode -> VANT mode
 *
 * */
typedef enum{

	RF_TP_POWER_INDEX_N8p78dBm,
	RF_TP_POWER_INDEX_N6p54dBm,
	RF_TP_POWER_INDEX_N4p77dBm,
	RF_TP_POWER_INDEX_N3p37dBm,
	RF_TP_POWER_INDEX_N2p01dBm,
	RF_TP_POWER_INDEX_N1p37dBm,
	RF_TP_POWER_INDEX_N0p53dBm,
	RF_TP_POWER_INDEX_P0p01dBm,
	RF_TP_POWER_INDEX_P0p80dBm,
	RF_TP_POWER_INDEX_P1p72dBm,
	RF_TP_POWER_INDEX_P2p32dBm,
	RF_TP_POWER_INDEX_P2p79dBm,
	RF_TP_POWER_INDEX_MAX,
}rf_pwr_tp_index;

extern u8 rf_tx_iq_scale;
extern u8 rf_tx_iq_slice;
extern u8 rf_tx_tp_slice;

void rf_set_tx_iq_power (rf_pwr_iq_index index);
void rf_set_tx_tp_power (rf_pwr_tp_index index);

void rf_set_tx_iq_scale_slice (u8 scale, u8 slice);
void rf_set_tx_tp_slice (u8 slice);

/**********************************************************************************************************************
 * RF PA MODULE SETTING BEGIN:
 *********************************************************************************************************************/

#ifndef PA_TX_EN_PIN
	#define PA_TX_EN_PIN   RF_RFFE_TX_PD7
#endif

#ifndef PA_RX_EN_PIN
	#define PA_RX_EN_PIN   RF_RFFE_RX_PD6
#endif

#define PA_TX_RX_SWITCH_US          10

/**********************************************************************************************************************
 * RF PATH/DELAY/POWERUP/POWERDOWN SETTING BEGIN:
 *********************************************************************************************************************/

#define	BT_RADIO_RX_EIR_ADJ_EN		1// for inquiry eir

#define	BT_RADIO_SYNC_OFFSET		0

#define	BT_RADIO_RX_PATH_DELAY		14

#define	BT_RADIO_IQ_TX_PATH_DELAY	6
#define	BT_RADIO_TP_TX_PATH_DELAY	6

#define	BT_RADIO_RX_POWER_UP		(100 - BT_RADIO_RX_PATH_DELAY)		//CEVA bug

#define BT_RADIO_TP_TX_POWER_UP     110 //>108
#define	BT_RADIO_IQ_TX_POWER_UP		100

#define	BT_RADIO_IQ_POWER_DOWN		4
#define	BT_RADIO_TP_POWER_DOWN		8

/**********************************************************************************************************************
 * RF TX MODE SETTING BEGIN:
 *********************************************************************************************************************/

typedef enum {
    RADIO_TX_MODE_BLE = 1,
	RADIO_TX_MODE_IQ = 2,
	RADIO_TX_MODE_TP  =3
} Radio_TxModeDef;

extern unsigned char rf_mode_curr;

static inline void rf_tx_mode_reset(void) {
	rf_mode_curr = 0xff;
}

void rf_tx_iq_tp_by_txtype_set(Radio_TxModeDef mode);


/**********************************************************************************************************************
 * RF RADIO/MODEM SETTING BEGIN:
 *********************************************************************************************************************/

void btc_radio_modem_init(void);

/**********************************************************************************************************************
 * RF FAST SETTLE SETTING BEGIN:
 *********************************************************************************************************************/

void async_rf_fast_settle_en (int en);
void async_rf_fast_settle_init (void);

///wait clear
void bt_ip_clk_reg_reset(void);
void rf_set_br_clk(void);
void rf_br_init(void);
#endif /* BT_RF_H_ */
