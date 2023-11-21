#pragma once
#include "MathBase.h"
#include "Vector3.h"

namespace core
{
    //  Colum major matrix
    //  1   0   0   0
    //  0   1   0   0
    //  0   0   1   0
    //  Tx  Ty  Tz  1
    struct FMatrix4x4
    {
    public:
        float M[4][4];

        FMatrix4x4(float Diagonal = 0);
        FMatrix4x4(float M[][4]);
        FMatrix4x4(float M00, float M01, float M02, float M03, float M10, float M11, float M12, float M13, float M20, float M21, float M22, float M23, float M30, float M31, float M32, float M33);
        float GetDeterminant() const ;
        FMatrix4x4 GetInverse() const ;
        FMatrix4x4 GetTranspose() const ;
        static FMatrix4x4 GetRotationAroundX(float Radian);
        static FMatrix4x4 GetRotationAroundY(float Radian);
        static FMatrix4x4 GetRotationAroundZ(float Radian);
        void SetTranslation(const FVector3& Position);
        void SetRotation(const FVector3& Rotation);
        void SetScale(const FVector3& Scale);

        FMatrix4x4 operator * (const FMatrix4x4 & Matrix);
        const FMatrix4x4& operator *= (const FMatrix4x4 & Matrix);
    };
}
