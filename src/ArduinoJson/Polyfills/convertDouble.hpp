//
// Created by matheus on 18/02/18.
//

#ifndef ARDUNOMESSAGEPACK_CONVERTDOUBLE_HPP
#define ARDUNOMESSAGEPACK_CONVERTDOUBLE_HPP

#include <inttypes.h>

#define signF64UI( a ) ((bool) ((uint64_t) (a)>>63))
#define expF64UI( a ) ((int_fast16_t) ((a)>>52) & 0x7FF)
#define fracF64UI( a ) ((a) & UINT64_C( 0x000FFFFFFFFFFFFF ))
#define packToF32UI( sign, exp, sig ) (((uint32_t) (sign)<<31) + ((uint32_t) (exp)<<23) + (sig))

typedef struct { uint32_t v; } float32_t; //used for soft convertion
typedef struct { uint64_t v; } float64_t; //used for soft convertion
struct commonNaN {
    bool sign;
    uint64_t v0, v64;
};

union ui32_f32 { uint32_t ui; float32_t f; };
union ui64_f64 { uint64_t ui; float64_t f; };


float32_t softfloat_roundPackToF32( bool, int_fast16_t, uint_fast32_t );


uint32_t softfloat_shiftRightJam32( uint32_t a, uint_fast16_t count )
{

    return
            (count < 31) ? a>>count | ((uint32_t) (a<<(-count & 31)) != 0)
                         : (a != 0);

}


void softfloat_f64UIToCommonNaN( uint_fast64_t uiA, struct commonNaN *zPtr )
{
    zPtr->sign = uiA>>63;
    zPtr->v64  = uiA<<12;
    zPtr->v0   = 0;
}

uint_fast32_t softfloat_commonNaNToF32UI( const struct commonNaN *aPtr )
{

    return (uint_fast32_t) aPtr->sign<<31 | 0x7FC00000 | aPtr->v64>>41;

}


uint64_t softfloat_shortShiftRightJam64( uint64_t a, uint_fast8_t count )
{

    return a>>count | ((a & (((uint_fast64_t) 1<<count) - 1)) != 0);

}


float32_t f64_to_f32( float64_t a )
{
    union ui64_f64 uA;
    uint_fast64_t uiA;
    bool sign;
    int_fast16_t exp;
    uint_fast64_t sig;
    struct commonNaN commonNaN;
    uint_fast32_t uiZ, sig32;
    union ui32_f32 uZ;

    uA.f = a;
    uiA = uA.ui;
    sign = signF64UI( uiA );
    exp  = expF64UI( uiA );
    sig  = fracF64UI( uiA );
    if ( exp == 0x7FF ) {
        if ( sig ) {
            softfloat_f64UIToCommonNaN( uiA, &commonNaN );
            uiZ = softfloat_commonNaNToF32UI( &commonNaN );
        } else {
            uiZ = packToF32UI( sign, 0xFF, 0 );
        }
        goto uiZ;
    }
    sig32 = softfloat_shortShiftRightJam64( sig, 22 );
    if ( ! (exp | sig32) ) {
        uiZ = packToF32UI( sign, 0, 0 );
        goto uiZ;
    }
    return softfloat_roundPackToF32( sign, exp - 0x381, sig32 | 0x40000000 );
    uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

float32_t
softfloat_roundPackToF32( bool sign, int_fast16_t exp, uint_fast32_t sig )
{

    bool roundNearEven;
    uint_fast8_t roundIncrement, roundBits;
    uint_fast32_t uiZ;
    union ui32_f32 uZ;

    roundIncrement = 0x40;

    roundBits = sig & 0x7F;
    if ( 0xFD <= (uint16_t) exp ) {
        if ( exp < 0 ) {
            sig = softfloat_shiftRightJam32( sig, -exp );
            exp = 0;
            roundBits = sig & 0x7F;

        } else if ( (0xFD < exp) || (0x80000000 <= sig + roundIncrement) ) {
            uiZ = packToF32UI( sign, 0xFF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }

    sig = (sig + roundIncrement)>>7;
    sig &= ~(uint_fast32_t) (! (roundBits ^ 0x40) & roundNearEven);
    uiZ = packToF32UI( sign, sig ? exp : 0, sig );
    uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}

#endif //ARDUNOMESSAGEPACK_CONVERTDOUBLE_HPP
