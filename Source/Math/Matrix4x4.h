#pragma once
#include "MathBase.h"
#include "Vector3.h"

namespace core
{
    //  Row major matrix, store in memory layout.
    //  [0][0]  [0][1]  [0][2]  [0][3]  [1][0]  [1][1]  [1][2]  [1][3]  [2][0]  [2][1]  [2][2]  [2][3]  [3][0]  [3][1]  [3][2]  [3][3]
    //  Basic vectors store as "column major".
    //  Vector4 stores as column vector, FMatrix4x4 post-multiply Vector4.
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
        FMatrix4x4(const FMatrix4x4& Matrix);
        float GetDeterminant() const ;
        FMatrix4x4 GetInverse() const ;
        FMatrix4x4 GetTranspose() const ;
        //  Clockwise, from 0 -> +X
        static FMatrix4x4 GetRotationAroundX(float Radian);
        //  Clockwise, from 0 -> +Y
        static FMatrix4x4 GetRotationAroundY(float Radian);
        //  Clockwise, from 0 -> +Z
        static FMatrix4x4 GetRotationAroundZ(float Radian);
        void RotateAround(const FVector3& Axis, float Radian);
        static FMatrix4x4 RotateAround(const FMatrix4x4& Matrix, const FVector3& Axis, float Radian);
        FVector3 GetXAxis() const;
        FVector3 GetYAxis() const;
        FVector3 GetZAxis() const;
        void SetTranslation(const FVector3& Position);
        void SetRotation(const FVector3& Rotation);
        void SetScale(const FVector3& Scale);
        void LookAt(const FVector3& Postion, const FVector3& Center, const FVector3& Up);
        FMatrix4x4 operator * (const FMatrix4x4 & Matrix);
        const FMatrix4x4& operator *= (const FMatrix4x4 & Matrix);
    };
}
