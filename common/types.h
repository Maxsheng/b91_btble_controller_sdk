/********************************************************************************************************
 * @file     types.h
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

#ifndef TYPES_H_
#define TYPES_H_


#ifndef NULL
#define NULL  0
#endif

typedef unsigned char  u8  ;
typedef unsigned short u16 ;
typedef unsigned int   u32 ;
typedef signed char    s8  ;
typedef signed short   s16 ;
typedef signed int     s32 ;

typedef long long s64;
typedef unsigned long long u64;

typedef unsigned char  uint8_t  ;
typedef unsigned short uint16_t ;
//typedef unsigned int   uint32_t ;
//typedef signed char    int8_t   ;
typedef signed short   int16_t  ;
//typedef signed int     int32_t  ;
typedef unsigned long uint32_t;

#if 0
typedef enum
{
	false,
	true
} bool;
#endif

typedef struct ke_msg {
    u8 msg_id;
} ke_msg_t;


#ifdef __GNUC__
typedef	u16	wchar_t;
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE     0
#endif

#define U32_MAX ((u32)0xffffffff)
#define U16_MAX ((u16)0xffff)
#define U8_MAX ((u8)0xff)


#endif /* TYPES_H_ */

