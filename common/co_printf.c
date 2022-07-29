/********************************************************************************************************
 * @file     co_printf.c
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

#if 0
#include <stdarg.h>



#include "drivers/driver.h"
#include "common/utility.h"
#include "stack/stack.h"
//#include "inc_stack.h"
//#include "host/l2cap_api.h"
//#include "host/l2cap.h"
#include "drivers/caracara/bt/btm_security.h"
#include "drivers/caracara/bt/btlib_type.h"
//#include "host/sdp.h"
//#include "host/avdtp.h"
#include "drivers/caracara/bt/a2dp_api.h"
#include "drivers/caracara/bt/a2dp.h"
#endif

#include "co_printf.h"
#include <stdarg.h>
#include "../drivers/B91/uart.h"
#include "../vendor/common/user_config.h"

static void printchar(char **str, int c)
{
	uint8_t print_buff[1];
	if (str) {
		**str = c;
		++(*str);
	}
//	else putc((char)c);
	else {
		print_buff[0]  =(uint8_t)c;
		if(1)
		{
#if  UART_DEBUG_EN
        uart_send_byte(UART1, c);
#elif NO_TEST_HCI_MODE
				//uart1_write(print_buff, 1, NULL);
#else
				//uart0_write(print_buff, 1, NULL);
#endif
		}
	}

	if (print_buff[0])
	{

	}
}


#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;

		for (ptr = string; *ptr; ++ptr) {
			++len;
		}

		if (len >= width) {
			width = 0;
		}
		else {
			width -= len;
		}

		if (pad & PAD_ZERO) {
			padchar = '0';
		}
	}

	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}

	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}

	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;

		if( t >= 10 ) {
			t += letbase - '0' - 10;
		}

		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int print(char **out, const char *format, va_list args )
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;

			if (*format == '\0') {
				break;
			}

			if (*format == '%') {
				goto out;
			}

			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}

			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}

			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}

			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}

			if( *format == 'd' ) {
				pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
				continue;
			}

			if( *format == 'x' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
				continue;
			}

			if( *format == 'X' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
				continue;
			}

			if( *format == 'u' ) {
				pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
				continue;
			}

			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
out:
			printchar (out, *format);
			++pc;
		}
	}

	if (out) {
		**out = '\0';
	}

	va_end( args );
	return pc;
}

int co_printf(const char *format, ...)
{
	va_list args;
	if(1)
	{
		va_start( args, format );
		return print( 0, format, args );
	}
}

int co_sprintf(char *out, const char *format, ...)
{
	va_list args;

	va_start( args, format );
	return print( &out, format, args );
}


int co_strlen( const char *s )
{
	register char *cp;
	cp = (char *)s;

	while (*cp) {
		++cp;
	}

	return (int)(cp - s);
}

int co_memcmp( const void *s1, const void *s2, uint32_t n )

{
	register int result;
	register unsigned char *cp1, *cp2;
	cp1 = (unsigned char *)s1;
	cp2 = (unsigned char *)s2;

	while (n-- > 0) {
		result = *cp1++ - *cp2++;

		if (result) {
			return result;
		}
	}

	return 0;
}





/*no need in ARM ads, because it default have memcpy enabled*/
void *co_memcpy(void *dst, const void *src, uint32_t n)
{

	register unsigned char *cdst, *csrc;
	cdst = (unsigned char *)dst;
	csrc = (unsigned char *)src;

	while (n-- > 0) {
		*cdst++ = *csrc++;
	}

	return dst;

}

void *co_memset( void *s, int c, uint32_t n)
{
	unsigned char *c_from = s;
	unsigned char uV = (unsigned char )c;

	while(n-- >0) {
		*c_from++ = uV;
	}

	return s;

}

char *co_strncat( char *dst, const char *src, uint32_t n )
{
	register char *cp;
	cp = dst;

	while (*cp) {
		++cp;
	}

	while (*src) {
		if (n-- == 0) {
			break;
		}

		*cp++ = *src++;
	}

	*cp = '\0';

	return dst;
}



char *co_strcpy( char *dst, const char *src )
{
	register char *cp;
	cp = dst;

	do {
		*cp++ = *src;
	}
	while (*src++);

	return dst;
}



char *co_strncpy( char *dst, const char *src, uint32_t n )
{
	register char *cp;
	cp = dst;

	do {
		if (n-- == 0) {
			return dst;
		}		*cp++ = *src;
	}
	while (*src++);

	while (n-- > 0) {
		*cp++ = 0;
	}

	return dst;
}

int co_strncmp( const char *s1, const char *s2, uint32_t n )
{
	register int result = 0;

	while (*s1) {
		if (n-- == 0) {
			return result;
		}

		result = (unsigned char)*s1++ - (unsigned char)*s2++;

		if (result) {
			return result;
		}
	}

	if (n != 0) {
		return (unsigned char)*s1 - (unsigned char)*s2;
	}
	else {
		return 0;
	}
}


char *co_strstr(const char *s1, const char *s2)
{

	register const char *s = s1;
	register const char *p = s2;

	do {
		if (!*p) {
			return (char *) s1;
		}
		if (*p == *s) {
			++p;
			++s;
		} else {
			p = s2;
			if (!*s) {
				return NULL;
			}
			s = ++s1;
		}
	} while (1);



}



/*-------------------------------------------------------------------------
    Function    :  ascii_char2val             xxx
    Return: -1=error
    Description:
        'a' -> 0xa  'A' -> 0xa
-------------------------------------------------------------------------*/
char ascii_char2val(const  char c)
{
    if(c>='0' && c<='9')
        return c-'0';
    if((c>='a' && c<='f') || (c>='A' && c<='F'))
        return (c&0x7)+9;

    return (char)(-1);
}

/*-------------------------------------------------------------------------
    Function    :  ascii_str2val             xxx
    Return: -1=error
    Description:
        str = "123" bas = 10 return 123
        str = "123" bas = 16 return 0x123        
-------------------------------------------------------------------------*/
int ascii_str2val( const  char str[],  char base)
{
    int val = 0;
    signed char v;
    while(*str != 0){

        v = ascii_char2val(*str);

#if 0
        if (v == -1 || v >= base) 
            return -1;
#else
        if (v == -1 || v >= base)
        {
            if(val == 0) //to filter abormal beginning and ending
            {
                str ++;
                continue;
            }
            else
            {
                break;
            }
        }
#endif

        val = val*base + v;
        str++;
    }
    return val;
}

/*-------------------------------------------------------------------------
    Function    :  ascii_strn2val             xxx
    Return: -1=error
    Description:
        str = "123" bas = 10 return 123
        str = "123" bas = 16 return 0x123        
-------------------------------------------------------------------------*/
int ascii_strn2val( const  char str[],  char base,  char n)
{
    int val = 0;
    signed char v;
    while(n != 0){

        v = ascii_char2val(*str);
#if 0
        if (v == -1 || v >= base) 
            return -1;
#else
        if (v == -1 || v >= base)
        {
            if(val == 0) //to filter abormal beginning and ending
            {
                str ++;
                n --;
                continue;
            }
            else
            {
                break;
            }
        }
#endif

        val = val*base + v;
        
        str++;
        n--;
    }
    return val;
}


