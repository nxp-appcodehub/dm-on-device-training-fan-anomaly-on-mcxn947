// Copyright 2017-2023 NXP
// All rights reserved.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BM_WORDBITS
#define BM_WORDBITS		32
#endif
#if BM_WORDBITS == 32
#define BM_WORDBYTES	4
#define BM_ALLONE		(~0U)
#define BM_1			1U
typedef unsigned int BmAry_t;
#elif BM_WORDBITS == 64
#define BM_WORDBYTES	8
#define BM_ALLONE		(~0ULL)
#define BM_1			1ULL
typedef unsigned long long BmAry_t;
#else
#error "Unsupported word bits"
#endif



// memory layout
// 0 : bitCnt
// 4 : pData
// 8 : unitSize
// c : pBm (variable size)
typedef struct tagL2BmCtrl_t
{
    int bitCnt;
    int unitSize;
    BmAry_t* pBm;
    char* pData;
    // first member is level 2 bm, other members are level 1 bm, up to 64

} L2BmCtrl_t;

typedef struct tagL2BmMetrics_t
{
    int bitCnt;
    int ctrlSize;
    int dataSize;
} L2BmMetrics_t;

L2BmCtrl_t* L2Bm_Create( void* pvMem, int unitSize, int memSize, int isZeroMem );

int L2Bm_GetCtrlSize( int bitCnt );

int L2Bm_CalcMetrics( int unitSize, int memSize, L2BmMetrics_t* pRet );

void* L2Bm_Take( L2BmCtrl_t* pObj );

void L2Bm_Give( L2BmCtrl_t* pObj, void* pv );
#ifdef __cplusplus
}
#endif
