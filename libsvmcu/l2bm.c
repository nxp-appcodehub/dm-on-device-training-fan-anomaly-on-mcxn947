// Copyright 2017-2023 NXP
// All rights reserved.
#include "l2bm.h"
#include <stdio.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
const char cg_tabCtz8[256] =
{
    8,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0000 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0001 0000
    5,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0010 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0011 0000
    6,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0100 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0101 0000
    5,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0110 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 0111 0000
    7,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1000 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1001 0000
    5,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1010 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1011 0000
    6,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1100 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1101 0000
    5,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1110 0000
    4,	0,	1,	0,	2,	0,	1,	0,		3,	0,	1,	0,	2,	0,	1,	0,	// 1111 0000
};

int CtzWord( BmAry_t* pBm )
{
    char* pBm8 = ( char* )pBm;
    int ret = 0;
    int tmp;
    unsigned char curByte = 0xFF;
    for ( int i = 0; i < BM_WORDBYTES; i++ )
    {
        if ( ( curByte = *pBm8++ ) != 0 )
        {
            tmp = cg_tabCtz8[curByte];
            ret += tmp;
            break;
        }
        ret += 8;
    }
    return ret;
}

int CtzWordClear( BmAry_t* pBm )
{
    char* pBm8 = ( char* )pBm;
    int ret = 0;
    int tmp;
    unsigned char curByte = 0xFF;
    for ( int i = 0; i < BM_WORDBYTES; i++ )
    {
        if ( ( curByte = *pBm8++ ) != 0 )
        {
            tmp = cg_tabCtz8[curByte];
            ret += tmp;
            pBm8[-1] &= ~( 1 << tmp );
            break;
        }
        ret += 8;
    }
    return ret;
}

L2BmCtrl_t* L2Bm_Create( void* pvMem, int unitSize, int memSize, int isZeroMem )
{
    char* pMem = ( char* )pvMem;
    int bitCnt = memSize / unitSize;
    int ctrlSize, dataSize;
    do
    {
        ctrlSize = sizeof( L2BmCtrl_t ) + BM_WORDBYTES +
                   ( ( bitCnt + BM_WORDBITS - 1 ) / BM_WORDBITS ) * BM_WORDBYTES;
        dataSize = bitCnt * unitSize;
    }
    while ( ctrlSize + dataSize > memSize && bitCnt-- );

    L2BmCtrl_t* pObj = ( L2BmCtrl_t* )pMem;
    pObj->bitCnt = bitCnt;
    pObj->unitSize = unitSize;
    pObj->pBm = ( BmAry_t* )( pMem + sizeof( L2BmCtrl_t ) );
    pObj->pData = pMem + ctrlSize;
    pObj->pBm[0] = ( BM_1 << ( bitCnt + BM_WORDBITS - 1 ) / BM_WORDBITS ) - 1;
    BmAry_t* pL1 = pObj->pBm + 1;
    while ( bitCnt >= BM_WORDBITS )
    {
        *pL1++ = BM_ALLONE;
        bitCnt -= BM_WORDBITS;
    }
    if ( bitCnt )
    {
        *pL1++ = ( ( BM_1 << bitCnt ) - BM_1 );
    }
    if ( isZeroMem )
    {
        memset( pObj->pData, 0, bitCnt * unitSize );
    }
    return pObj;
}

int L2Bm_GetCtrlSize( int bitCnt )
{
    int ctrlSize = sizeof( L2BmCtrl_t ) + BM_WORDBYTES +
                   ( ( bitCnt + BM_WORDBITS - 1 ) / BM_WORDBITS ) * BM_WORDBYTES;
    return ctrlSize;
}

int L2Bm_CalcMetrics( int unitSize, int memSize, L2BmMetrics_t *pRet )
{
    int bitCnt = memSize / unitSize;
    int ctrlSize, dataSize;
    do
    {
        ctrlSize = sizeof( L2BmCtrl_t ) + BM_WORDBYTES +
                   ( ( bitCnt + BM_WORDBITS - 1 ) / BM_WORDBITS ) * BM_WORDBYTES;
        dataSize = bitCnt * unitSize;
    }
    while ( ctrlSize + dataSize > memSize && bitCnt-- );
    pRet->bitCnt = bitCnt;
    pRet->ctrlSize = ctrlSize;
    pRet->dataSize = dataSize;
    return bitCnt;
}

void* L2Bm_Take( L2BmCtrl_t* pObj )
{
    if ( pObj->pBm[0] == 0 )
        return ( void* )0;
    int wordNdx = CtzWord( pObj->pBm );
    BmAry_t* pWord = pObj->pBm + 1 + wordNdx;
    int bitNdxRem = CtzWordClear( pWord );
    if ( *pWord == 0 )
    {
        pObj->pBm[0] &= ~( BM_1 << wordNdx );
    }

    int bitNdx = wordNdx + bitNdxRem;

    char* pRet = pObj->pData + bitNdx * pObj->unitSize;
    return ( void* )pRet;
}

void* L2Bm_Realloc( L2BmCtrl_t* pObj, void* pOld )
{
    void* pNew = L2Bm_Take( pObj );
    if ( pNew )
    {
        memcpy( pNew, pOld, pObj->unitSize );
    }
    else
    {
        pNew = pOld;
    }
}

void L2Bm_Give( L2BmCtrl_t* pObj, void* pv )
{
    char* pFree = ( char* )pv;
    int bitNdx = ( pObj->pData - pFree ) / pObj->unitSize;
    int wordNdx = bitNdx / BM_WORDBITS;
    int bitNdxRem = bitNdx & ( BM_WORDBITS - 1 );
    BmAry_t* pWord = pObj->pBm + 1 + wordNdx;
    *pWord |= ( BM_1 << bitNdxRem );
    pObj->pBm[0] |= ( BM_1 << wordNdx );
}
#ifdef __cplusplus
}
#endif
