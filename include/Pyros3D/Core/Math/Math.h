//============================================================================
// Name        : Math.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : Math
//============================================================================

#ifndef MATH_H
#define	MATH_H

#include <Pyros3D/Other/Export.h>
#include <stdint.h>

namespace p3d {

    // Type Definition
    typedef uint8_t PYROS3D_API uint8;
    typedef int8_t PYROS3D_API int8;
    typedef uint16_t PYROS3D_API uint16;
    typedef int16_t PYROS3D_API int16;
    typedef int32_t PYROS3D_API int32;
    typedef uint32_t PYROS3D_API uint32;
    typedef int64_t PYROS3D_API int64;
    typedef uint64_t PYROS3D_API uint64;
    typedef float PYROS3D_API f32;
    typedef unsigned char PYROS3D_API uchar;
    typedef double PYROS3D_API f64;    
};
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <math.h>

// Constants
#ifndef NULL
#define NULL 0
#endif
#define EPSILON 1e-8 
#define PI 3.14159265358979323846
#define DEGTORAD(x)	( ((x) * PI) / 180.0 )
#define RADTODEG(x)	( ((x) * 180.0) / PI )

#define SQR(x) ( (x) * (x) )

#define LIMIT_RANGE(low, value, high) { if (value < low)	value = low; else if(value > high) value = high; }

#define Min(a,b) ((a)<(b)?(a):(b))
#define Max(a,b) ((a)>(b)?(a):(b))
#define Clamp(x) ( Min(Max(x,-1),1) )

using namespace p3d::Math;

namespace p3d {
    
    namespace Math {
    
	namespace RotationOrder {
		enum {
			XYZ = 0,
			YXZ,
			ZXY,
			ZYX,
			YZX,
			XZY
		};
	};
        template <typename T>
        std::string NumberToString ( T Number )
        {
           std::ostringstream ss;
           ss << Number;
           return ss.str();
        }
    };
};

#endif	/* MATH_H */