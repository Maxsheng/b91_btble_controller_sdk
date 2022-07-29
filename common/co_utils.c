/********************************************************************************************************
 * @file     co_utils.c
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

/**
****************************************************************************************
*
* @file co_utils.c
*
* @brief Common Utility functions
*
* Copyright (C) RivieraWaves 2009-2015
*
*
****************************************************************************************
*/

/**
****************************************************************************************
* @addtogroup  CO_UTILS
* @{
****************************************************************************************
*/

/*
 * INCLUDE FILES
 ****************************************************************************************
 */



#include <string.h>           // for mem* functions
#include "co_utils.h"
#include "incs.h"            // common bt definitions




/*
 * DEFINES
 ****************************************************************************************
 */

///Default BT address (if none defined in NVDS)
#define BT_DEFAULT_BDADDR  {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB}}

#define CPU_LE          1

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */


/*
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */

/// Number of '1' bits in values from 0 to 15, used to fasten bit counting
const unsigned char one_bits[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

/// SCA to PPM
const uint16_t co_sca2ppm[] =
{
    [SCA_500PPM] = 500,
    [SCA_250PPM] = 250,
    [SCA_150PPM] = 150,
    [SCA_100PPM] = 100,
    [SCA_75PPM] = 75,
    [SCA_50PPM] = 50,
    [SCA_30PPM] = 30,
    [SCA_20PPM] = 20
};

/// NULL BD address
const struct bd_addr co_null_bdaddr = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

/// Default BD address
const struct bd_addr co_default_bdaddr = BT_DEFAULT_BDADDR;

/// Table for converting rate to PHY
const uint8_t co_rate_to_phy[] =
{
    [CO_RATE_1MBPS  ]  = PHY_1MBPS_VALUE  ,
    [CO_RATE_2MBPS  ]  = PHY_2MBPS_VALUE  ,
    [CO_RATE_125KBPS]  = PHY_CODED_VALUE  ,
    [CO_RATE_500KBPS]  = PHY_CODED_VALUE  ,
    [CO_RATE_UNDEF]    = PHY_UNDEF_VALUE  ,
};

/// Table for converting PHY to rate (Warning: the coded PHY is converted to 500Kbps by default)
const uint8_t co_phy_to_rate[] =
{
    [PHY_1MBPS_VALUE]  = CO_RATE_1MBPS   ,
    [PHY_2MBPS_VALUE]  = CO_RATE_2MBPS   ,
    [PHY_CODED_VALUE]  = CO_RATE_500KBPS ,
};

/// Convert PHY mask (with one single bit set) to a value
const uint8_t co_phy_mask_to_value[] =
{
    [PHY_1MBPS_BIT] = PHY_1MBPS_VALUE,
    [PHY_2MBPS_BIT] = PHY_2MBPS_VALUE,
    [PHY_CODED_BIT] = PHY_CODED_VALUE,
};

/// Convert PHY a value to the corresponding mask bit
const uint8_t co_phy_value_to_mask[] =
{
    [PHY_1MBPS_VALUE] = PHY_1MBPS_BIT,
    [PHY_2MBPS_VALUE] = PHY_2MBPS_BIT,
    [PHY_CODED_VALUE] = PHY_CODED_BIT,
};

/// Convert Rate value to the corresponding PHY mask bit
const uint8_t co_rate_to_phy_mask[] =
{
    [CO_RATE_1MBPS  ]  = PHY_1MBPS_BIT  ,
    [CO_RATE_2MBPS  ]  = PHY_2MBPS_BIT  ,
    [CO_RATE_125KBPS]  = PHY_CODED_BIT  ,
    [CO_RATE_500KBPS]  = PHY_CODED_BIT  ,
};

const uint8_t co_rate_to_byte_dur_us[] =
{
    [CO_RATE_1MBPS  ]  = 8,
    [CO_RATE_2MBPS  ]  = 4,
    [CO_RATE_125KBPS]  = 16,
    [CO_RATE_500KBPS]  = 64,
    [CO_RATE_UNDEF]    = 0,
};

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/// Extract length of an array from a format string
static uint16_t co_util_read_array_size(char **fmt_cursor)
{
    // Read size
    uint16_t size = 0;

    // Sanity check
    ASSERT_ERR(fmt_cursor);

    // Convert unit
    size = (*(*fmt_cursor)++) - '0';

    while(((*(*fmt_cursor)) >= '0') && ((*(*fmt_cursor)) <= '9'))
    {
        // Convert tens
        size = 10 * size + ((*(*fmt_cursor)++) - '0');
    }

    // Return the read size
    return (size);
}

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if (RW_DEBUG || DISPLAY_SUPPORT)
void co_bytes_to_string(char* dest, uint8_t* src, uint8_t nb_bytes)
{
    for(int i = 0 ; i < nb_bytes ; i++)
    {
        char digit;
        uint8_t byte = *(src + nb_bytes - 1 - i);

        // MSbs
        digit = (byte & 0xF0) >> 4;
        digit += (digit < 10) ? 48:55;
        *(dest+2*i) = (char)digit;

        // LSbs
        digit = (byte & 0x0F) >> 0;
        digit += (digit < 10) ? 48:55;
        *(dest+2*i+1) = (char)digit;
    }
}
#endif //(RW_DEBUG || DISPLAY_SUPPORT)

bool co_bdaddr_compare(struct bd_addr const *bd_address1, struct bd_addr const *bd_address2)
{

    for (uint8_t idx=0; idx < BD_ADDR_LEN; idx++)
    {
        /// checks if the addresses are similar
        if (bd_address1->addr[idx] != bd_address2->addr[idx])
        {
           return (false);
        }
    }
    return (true);
}

#if (BLE_EMB_PRESENT)
uint8_t co_nb_good_le_channels(const struct le_chnl_map* map)
{
    uint8_t nb_good_channels = 0;

    // Count number of good channels
    for(int i = (LE_CHNL_MAP_LEN-1) ; i >= 0 ; i--)
    {
        uint8_t byte = map->map[i];
        nb_good_channels += NB_ONE_BITS(byte);
    }

    return nb_good_channels;
}
#endif //BLE_EMB_PRESENT

#if (BT_EMB_PRESENT)
uint32_t co_slot_to_duration(uint32_t slot_cnt)
{
    return co_max(slot_cnt >> 4 , 1);
}

uint8_t co_nb_good_channels(const struct chnl_map* map)
{
    uint8_t nb_good_channels = 0;

    // Count number of good channels
    for(int i = (CHNL_MAP_LEN-1) ; i >= 0 ; i--)
    {
        uint8_t byte = map->map[i];
        nb_good_channels += NB_ONE_BITS(byte);
    }

    return nb_good_channels;
}
#endif //BT_EMB_PRESENT

uint8_t co_util_pack(uint8_t* out, uint8_t* in, uint16_t* out_len, uint16_t in_len, const char* format)
{
    uint8_t status = CO_UTIL_PACK_OK;
    uint8_t* p_in = in;
    uint8_t* p_out = out;
    uint8_t* p_in_end = in + in_len;
    char* cursor = (char*) format;
    bool b_copy = (in != NULL) && (out != NULL);

    ASSERT_ERR(format != NULL);

    bool b_lsb = true;
    bool big_number = false;
    uint16_t nb = 1;

    // Check if forced to little endian
    if(*cursor == '<')
    {
        b_lsb = true;
        cursor++;
    }
    else if(*cursor == '>')
    {
        b_lsb = false;
        cursor++;
    }

    while((*cursor != '\0') && (status == CO_UTIL_PACK_OK))
    {
        char data_type;

        // Check if the new field is an array (starting with a number)
        if((*cursor >= '0') && (*cursor <= '9'))
        {
            nb = co_util_read_array_size(&cursor);
        }

        data_type = *cursor++;

        // Parse the format string
        switch (data_type)
        {
            case ('G'): // Big Number
            {
                big_number = true;
            } // No break
            case ('n'): // table size over 1 byte
            case ('B'): // Byte
            {
                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if((p_in + nb) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Copy bytes
                    if(!big_number || (b_lsb == CPU_LE))
                    {
                        memmove(p_out, p_in, nb);
                    }
                    // Swap bytes
                    else
                    {
                        co_bswap(p_out, p_in, nb);
                    }
                }

                // Move pointers
                p_out += nb;
                p_in += nb;

                // Re-initialize table size for next loop
                nb = 1;
                if(data_type == 'n')
                {
                    // The current byte represents the size of the following table
                    nb = *(p_in - 1);
                }

                big_number = false;
            }
            break;

            case ('N'): // table size over 2 byte
            case ('H'): // Short Word
            {
                uint16_t *short_word = NULL;
                uint8_t i;

                // loop for all short numbers
                for(i = 0 ; i < nb ; i++)
                {
                    // Align data buffer to a 16-bits address
                    short_word = (uint16_t *)CO_ALIGN2_HI((uint32_t)p_in);

                    if(b_copy)
                    {
                        // Check if enough space in input buffer to read
                        if(((uint8_t *)(short_word + 1)) > p_in_end)
                        {
                            status = CO_UTIL_PACK_IN_BUF_OVFLW;
                            break;
                        }

                        // Copy data
                        if(b_lsb)
                        {
                            co_write16p(p_out, co_htobs(*short_word));
                        }
                        else
                        {
                            co_write16p(p_out, co_htons(*short_word));
                        }
                    }

                    // Move pointers
                    p_in = (uint8_t *)(short_word + 1);
                    p_out += 2;
                }

                // Re-initialize table size for next loop
                nb = 1;
                if(data_type == 'N')
                {
                    // The current word represents the size of the following table
                    nb = *short_word;
                }
            }
            break;

            case ('D'): // 24 bits integer
            {
                // Align data buffer to a 32-bits address
                uint32_t *long_word = (uint32_t *)CO_ALIGN4_HI((uint32_t)p_in);

                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if(((uint8_t *)(long_word + 1)) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Copy data
                    if(b_lsb)
                    {
                        co_write24p(p_out, co_htob24(*long_word));
                    }
                    else
                    {
                        co_write24p(p_out, co_hton24(*long_word));
                    }
                }

                // Move pointers
                p_in = (uint8_t *)(long_word + 1);
                p_out += 3;
            }
            break;

            case ('L'): // Long Word
            {
                // Align data buffer to a 32-bits address
                uint32_t *long_word = (uint32_t *)CO_ALIGN4_HI((uint32_t)p_in);

                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if(((uint8_t *)(long_word + 1)) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Copy data
                    if(b_lsb)
                    {
                        co_write32p(p_out, co_htobl(*long_word));
                    }
                    else
                    {
                        co_write32p(p_out, co_htonl(*long_word));
                    }
                }

                // Move pointers
                p_in = (uint8_t *)(long_word + 1);
                p_out += 4;
            }
            break;

            default:
            {
                // data format error
                status = CO_UTIL_PACK_WRONG_FORMAT;
            }
            break;
        }
    }

    if(status == CO_UTIL_PACK_OK)
    {
        *out_len = (uint16_t)(p_out - out);
    }

    return (status);
}

uint8_t co_util_unpack(uint8_t* out, uint8_t* in, uint16_t* out_len, uint16_t in_len, const char* format)
{
    uint8_t status = CO_UTIL_PACK_OK;
    uint8_t* p_in = in;
    uint8_t* p_out = out;
    uint8_t* p_in_end = in + in_len;
    uint8_t* p_out_end = out + *out_len;
    char* cursor = (char*) format;
    bool b_copy = ((out != NULL) && (in != NULL));

    bool b_lsb = true;
    bool big_number = false;
    uint16_t nb = 1;

    // Check if forced to little endian
    if(*cursor == '<')
    {
        b_lsb = true;
        cursor++;
    }
    else if(*cursor == '>')
    {
        b_lsb = false;
        cursor++;
    }

    ASSERT_ERR(format != NULL);

    while((*cursor != '\0') && (status == CO_UTIL_PACK_OK))
    {
        char data_type;

        // Check if the new field is a fixed size array (starting with a number)
        if((*cursor >= '0') && (*cursor <= '9'))
        {
            nb = co_util_read_array_size(&cursor);
        }

        data_type = *cursor++;

        // Parse the format string
        switch (data_type)
        {
            case ('G'): // Big Number
            {
                big_number = true;
            } // No break
            case ('n'): // table size over 1 byte
            case ('B'): // Byte
            {
                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if((p_in + nb) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Check if enough space in out buffer to write
                    if((p_out + nb) > p_out_end)
                    {
                        status = CO_UTIL_PACK_OUT_BUF_OVFLW;
                        break;
                    }

                    // Copy bytes
                    if(!big_number || (b_lsb == CPU_LE))
                    {
                        memmove(p_out, p_in, nb);
                    }
                    // Swap bytes
                    else
                    {
                        co_bswap(p_out, p_in, nb);
                    }
                }

                // Move pointers
                p_out += nb;
                p_in += nb;

                // Re-initialize table size for next loop
                nb = 1;
                if(data_type == 'n')
                {
                    // The current byte represents the size of the following table
                    nb = *(p_in - 1);
                }

                big_number = false;
            }
            break;

            case ('N'): // table size over 2 byte
            case ('H'): // Short Word
            {
                uint16_t *short_word = NULL;
                uint8_t i;
                // loop for all short numbers
                for(i = 0 ; i < nb ; i++)
                {
                    // Align data buffer to a 16-bits address
                    short_word = (uint16_t *)CO_ALIGN2_HI((uint32_t)p_out);

                    if(b_copy)
                    {
                        // Check if enough space in input buffer to read
                        if((p_in + 2) > p_in_end)
                        {
                            status = CO_UTIL_PACK_IN_BUF_OVFLW;
                            break;
                        }

                        // Check if enough space in out buffer to write
                        if(((uint8_t *)(short_word + 1)) > p_out_end)
                        {
                            status = CO_UTIL_PACK_OUT_BUF_OVFLW;
                            break;
                        }

                        // Copy data
                        if(b_lsb)
                        {
                            *short_word = co_btohs(co_read16p(p_in));
                        }
                        else
                        {
                            *short_word = co_ntohs(co_read16p(p_in));
                        }
                    }

                    // Move pointers
                    p_out = (uint8_t *)(short_word + 1);
                    p_in += 2;
                }

                // Re-initialize table size for next loop
                nb = 1;
                if(data_type == 'N')
                {
                    nb = *short_word;
                }
            }
            break;

            case ('D'): // 24 bits integer
            {
                // Align data buffer to a 32-bits address
                uint32_t *long_word = (uint32_t *)CO_ALIGN4_HI((uint32_t)p_out);

                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if((p_in + 3) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Check if enough space in out buffer to write
                    if(((uint8_t *)(long_word + 1)) > p_out_end)
                    {
                        status = CO_UTIL_PACK_OUT_BUF_OVFLW;
                        break;
                    }

                    // Copy data
                    if(b_lsb)
                    {
                        *long_word = co_btoh24(co_read24p(p_in));
                    }
                    else
                    {
                        *long_word = co_ntoh24(co_read24p(p_in));
                    }
                }

                // Move pointers
                p_out = (uint8_t *)(long_word + 1);
                p_in += 3;
            }
            break;

            case ('L'): // Long Word
            {
                // Align data buffer to a 32-bits address
                uint32_t *long_word = (uint32_t *)CO_ALIGN4_HI((uint32_t)p_out);

                if(b_copy)
                {
                    // Check if enough space in input buffer to read
                    if((p_in + 4) > p_in_end)
                    {
                        status = CO_UTIL_PACK_IN_BUF_OVFLW;
                        break;
                    }

                    // Check if enough space in out buffer to write
                    if(((uint8_t *)(long_word + 1)) > p_out_end)
                    {
                        status = CO_UTIL_PACK_OUT_BUF_OVFLW;
                        break;
                    }

                    // Copy data
                    if(b_lsb)
                    {
                        *long_word = co_btohl(co_read32p(p_in));
                    }
                    else
                    {
                        *long_word = co_ntohl(co_read32p(p_in));
                    }
                }

                // Move pointers
                p_out = (uint8_t *)(long_word + 1);
                p_in += 4;
            }
            break;

            default:
            {
                // data format error
                status = CO_UTIL_PACK_WRONG_FORMAT;
            }
            break;
        }
    }

    // Check a potential mismatch between the theoretical (measured) input length and the given input length
    if(p_in > p_in_end)
    {
        status = CO_UTIL_PACK_IN_BUF_OVFLW;
    }

    // Return the total size needed for unpacked parameters
    *out_len = (uint16_t)(p_out - out);

    return (status);
}

#if  (BLE_EMB_PRESENT)
uint16_t co_ble_pkt_dur_in_us(uint8_t len, uint8_t rate)
{
    uint16_t pdu_len_us = 0;

    switch (rate)
    {
        case CO_RATE_1MBPS:
        {
            pdu_len_us = PDU_1MBPS_LEN_US(len);
        }
        break;
        case CO_RATE_2MBPS:
        {
            pdu_len_us = PDU_2MBPS_LEN_US(len);
        }
        break;
        case CO_RATE_125KBPS:
        {
            pdu_len_us = PDU_125KBPS_LEN_US(len);
        }
        break;
        case CO_RATE_500KBPS:
        {
            pdu_len_us = PDU_500KBPS_LEN_US(len);
        }
        break;
        default:
        {
            ASSERT_INFO(0, len, rate);
        }
        break;
    }

    return pdu_len_us;
}
#endif // (BLE_EMB_PRESENT)

/// @} CO_UTILS
