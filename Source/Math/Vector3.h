#pragma once
#include "MathBase.h"

namespace core
{
    struct FVector3
    {
        float X;
        float Y;
        float Z;

        FVector3();
        FVector3(float InX, float InY, float InZ);
        FVector3& operator= (const FVector3& Vector);
        FVector3 operator + (const FVector3& Vector3);
        FVector3 operator - (const FVector3& Vector) const;
        FVector3 operator * (float Scale) const;
        FVector3 operator - () const;
        const FVector3& operator += (const FVector3& Vector);
        float GetLength() const;
        void Normalize();
        static FVector3 Forward;
        static FVector3 Up;
        static FVector3 Right;
        static FVector3 Normalize(const FVector3& Vector);
        static float Dot(const FVector3& LHS, const FVector3& RHS);
        //  left-hand rule
        static FVector3 Cross(const FVector3& LHS, const FVector3& RHS);
    };
}
