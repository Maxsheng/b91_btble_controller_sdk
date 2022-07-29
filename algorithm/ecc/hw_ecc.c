/********************************************************************************************************
 * @file     hw_ecc.c
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

#include <algorithm/ecc/hw_ecc.h>
#include "stack/ble/ble_config.h"
#include "drivers.h"


#if ( (MCU_CORE_TYPE == MCU_CORE_827x) || (MCU_CORE_TYPE == MCU_CORE_9518) )

//ECDH return code
enum ECDH_RET_CODE
{
	ECDH_SUCCESS = PKE_SUCCESS,
	ECDH_POINTOR_NULL = PKE_SUCCESS+0x60,
	ECDH_INVALID_INPUT,
	ECDH_INVALID_POINT,
	ECDH_RNG_NULL,
};

typedef enum
{
	TRNG_ERROR,
	TRNG_SUCCESS = 1,
}pke_trng_ret_code_e;


/**************************** secp192r1 ******************************/
unsigned int const secp192r1_p[6]    = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp192r1_p_h[6]  = {0x00000001,0x00000000,0x00000002,0x00000000,0x00000001,0x00000000};
unsigned int const secp192r1_p_n1[1] = {1};
unsigned int const secp192r1_a[6]    = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp192r1_b[6]    = {0xC146B9B1,0xFEB8DEEC,0x72243049,0x0FA7E9AB,0xE59C80E7,0x64210519};
unsigned int const secp192r1_Gx[6]   = {0x82FF1012,0xF4FF0AFD,0x43A18800,0x7CBF20EB,0xB03090F6,0x188DA80E};
unsigned int const secp192r1_Gy[6]   = {0x1E794811,0x73F977A1,0x6B24CDD5,0x631011ED,0xFFC8DA78,0x07192B95};
unsigned int const secp192r1_n[6]    = {0xB4D22831,0x146BC9B1,0x99DEF836,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp192r1_n_h[6]  = {0xDEB35961,0xCE66BACC,0xBB3A6BEE,0x4696EA5B,0xEA0581A2,0x28BE5677};
unsigned int const secp192r1_n_n1[1] = {0x0DDBCF2F};

const eccp_curve_t secp192r1[1]={
{
	192,
	192,
	(unsigned int *)secp192r1_p,
	(unsigned int *)secp192r1_p_h,
	(unsigned int *)secp192r1_p_n1,
	(unsigned int *)secp192r1_a,
	(unsigned int *)secp192r1_b,
	(unsigned int *)secp192r1_Gx,
	(unsigned int *)secp192r1_Gy,
	(unsigned int *)secp192r1_n,
	(unsigned int *)secp192r1_n_h,
	(unsigned int *)secp192r1_n_n1,},
};

/**************************** secp224r1 ******************************/
unsigned int const secp224r1_p[7]        = {0x00000001,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp224r1_p_h[7]      = {0x00000001,0x00000000,0x00000000,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0x00000000};
unsigned int const secp224r1_p_n1[1]     = {0xFFFFFFFF};
unsigned int const secp224r1_a[7]        = {0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp224r1_b[7]        = {0x2355FFB4,0x270B3943,0xD7BFD8BA,0x5044B0B7,0xF5413256,0x0C04B3AB,0xB4050A85};
unsigned int const secp224r1_Gx[7]       = {0x115C1D21,0x343280D6,0x56C21122,0x4A03C1D3,0x321390B9,0x6BB4BF7F,0xB70E0CBD};
unsigned int const secp224r1_Gy[7]       = {0x85007E34,0x44D58199,0x5A074764,0xCD4375A0,0x4C22DFE6,0xB5F723FB,0xBD376388};
unsigned int const secp224r1_n[7]        = {0x5C5C2A3D,0x13DD2945,0xE0B8F03E,0xFFFF16A2,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
unsigned int const secp224r1_n_h[7]      = {0x3AD01289,0x6BDAAE6C,0x97A54552,0x6AD09D91,0xB1E97961,0x1822BC47,0xD4BAA4CF};
unsigned int const secp224r1_n_n1[1]     = {0x6A1FC2EB};

const eccp_curve_t secp224r1[1]={
{
	224,
	224,
	(unsigned int *)secp224r1_p,
	(unsigned int *)secp224r1_p_h,
	(unsigned int *)secp224r1_p_n1,
	(unsigned int *)secp224r1_a,
	(unsigned int *)secp224r1_b,
	(unsigned int *)secp224r1_Gx,
	(unsigned int *)secp224r1_Gy,
	(unsigned int *)secp224r1_n,
	(unsigned int *)secp224r1_n_h,
	(unsigned int *)secp224r1_n_n1,},
};

/**************************** secp256r1 ******************************/
unsigned int const secp256r1_p[8]    = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int const secp256r1_p_h[8]  = {0x00000003,0x00000000,0xFFFFFFFF,0xFFFFFFFB,0xFFFFFFFE,0xFFFFFFFF,0xFFFFFFFD,0x00000004};
unsigned int const secp256r1_p_n1[1] = {1};
unsigned int const secp256r1_a[8]    = {0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0x00000000,0x00000001,0xFFFFFFFF};
unsigned int const secp256r1_b[8]    = {0x27D2604B,0x3BCE3C3E,0xCC53B0F6,0x651D06B0,0x769886BC,0xB3EBBD55,0xAA3A93E7,0x5AC635D8};
unsigned int const secp256r1_Gx[8]   = {0xD898C296,0xF4A13945,0x2DEB33A0,0x77037D81,0x63A440F2,0xF8BCE6E5,0xE12C4247,0x6B17D1F2};
unsigned int const secp256r1_Gy[8]   = {0x37BF51F5,0xCBB64068,0x6B315ECE,0x2BCE3357,0x7C0F9E16,0x8EE7EB4A,0xFE1A7F9B,0x4FE342E2};
unsigned int const secp256r1_n[8]    = {0xFC632551,0xF3B9CAC2,0xA7179E84,0xBCE6FAAD,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0xFFFFFFFF};
unsigned int const secp256r1_n_h[8]  = {0xBE79EEA2,0x83244C95,0x49BD6FA6,0x4699799C,0x2B6BEC59,0x2845B239,0xF3D95620,0x66E12D94};
unsigned int const secp256r1_n_n1[1] = {0xEE00BC4F};

const eccp_curve_t secp256r1[1]={
{	256,
	256,
	(unsigned int *)secp256r1_p,
	(unsigned int *)secp256r1_p_h,
	(unsigned int *)secp256r1_p_n1,
	(unsigned int *)secp256r1_a,
	(unsigned int *)secp256r1_b,
	(unsigned int *)secp256r1_Gx,
	(unsigned int *)secp256r1_Gy,
	(unsigned int *)secp256r1_n,
	(unsigned int *)secp256r1_n_h,
	(unsigned int *)secp256r1_n_n1,}
};


eccp_curve_t* blt_ecc_get_eccp_curve(ecc_curve_t curve)
{
	eccp_curve_t* pEccCurve = NULL;
	switch (curve){
//		case ECC_use_secp160r1:
//			pEccCurve = (eccp_curve_t*)secp160r1; //TODO
//		break;

		case ECC_use_secp192r1:
			pEccCurve = (eccp_curve_t*)secp192r1;
		break;

		case ECC_use_secp224r1:
			pEccCurve = (eccp_curve_t*)secp224r1;
		break;

		case ECC_use_secp256r1:
			pEccCurve = (eccp_curve_t*)secp256r1;
		break;

//		case ECC_use_secp256k1:
//			pEccCurve = (eccp_curve_t*)secp256k1; //TODO
//		break;

		default:
			pEccCurve = (eccp_curve_t*)secp256r1;
		break;
	}

	return pEccCurve;
}


static hECC_rng_func g_rng_function = NULL;

void hwECC_set_rng(hECC_rng_func rng_func) {
    g_rng_function = rng_func;
}


/**
 * @brief		This function is used to determine whether the array is all 0s.
 * @param[in]	data	- the buffer data.
 * @param[in]	len		- the length of data.
 * @return		1: all 0, 0: not all 0.
 */
static int ismemzero4(void *data, unsigned int len){
	int *p = (int*)data;
	len = len >> 2;
	for(unsigned int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}




/**
 * @brief		get ECCP key pair(the key pair could be used in ECDH).
 * @param[out]	public_key	- public key, big--endian.
 * @param[out]	private_key	- private key, big--endian.
 * @return		1(success), 0(error).
 */
unsigned char hwECC_make_key(unsigned char *public_key, unsigned char *private_key, ecc_curve_t curve_sel)
{
	eccp_curve_t *curve = blt_ecc_get_eccp_curve(curve_sel);

	unsigned char ret;
	unsigned int tmpLen;
 	unsigned int k[PKE_OPERAND_MAX_WORD_LEN] = {0};
 	unsigned int x[PKE_OPERAND_MAX_WORD_LEN] = {0};
 	unsigned int y[PKE_OPERAND_MAX_WORD_LEN] = {0};
 	unsigned int nByteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
 	unsigned int nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);
 	unsigned int pByteLen = GET_BYTE_LEN(curve->eccp_p_bitLen);

 	ECCP_GETKEY_LOOP:

	if(g_rng_function == NULL)
	{
		return 0; //ECDH_RNG_NULL;
	}

	ret = g_rng_function((unsigned char *)k, nByteLen);

 	if(TRNG_SUCCESS != ret)
 	{
 		return 0; //ECDH_RNG_NULL;
 	}

 	//make sure k has the same bit length as n
 	tmpLen = (curve->eccp_n_bitLen)&0x1F;
 	if(tmpLen)
 	{
 		k[nWordLen-1] &= (1<<(tmpLen))-1;
 	}

 	//make sure k in [1, n-1]
 	if(ismemzero4(k, nWordLen<<2))
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	if(big_integer_compare(k, nWordLen, curve->eccp_n, nWordLen) >= 0)
 	{
 		goto ECCP_GETKEY_LOOP;
 	}
 	//get public_key
 	ret = pke_eccp_point_mul(curve, k, curve->eccp_Gx, curve->eccp_Gy, x, y);
 	if(PKE_SUCCESS != ret)
 	{
 		return 0; //Q=[k]P Failed
 	}

 	//to big-end
 	swapX((unsigned char *)k, private_key, nByteLen);
 	swapX((unsigned char *)x, public_key, pByteLen);
 	swapX((unsigned char *)y, public_key + pByteLen, pByteLen);

 	return 1; //PKE_SUCCESS;
}


/**
 * @brief		ECDH compute key.
 * @param[in]	local_prikey	- local private key, big--endian.
 * @param[in]	public_key		- peer public key, big--endian.
 * @param[out]	dhkey			- output dhkey, big--endian..
 * @Return		1(success); 0(error).
 */
unsigned char hwECC_shared_secret(const unsigned char *public_key, const unsigned char *private_key, \
								  unsigned char *secret, ecc_curve_t curve_sel)
{
	eccp_curve_t *curve = blt_ecc_get_eccp_curve(curve_sel);

	unsigned char ret;
	unsigned int k[ECC_MAX_WORD_LEN] = {0};
	unsigned int Px[ECC_MAX_WORD_LEN] = {0};
	unsigned int Py[ECC_MAX_WORD_LEN] = {0};
	unsigned int byteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
	unsigned int wordLen = GET_WORD_LEN(curve->eccp_n_bitLen);


	if(0 == private_key || 0 == public_key || 0 == secret)
	{
		return 0; //ECDH_POINTOR_NULL;
	}



	//make sure private key is in [1, n-1]
	swapX((unsigned char *)private_key, (unsigned char *)k, byteLen);

	if(ismemzero4(k, wordLen<<2))
	{
		return 0; //ECDH_INVALID_INPUT;
	}
	if(big_integer_compare(k, wordLen, curve->eccp_n, wordLen) >= 0)
	{
		return 0;// ECDH_INVALID_INPUT;
	}

	//check public key
	swapX(public_key, (unsigned char *)Px, byteLen);
	swapX(public_key+byteLen, (unsigned char *)Py, byteLen);
	ret = pke_eccp_point_verify(curve, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return 0; //ECDH_INVALID_POINT
	}

	ret = pke_eccp_point_mul(curve, k, Px, Py, Px, Py);
	if(PKE_SUCCESS != ret)
	{
		return 0; //Q=[k]P Failed
	}

	swapX((unsigned char *)Px, secret, byteLen);

	return 1; //ECDH_SUCCESS;
}

#endif


