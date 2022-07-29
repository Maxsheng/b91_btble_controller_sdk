/********************************************************************************************************
 * @file     bt_spcomputation.h
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

/**
****************************************************************************************
*
* @file lm_spcomputation.h
*
* @brief Link Manager simple pairing
*
* Copyright (C) OpenC 2009-2013
*
*
****************************************************************************************
*/
#ifndef LM_SPCOMPUTATION_H_
#define LM_SPCOMPUTATION_H_
/**
****************************************************************************************
* @defgroup LMSPCOMPUTATION SP Computation
* @ingroup LM
* @brief LM module for simple pairing algorithms and mathematical computations.
* @{
****************************************************************************************
*/
#include "types.h"
#include "co_bt_defines.h"
/*
 * DEFINITION
 * ***************************************************************************************
 */
#define LM_SP_WORD_BITS4        16

#define LM_SP_WORD_MASK2        0xffffffff
#define LM_SP_WORD_MASK2l       0x0000ffff
#define LM_SP_WORD_MASK2h1      0xffff8000
#define LM_SP_WORD_MASK2h       0xffff0000
#define LM_SP_WORD_TBIT         0x80000000

#define LM_SPLBITS(a)           ((a) & LM_SP_WORD_MASK2l)
#define LM_SPHBITS(a)           (((a) >> LM_SP_WORD_BITS4)& LM_SP_WORD_MASK2l)
#define LM_SPL2HBITS(a)         (((a) << LM_SP_WORD_BITS4)& LM_SP_WORD_MASK2)

#define LM_SP_NUM_192_SIGN_BIT  0x80000000
#define LM_SP_NUM_192_BYTES     0x06
#define LM_SP_NUM_192_BITS      32

#define LM_SP_TYPE_NUM_192      1
#define LM_SP_TYPE_NUM_384      2

#define LM_SP_WNAF_WINDOW           0x03
#define LM_SP_WNAF_MSB_BIT          (1<<LM_SP_WNAF_WINDOW)
#define LM_SP_WNAF_NEXT_DIGIT_BIT   (LM_SP_WNAF_MSB_BIT<<1)
#define LM_SP_WNAF_DIGIT_MASK       (LM_SP_WNAF_NEXT_DIGIT_BIT-1)

#define LM_SP_P192_PRE_COMP_POINTS  4

/*
 * NEW TYPE
 * ***************************************************************************************
 */
/// Simple pairing 192
typedef struct _lm_sp_num_192_
{
    uint32_t Neg;
    uint32_t Number[6];
}
lm_sp_num_192;

/// Simple pairing 384
typedef struct _lm_sp_num_384_
{
    uint32_t Neg;
    uint32_t Number[12];
}
lm_sp_num_384;

/// Simple pairing P192
typedef struct _lm_sp_p192_point_
{
    lm_sp_num_192 X;
    lm_sp_num_192 Y;
    lm_sp_num_192 Z;
}
lm_sp_p192_point;

/// Simple pairing P256
typedef struct _lm_sp_p256_point_
{
    uint8_t X[32];
    uint8_t Y[32];
}
lm_sp_p256_point;

/// P-192 curve data
typedef struct _lm_sp_p192_group_data_
{
    lm_sp_p192_point    G;
    lm_sp_num_192       A;
}
lm_sp_p192_group_data;

/// Simple pairing P192 Computation Global Data
typedef struct _lm_sp_p192_comp_global_data_
{
	uint8_t     Pre_comp[LM_SP_P192_PRE_COMP_POINTS][LMP_ENCAPS_P192_PAYL_LEN];
    uint8_t     RemotePublicKey[LMP_ENCAPS_P192_PAYL_LEN];
    uint8_t     check_RemotePublicKey[LMP_ENCAPS_P192_PAYL_LEN];
    uint8_t     DH_Key[LMP_ENCAPS_P192_PAYL_LEN];
    signed char          wnaf[196];
    uint32_t             wnaf_len;
    uint8_t              Loop_ctr;
    uint8_t              Result_at_infinity;
    uint8_t              Result_inverted;
}
lm_sp_p192_comp_global_data;

/// Simple pairing P256 Computation Global Data
typedef struct _lm_sp_p256_comp_global_data_
{
	uint8_t     RemotePublicKey[LMP_ENCAPS_P256_PAYL_LEN];
	uint8_t     DH_Key[LMP_ENCAPS_P256_PAYL_LEN];
}
lm_sp_p256_comp_global_data;
/*
 * FUNCTION DECLARATION
 * ***************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief This function calculates the SHA hash for the given input block.
 *
 * @param[in] Input_Message    The pointer to the message block for which hash is to be calculated
 * @param[in] SHA_256_Hash     Hash Value calculated
 * @param[in] Length           Length of the Input block
 ****************************************************************************************
 */
void lm_sp_sha256_calculate(uint8_t* Input_Message, uint32_t* SHA_256_Hash, uint32_t Length);

/**
 ****************************************************************************************
 * @brief This function checks if the input is zero
 *
 * @param[in] a      The input
 * @param[in] Type   Type of the input
 *
 * @return non zero if number is zero else returns 0
 *
 ****************************************************************************************
 */
int lm_n_is_zero(uint8_t* a, uint32_t type);

/**
 ****************************************************************************************
 * @brief This function stores the one on a given number.
 *
 * @param[in] a     The input
 * @param[in] Type  Type of the input
 ****************************************************************************************
 */
void lm_sp_n_one(void* a, uint32_t type);

/**
 ****************************************************************************************
 * @brief This function returns converts the number into naf form with window
 *        size of 4
 *
 * @param[in] wnaf     returns wnaf form
 * @param[in] input    the input num to be converted
 * @param[in] ret_len  the length of the wnaf form number
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_n192_convert_wnaf(signed char *r, lm_sp_num_192 *scalar, uint32_t *ret_len);

/**
 ****************************************************************************************
 * @brief This function sets the input point to infinity
 *
 * @param[in] p     Input
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_point_to_inf(lm_sp_p192_point *p);

/**
 ****************************************************************************************
 * @brief This function converts the coordinate system from jacobian to affine
 *
 * @param[in] p      Input
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_point_jacobian_to_affine(lm_sp_p192_point *points);

/**
 ****************************************************************************************
 * @brief This function is used to convert the points from jacobian to affine
 *
 * @param[in] point  The array of points to be converted from Jacobian to affine
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_points_jacobian_to_affine(lm_sp_p192_point *points);

/**
 ****************************************************************************************
 * @brief This function is used to precompute the points needed for scalar mul
 *
 * @param[in] r     The array for P192 pre computation
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_pre_compute_points(lm_sp_p192_point *r);

/**
 ****************************************************************************************
 * @brief This function doubles the given p192 point
 *
 * @param[in] r     The result after point doubling
 * @param[in] a     The point to be doubled
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_dbl(lm_sp_p192_point *r, lm_sp_p192_point *a);

/**
 ****************************************************************************************
 * @brief This function adds two  p192 point
 *
 * @param[in] r     The result after point addition
 * @param[in] a     The point to be added
 * @param[in] b     The point to be added
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_add(lm_sp_p192_point *r, lm_sp_p192_point *a, lm_sp_p192_point *b);

/**
 ****************************************************************************************
 * @brief This function inverts the input p192 point
 *
 * @param[in] p    Input
 *
 * @return non zero on success and vice versa
 *
 ****************************************************************************************
 */
int lm_sp_p192_invert(lm_sp_p192_point *p);

///@} LMSPCOMPUTATION

#endif // LM_SPCOMPUTATION_H_
