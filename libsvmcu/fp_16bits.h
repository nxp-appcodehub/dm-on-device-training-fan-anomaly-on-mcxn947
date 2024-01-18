// Copyright 2017-2023 NXP
// All rights reserved.
#pragma once
#include <stdint.h>
#pragma pack(push)
#pragma pack(1)

typedef union tagSoftFP16
{
    struct
    {
        uint16_t F10 : 10;
        uint16_t E5 : 5;
        uint16_t S1 : 1;
    } std;
    struct
    {
        uint16_t F7 : 7;
        uint16_t E8 : 8;
        uint16_t S1 : 1;
    } alt;
    int16_t i16Val;
} SoftFP16_t;


typedef union tagSoftFP32
{
public:
    struct
    {
        uint32_t F23 : 23;
        uint32_t E8 : 8;
        uint32_t S1 : 1;
    };
    int16_t i16Vals[2];
    int32_t i32Val;
    uint32_t u32Val;
    float f32Val;
} SoftFP32_t;



union CBFloat16
{
public:
    struct
    {
        uint16_t F10 : 10;
        uint16_t E5 : 5;
        uint16_t S1 : 1;
    } std;
    struct
    {
        uint16_t F7 : 7;
        uint16_t E8 : 8;
        uint16_t S1 : 1;
    } alt;
    uint16_t u16Val;
    int16_t i16Val;

    CBFloat16( float din )
    {
        SoftFP32_t t1;
        t1.f32Val = din;
        i16Val = t1.i16Vals[1];
        if ( alt.F7 != 0x7F )
        {
            alt.F7 += ( t1.u32Val >> 15 ) & 1;
        }
    }

    static SoftFP16_t BuildStd( int F, int E, int S )
    {
        SoftFP16_t ret;
        ret.std.F10 = ( uint16_t )F;
        ret.std.E5 = ( uint16_t )E;
        ret.std.S1 = ( S != 0 );
        return ret;
    }

    static SoftFP16_t BuildAlt( int F, int E, int S )
    {
        SoftFP16_t ret;
        ret.alt.F7 = ( uint16_t )F;
        ret.alt.E8 = ( uint16_t )E;
        ret.alt.S1 = ( S != 0 );
        return ret;
    }

    static void AltToFP32( const SoftFP16_t* pcSrc, float* pDst, int cnt )
    {
        SoftFP32_t* pSoftDst = ( SoftFP32_t* )pDst;
        while ( cnt-- )
        {
            ( pSoftDst++ )->i32Val = ( int32_t )( ( pcSrc++ )->i16Val ) << 16;
        }
    }
    static void FP32ToAlt( const float * pcSrc, SoftFP16_t * pDst, int cnt )
    {
        SoftFP32_t* pcSoftSrc = ( SoftFP32_t* )pcSrc;
        while ( cnt-- )
        {
            ( pDst++ )->i16Val = ( pcSoftSrc++ )->i16Vals[1];
        }
    }

    operator float()
    {
        unsigned int proc = u16Val << 16;
        return *reinterpret_cast<float*>( &proc );
    }

};


union CFP16
{
    struct
    {
        uint16_t F10 : 10;
        uint16_t E5 : 5;
        uint16_t S1 : 1;
    } std;
    uint16_t u16Val, x;
    int16_t i16Val;

    static uint32_t as_uint( const float x )
    {
        return *( uint32_t* )&x;
    }
    static float as_float( const uint32_t x )
    {
        return *( float* )&x;
    }


    CFP16 operator=( float din )
    {
        const uint32_t b = as_uint( din ) + 0x00001000; // round-to-nearest-even: add last bit after truncated mantissa
        const uint32_t e = ( b & 0x7F800000 ) >> 23; // exponent
        const uint32_t m = b & 0x007FFFFF; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding
        x = ( b & 0x80000000 ) >> 16
            | ( e > 112 ) * ( ( ( ( e - 112 ) << 10 ) & 0x7C00 )
                              | m >> 13 ) | ( ( e < 113 ) & ( e > 101 ) ) * ( ( ( ( 0x007FF000 + m ) >> ( 125 - e ) ) + 1 ) >> 1 )
            | ( e > 143 ) * 0x7FFF; // sign : normalized : denormalized : saturate
        return *this;
    }

    CFP16( float din )
    {
        const uint32_t b = as_uint( din ) + 0x00001000; // round-to-nearest-even: add last bit after truncated mantissa
        const uint32_t e = ( b & 0x7F800000 ) >> 23; // exponent
        const uint32_t m = b & 0x007FFFFF; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding
        x = ( b & 0x80000000 ) >> 16
            | ( e > 112 ) * ( ( ( ( e - 112 ) << 10 ) & 0x7C00 )
                              | m >> 13 ) | ( ( e < 113 ) & ( e > 101 ) ) * ( ( ( ( 0x007FF000 + m ) >> ( 125 - e ) ) + 1 ) >> 1 )
            | ( e > 143 ) * 0x7FFF; // sign : normalized : denormalized : saturate
    }

    operator float()
    {
        const uint32_t e = ( x & 0x7C00 ) >> 10; // exponent
        const uint32_t m = ( x & 0x03FF ) << 13; // mantissa
        const uint32_t v = as_uint( ( float )m ) >> 23; // evil log2 bit hack to count leading zeros in denormalized format
        int ret =
            ( x & 0x8000 ) << 16 | // sign bit
            ( e != 0 ) * ( ( e + 112 ) << 23 | m ) | // e and m for e != 0
            ( ( e == 0 ) & ( m != 0 ) ) * ( ( v - 37 ) << 23 | // e and m for e == 0
                                            ( ( m << ( 150 - v ) ) & 0x007FE000 ) ); // sign : normalized : denormalized
        return *reinterpret_cast<float*>( &ret );
    }

};
//	Error[Pe338]: more than one instance of overloaded function has "C" linkage (see "__vfp CBFloat16 operator*(CBFloat16, float)" (declared Hat line 158h))	D:\SDK_2_12_0_MIMXRT1170-EVK\boards\evkmimxrt1170\sdmmc_examples\sdcard_fatfs\cm7\fp_16bits.h	160
//	Error[Pe338]: more than one instance of overloaded function has "C" linkage (see "__vfp CBFloat16 operator*(CBFloat16, float)" (declared Hat line 158h))	D:\0228\boards\evkmimxrt1170\display_examples\h264_freertos\cm7\fp_16bits.h	160

#pragma pack(pop)

CBFloat16 operator*( CBFloat16 lhs, float rhs );

CBFloat16 operator*( float lhs, CBFloat16 rhs );

CBFloat16 operator*( CBFloat16 lhs, CBFloat16 rhs );



