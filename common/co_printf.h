/********************************************************************************************************
 * @file     co_printf.h
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

#ifndef _CO_PRINTF_H
#define _CO_PRINTF_H

#if 1

#include <string.h>        // string
#include <stddef.h>        // standard
#include <stdint.h>        // integer
#include <stdbool.h>       // boolean

int co_printf(const char *format, ...);

int co_sprintf(char *out, const char *format, ...);

int co_strlen( const char *s );

char *co_strstr(const char *s1, const char *s2);

char *co_strcpy( char *dst, const char *src );

char *co_strncat( char *dst, const char *src, uint32_t n );

int co_strncmp( const char *s1, const char *s2, uint32_t n );

int co_memcmp( const void *s1, const void *s2, uint32_t n );

void *co_memset( void *s, int c, uint32_t n);

char *co_strncpy( char *dst, const char *src, uint32_t n );

int ascii_strn2val( const  char str[],  char base,  char n);
char ascii_char2val(const  char c);

#endif

#endif
