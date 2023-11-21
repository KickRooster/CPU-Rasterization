#pragma once
#include <Core.h>

//  Right-hand coordinate
//  Colum major matrix

#ifndef FLOAT_ERROR
#define FLOAT_ERROR 0.000001f;
#endif

namespace core
{
    bool Equal(float Lhs, float Rhs);
}
