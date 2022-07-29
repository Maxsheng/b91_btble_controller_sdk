/********************************************************************************************************
 * @file     timer_event.c
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

#include "../../drivers.h"
#include "timer_event.h"
#include "stack.h"

#define LengthOfArray(arr_name) (sizeof(arr_name)/sizeof(arr_name[0]))
#define TIMER_SAFE_MARGIN     (1<<31)

#define TIMER_LIST_LEN_MAX    (32)
ev_timer_event_t timer_list[TIMER_LIST_LEN_MAX];

ev_timer_event_t *event_free_list;

ev_timer_event_t *timer_skip_cur_run=NULL;

void timer_event_init(void)
{
	unsigned char i;

    for (i = 0; i < (TIMER_LIST_LEN_MAX - 1); i++) {  /* Init. list of free timer_list            */
        timer_list[i].next = &timer_list[i+1];
    }

    timer_list[i].next = NULL;                        /* Last ev_timer_event_t        */
         
    GLOBAL_INT_DISABLE();                     

    event_free_list = timer_list;
    
    timer_skip_cur_run = NULL;

    GLOBAL_INT_RESTORE();
}
_attribute_ram_code_sec_noinline_ ev_timer_event_t *is_timer_event_exit(ev_timer_callback_t cb,ev_timer_list_t *event_pend_list)
{
	ev_timer_event_t *e = *event_pend_list;
	if(e == NULL) return NULL;
	while(e){
		if(e->cb == cb )
		{
		break;
		}
		e = e->next;
	}
	return e;
}
_attribute_ram_code_sec_noinline_ static int ev_is_timer_expired(ev_timer_event_t *e, unsigned int now)
{
    return ((unsigned int)(now - e->t) < TIMER_SAFE_MARGIN);
}

_attribute_ram_code_sec_noinline_ void ev_unon_timer(ev_timer_event_t *e, ev_timer_list_t *event_pend_list)
{
    if (((unsigned int)e < (unsigned int)(&timer_list[0])) || ((unsigned int)e > (unsigned int)(&timer_list[TIMER_LIST_LEN_MAX-1]))) {
        return;
    }

    ev_timer_event_t *p;
    ev_timer_event_t *pre;

    if (NULL == *event_pend_list || (timer_skip_cur_run==e)) {     /* if NULL ,return */
        return;
    }

    p = *event_pend_list;
    pre = *event_pend_list;
    while (p) {
		if (p == e)	{
            break;
        }
		pre = p;
		p = p->next;                      /* point to next event_pend_list */
    }

    GLOBAL_INT_DISABLE();

	if (p == (*event_pend_list)) {
		*event_pend_list = p->next;        /*  point to new event_pend_list	*/
	}
	else if (p == NULL) {
        GLOBAL_INT_RESTORE();
		return ;/*  error come here	*/
	}
	else {
		pre->next = p->next;              /*  pre next point p next	*/
	}
	p->next = event_free_list;  		  /*  return event_pend_list to free event_free_list	*/
	event_free_list = p;

    GLOBAL_INT_RESTORE();
}

_attribute_ram_code_sec_noinline_ static void ev_start_timer(ev_timer_event_t *e, ev_timer_list_t *event_pend_list)
{
    if (((unsigned int)e < (unsigned int)(&timer_list[0])) || ((unsigned int)e > (unsigned int)(&timer_list[TIMER_LIST_LEN_MAX-1]))) {
        return;
    }

    ev_timer_event_t *p = (*event_pend_list);
    ev_timer_event_t *pre = p;

    if (!p) {
        GLOBAL_INT_DISABLE();
        *event_pend_list = e;
        e->next = NULL;
        GLOBAL_INT_RESTORE();
    }
    else {
    	while (p && tick1_exceed_tick2(e->t, p->t)) {
            pre = p;
            p = p->next;
        }

        GLOBAL_INT_DISABLE();
        /// insert e in the tail of pending list
        if (!p) { 
            pre->next = e;
            e->next = NULL;
        }
        else {
            /// insert e in the head of pending list
            if (p == (*event_pend_list))  {
                e->next = *event_pend_list;
                *event_pend_list = e;
            }
            /// insert e in the middle of pending list
            else {
                pre->next = e;
                e->next = p;
            }
        }
        GLOBAL_INT_RESTORE();
    }
}

_attribute_ram_code_sec_noinline_ ev_timer_event_t *ev_on_timer(ev_timer_callback_t cb, void *data, unsigned int t_us, ev_timer_list_t *event_pend_list)
{
    ev_timer_event_t *e;

    if (!cb) {
        return NULL;
    }
    /*	if new timer event		*/
    e = is_timer_event_exit(cb,event_pend_list);
    if(e){
    	my_dump_str_u32s(DUMP_ACL_MSG, "ev_on_timer, overlap", e->cb,e->data, e->interval, e->next);
    	ev_unon_timer(e,event_pend_list);
    }

    e = event_free_list;

    if (e != NULL) {			/*	if no freelist		*/
        GLOBAL_INT_DISABLE();
        event_free_list = e->next;
        GLOBAL_INT_RESTORE();

        e->interval = t_us * SYSTEM_TIMER_TICK_1US;
        e->cb = cb;
        e->data = data;
        e->t = clock_time() + e->interval;
        ev_start_timer(e, event_pend_list);   /*	push pendlist		*/
	}

	return e;
}


/* Process time events */
void ev_process_timer(ev_timer_list_t *event_pend_list)
{
    if (NULL == *event_pend_list) {     /* if NULL ,return */
        return;
    }

	ev_timer_event_t *p = *event_pend_list;

    while (p) {
        unsigned int now = clock_time();
        if (ev_is_timer_expired(p, now) && p->cb) {

        	timer_skip_cur_run	= p;

        	int t = p->cb(p->data);

            if (t < 0) {
                GLOBAL_INT_DISABLE();

                *event_pend_list = p->next; /// delete current timer from pending list
                p->next = event_free_list; /// insert current timer back to free list
                event_free_list = p;

                GLOBAL_INT_RESTORE();
            }
            else {
                if (t > 0) {
                    p->interval = t * SYSTEM_TIMER_TICK_1US;
                }
                p->t = now + p->interval;
                
                GLOBAL_INT_DISABLE();
                *event_pend_list = p->next;                  /// delete current timer from pending list
                GLOBAL_INT_RESTORE();
                
                ev_start_timer(p, event_pend_list);         /// insert current timer in a new place in pending list
            }

            p = *event_pend_list;
            timer_skip_cur_run = NULL;
        }
        else {
            break;
        }
    }
}

