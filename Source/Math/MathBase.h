#pragma once
#include <Core.h>

//  Left-hand coordinate
//  Row major matrix, store in memory layout.
//  [0][0]  [0][1]  [0][2]  [0][3]  [1][0]  [1][1]  [1][2]  [1][3]  [2][0]  [2][1]  [2][2]  [2][3]  [3][0]  [3][1]  [3][2]  [3][3]
//  Basic vectors store as "column major".
//  Vector4 stores as column vector, FMatrix4x4 post-multiply Vector4.
//  1   0   0   0
//  0   1   0   0
//  0   0   1   0
//  Tx  Ty  Tz  1

#ifndef FLOAT_ERROR
#define FLOAT_ERROR 0.000001f;
#endif

#ifndef PI
#define PI 3.141592654f
#endif

#ifndef DEGREE_TO_RADIAN
#define DEGREE_TO_RADIAN 0.017453292519f
#endif

#ifndef RADIAN_TO_DEGREE
#define RADIAN_TO_DEGREE 57.295779513f
#endif
