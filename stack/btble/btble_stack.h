/********************************************************************************************************
 * @file     btble_stack.h
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

#ifndef STACK_BTBLE_BTBLE_STACK_H_
#define STACK_BTBLE_BTBLE_STACK_H_


#include "common/bit.h"
#include "drivers.h"
#include "compiler.h"

#include "btble_scheduler.h"  //when scheduler stack code merge to this file, no need this include



/******************************* debug start ******************************************************************/

//attention: disable this when release SDK !!!
#define	SATCK_DEBUG_EN									1

#if (SATCK_DEBUG_EN)
	void	btble_error_debug(u32 err_code);
	#define	STACK_ERR_DEBUG(cond, err_code)				if(cond){btble_error_debug(err_code);}
#else
	#define	STACK_ERR_DEBUG(cond, err_code)
#endif

#define BLE_ACL_DBG_EN									1
#define	BLE_ACL_CONN_DBG_EN								1
#define	BLE_ACL_SLAVE_DBG_EN							1

#define TX_FIFO_DBG_EN									1


#define BLE_ADV_DBG_EN									1
#define BLE_LEG_SCAN_DBG_EN								1

#define BT_ACL_DBG_EN									1


#define PM_DBG_EN										1
#define DBG_PM_TIMIG									1




#ifndef UART_LOW_POWER_DEBUG_EN
#define UART_LOW_POWER_DEBUG_EN							0
#endif

#if(UART_LOW_POWER_DEBUG_EN)

extern int lp_uart_init;
void low_power_uart_debug_init(void);

#endif

/******************************* debug end  ******************************************************************/









/******************************* btble_controller start ******************************************************************/


#define	BT_MAINLOOP_SPEED_UP_WHEN_NO_TASK			0  //save some ram_code and flash access time







/* attention: same bit as "sleep_state_cfg_t", code can be more easy: use ((A & B) == B) */
#define	STATEMSK_BB_INQUIRY_SCAN		BIT(0)
#define	STATEMSK_BB_PAGE_SCAN			BIT(1)
#define	STATEMSK_BB_ACL_SALVE			BIT(2)

#define	STATEMSK_LL_LEG_ADV				BIT(8)
#define	STATEMSK_LL_ACL_SLAVE			BIT(9)



/* BIT(15): special process when low power wake_up, SiHui can explain what it solve
 * only used for "ctlr_state_busy" */
#define STATE_PENDING_FLAG				BIT(15)  //there is a BT or BLE state task is nearby, ready to be run

#define STATEMSK_BT_ALL					0x00FF  //BIT(0-7)
#define STATEMSK_BLE_ALL				0x7F00  //BIT(8-14)

typedef struct {

//0x00
	u8		sdk_mainloop_flg;
	u8		host_task_busy; //for PM module detect if host process data & event over. <0..3> for BT, <4..7> for BLE
	u8		u8_rsvd[2];


	u16		ctlr_state_en;	 //controller state enable, current state combination
	u16		ctlr_state_busy; //controller state busy
//	u16		u16_rsvd;



} bt_ble_para_t;

extern _attribute_aligned_(4)	bt_ble_para_t  btble_stk;



/**
 * @brief	main loop function of BT stack
 * @param	none
 * @return	none
 */
void bt_mainloop(void);

void btble_event_callback_func(u8 e, u8 *p, int n);
/******************************* btble_controller end ******************************************************************/












/******************************* btble_scheduler start ******************************************************************/


typedef	void (*ll_mainloop_callback_t) (void);
extern ll_mainloop_callback_t mainloop_ble_cb;

typedef void (* bt_mainloop_callback_t)(void);
extern bt_mainloop_callback_t mainloop_bt_cb;
extern bt_mainloop_callback_t mainloop_bt_host;

static inline void bt_host_task_register(bt_mainloop_callback_t bt_host_task)
{
    mainloop_bt_host = bt_host_task;
}


typedef	u32 (*ll_low_power_timing_callback_t) (int, u32, u32);
extern ll_low_power_timing_callback_t	ll_low_power_timing_cb;
extern ll_low_power_timing_callback_t	tws_low_power_timing_cb;

/******************************* btble_scheduler end   ******************************************************************/















/******************************* btble_pm start ******************************************************************/
#ifndef BTBLE_PM_ENABLE
#define BTBLE_PM_ENABLE							1
#endif


/* sniff sync max time, optimize later */
#define BT_SNIFF_MAX_US									10000

/* ble_conn max time next to a BT sniff task */
#define BLE_CONN_MAX_US							    	5000  //must bigger than 3000

typedef struct schedule_low_power_para
{
	//00
	u8		tag;
	u8		lp_link;
	s16		window;

	s16		offset;
	s16		offset_dc;

	s16		ble_offset;
	s16		ble_offset_dc;

	u32		tick;

	//10
	u32		d0;
	u32		d1;
	u32		d2;
	u32		d3;

} schedule_low_power_para;

extern schedule_low_power_para lp_ble;


static inline void	bt_ll_schedule_low_power_exit (void)
{
	u32 r = core_disable_interrupt ();

	if (env_sch.lp_en & SCHEDULER_LOWPOWER_BUSY)
	{
		env_sch.lp_en |= SCHEDULER_LOWPOWER_EXIT;
	}

	core_restore_interrupt (r);
}


void bt_ll_irq_priority_store(void);
void bt_ll_irq_priority_restore(void);







#define LP_TYPE_SNIFF				0
#define LP_TYPE_GENERAL				1


#define LP_BT_EN					BIT(0)
#define LP_BLE_EN					BIT(1)


#define PM_CALIB_IDLE				0xFF


#define WKPTASK_INVALID				0xFF

_attribute_aligned_(4)
typedef struct {
	u8		lp_bt_en;
	u8		lp_ble_en;
	u8		lp_forbidden;
	u8		sleep_type_sel;  //sleep type selected by user

	u8		wakeupSrc_set;
	u8		pm_calib_idx;
	u8		wkpTsk_idx;
	u8      appWakeup_en;

	u8		appWakeup_flg;
	u8		u8_rsvd[3];

	u16		sleep_state_cfg;
	u16		u16_rsvd; //for 4B alignment

	u32		cur_wakeup_tick;
	u32     appWakeup_tick;
//	u32    	deepRet_earlyWakeupTick;  //do later

}bb_ll_pm_t;  //baseband linklayer power management data
extern bb_ll_pm_t  btblePm;




typedef 	void (*pm_module_callback_t)(void);
extern pm_module_callback_t  pm_module_cb;




/* for BLE PM module */
#define LP_TYPE_ENTER_CHECK			0
#define LP_TYPE_GPIO_WAKEUP			1
#define LP_TYPE_BLE_RECOVER			2

typedef	int (*lp_ble_callback_t) (int);
extern lp_ble_callback_t		   lp_contr_ble_cb;







static inline void btble_pm_setStateEnable(u16 state_msk)
{
	btble_stk.ctlr_state_en |= state_msk;
}


static inline void btble_pm_setStateDisable(u16 state_msk)
{
	btble_stk.ctlr_state_en &= ~state_msk;
}

static inline void btble_pm_setTaskBusy(u16 state_msk)
{
	btble_stk.ctlr_state_busy |= state_msk;

	//debug, remove later
	if(state_msk == STATEMSK_BB_ACL_SALVE){
		DBG_CHN7_HIGH;
	}
}


static inline void btble_pm_setTaskIdle(u16 state_msk)
{
	/* special process: remove BIT(15) by the way */
	btble_stk.ctlr_state_busy &= ~(state_msk | STATE_PENDING_FLAG);

	//debug, remove later
	if(state_msk == STATEMSK_BB_ACL_SALVE){
		DBG_CHN7_LOW;
	}
}


void bt_sniff_process(void);
int  btble_low_power_enter (void);
void btble_low_power_task (void);

void bt_lowpower_mainloop(void);
void bt_scan_low_power_process(uint8_t is_inq_scan);


void	bt_ll_schedule_low_power_enable (int en);
void	bt_ll_schedule_set_bt_low_power_link (int link, int en, u32 interval_us);
void	bt_ll_schedule_set_ble_low_power_link (int link, int en);

static inline int bt_ll_schedule_low_power_busy (void)
{
	return (env_sch.lp_en & SCHEDULER_LOWPOWER_BUSY);
}

/******************************* btble_pm end   ******************************************************************/


#endif /* STACK_BTBLE_BTBLE_STACK_H_ */
