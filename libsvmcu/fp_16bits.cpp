// Copyright 2017-2023 NXP
// All rights reserved.
#include "fp_16bits.h"
CBFloat16 operator*( CBFloat16 lhs, float rhs )
{
    SoftFP32_t t1;
    t1.u32Val = ( uint32_t )lhs.i16Val << 16;
    t1.f32Val = ( t1.f32Val * rhs );
    return CBFloat16( t1.f32Val );
}

CBFloat16 operator*( float lhs, CBFloat16 rhs )
{
    SoftFP32_t t1;
    t1.u32Val = ( uint32_t )rhs.i16Val << 16;
    t1.f32Val = ( t1.f32Val * lhs );
    return CBFloat16( t1.f32Val );
}

CBFloat16 operator*( CBFloat16 lhs, CBFloat16 rhs )
{
    SoftFP32_t t1, t2;
    t1.u32Val = ( uint32_t )lhs.i16Val << 16;
    t2.u32Val = ( uint32_t )rhs.i16Val << 16;
    t1.f32Val = ( t1.f32Val * t2.f32Val );
    return CBFloat16( t1.f32Val );
}


CFP16 operator*( CFP16 lhs, float rhs )
{
    CFP16 ret( ( float )lhs * rhs );
    return ret;
}

CFP16 operator*( float lhs, CFP16 rhs )
{
    CFP16 ret( ( float )rhs * lhs );
    return ret;
}
