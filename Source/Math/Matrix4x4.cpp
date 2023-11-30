#include "Matrix4x4.h"

namespace core
{
    FMatrix4x4::FMatrix4x4(float Diagonal)
    {
        M[0][0] = Diagonal;
        M[0][1] = 0;
        M[0][2] = 0;
        M[0][3] = 0;

        M[1][0] = 0;
        M[1][1] = Diagonal;
        M[1][2] = 0;
        M[1][3] = 0;

        M[2][0] = 0;
        M[2][1] = 0;
        M[2][2] = Diagonal;
        M[2][3] = 0;

        M[3][0] = 0;
        M[3][1] = 0;
        M[3][2] = 0;
        M[3][3] = Diagonal;
    }

    FMatrix4x4::FMatrix4x4(float M[][4])
    {
        this->M[0][0] = M[0][0];
        this->M[0][1] = M[0][1];
        this->M[0][2] = M[0][2];
        this->M[0][3] = M[0][3];

        this->M[1][0] = M[1][0];
        this->M[1][1] = M[1][1];
        this->M[1][2] = M[1][2];
        this->M[1][3] = M[1][3];

        this->M[2][0] = M[2][0];
        this->M[2][1] = M[2][1];
        this->M[2][2] = M[2][2];
        this->M[2][3] = M[2][3];

        this->M[3][0] = M[3][0];
        this->M[3][1] = M[3][1];
        this->M[3][2] = M[3][2];
        this->M[3][3] = M[3][3];
    }

    FMatrix4x4::FMatrix4x4(float M00, float M01, float M02, float M03, float M10, float M11, float M12, float M13, float M20, float M21, float M22, float M23, float M30, float M31, float M32, float M33)
    {
        M[0][0] = M00;
        M[0][1] = M01;
        M[0][2] = M02;
        M[0][3] = M03;

        M[1][0] = M10;
        M[1][1] = M11;
        M[1][2] = M12;
        M[1][3] = M13;

        M[2][0] = M20;
        M[2][1] = M21;
        M[2][2] = M22;
        M[2][3] = M23;

        M[3][0] = M30;
        M[3][1] = M31;
        M[3][2] = M32;
        M[3][3] = M33;
    }

    FMatrix4x4::FMatrix4x4(const FMatrix4x4& Matrix)
    {
        M[0][0] = Matrix.M[0][0];
        M[0][1] = Matrix.M[0][1];
        M[0][2] = Matrix.M[0][2];
        M[0][3] = Matrix.M[0][3];

        M[1][0] = Matrix.M[1][0];
        M[1][1] = Matrix.M[1][1];
        M[1][2] = Matrix.M[1][2];
        M[1][3] = Matrix.M[1][3];

        M[2][0] = Matrix.M[2][0];
        M[2][1] = Matrix.M[2][1];
        M[2][2] = Matrix.M[2][2];
        M[2][3] = Matrix.M[2][3];

        M[3][0] = Matrix.M[3][0];
        M[3][1] = Matrix.M[3][1];
        M[3][2] = Matrix.M[3][2];
        M[3][3] = Matrix.M[3][3];
    }

    float FMatrix4x4::GetDeterminant() const
    {
        //  00  01  02  03  00  01  02  03
        //  10  11  12  13  10  11  12  13
        //  20  21  22  23  20  21  22  23
        //  30  31  32  33  30  31  32  33
        float Determinant =
            M[0][0] * (
                M[1][1] * M[2][2] * M[3][3] +
                M[1][2] * M[2][3] * M[3][1] +
                M[1][3] * M[2][1] * M[3][2] -
                M[1][1] * M[2][3] * M[3][2] -
                M[1][2] * M[2][1] * M[3][3] -
                M[1][3] * M[2][2] * M[3][1])
        -
            M[1][0] * (
                M[0][1] * M[2][2] * M[3][3] +
                M[0][2] * M[2][3] * M[3][1] +
                M[0][3] * M[2][1] * M[3][2] -
                M[0][1] * M[2][3] * M[3][2] -
                M[0][2] * M[2][1] * M[3][3] -
                M[0][3] * M[2][2] * M[3][1])
        +
            M[2][0] * (
                M[0][1] * M[1][2] * M[3][3] +
                M[0][2] * M[1][3] * M[3][1] +
                M[0][3] * M[1][1] * M[3][2] -
                M[0][1] * M[1][3] * M[3][2] -
                M[0][2] * M[1][1] * M[3][3] -
                M[0][3] * M[1][2] * M[3][1])
        -
            M[3][0] * (
                M[0][1] * M[1][2] * M[2][3] +
                M[0][2] * M[1][3] * M[2][1] +
                M[0][3] * M[1][1] * M[2][2] -
                M[0][1] * M[1][3] * M[2][2] -
                M[0][2] * M[1][1] * M[2][3] -
                M[0][3] * M[1][2] * M[2][1]) ;

        return Determinant;
    }

    FMatrix4x4 FMatrix4x4::GetInverse() const
    {
        float C00 = (
            M[1][1] * M[2][2] * M[3][3] +
            M[1][2] * M[2][3] * M[3][1] +
            M[1][3] * M[2][1] * M[3][2] -
            M[1][1] * M[2][3] * M[3][2] -
            M[1][2] * M[2][1] * M[3][3] -
            M[1][3] * M[2][2] * M[3][1]
            );

        float C01 = -(
            M[1][0] * M[2][2] * M[3][3] +
            M[1][2] * M[2][3] * M[3][0] +
            M[1][3] * M[2][0] * M[3][2] -
            M[1][0] * M[2][3] * M[3][2] -
            M[1][2] * M[2][0] * M[3][3] -
            M[1][3] * M[2][2] * M[3][0]
            );

        float C02 = (
            M[1][0] * M[2][1] * M[3][3] +
            M[1][1] * M[2][3] * M[3][0] +
            M[1][3] * M[2][0] * M[3][1] -
            M[1][0] * M[2][3] * M[3][1] -
            M[1][1] * M[2][0] * M[3][3] -
            M[1][3] * M[2][1] * M[3][0]
            );

        float C03 = -(
            M[1][0] * M[2][1] * M[3][2] +
            M[1][1] * M[2][2] * M[3][0] +
            M[1][2] * M[2][0] * M[3][1] -
            M[1][0] * M[2][2] * M[3][1] -
            M[1][1] * M[2][0] * M[3][2] -
            M[1][2] * M[2][1] * M[3][0]
            );

        float C10 = -(
            M[0][1] * M[2][2] * M[3][3] +
            M[0][2] * M[2][3] * M[3][1] +
            M[0][3] * M[2][1] * M[3][2] -
            M[0][1] * M[2][3] * M[3][2] -
            M[0][2] * M[2][1] * M[3][3] -
            M[0][3] * M[2][2] * M[3][1]
            );

        float C11 = (
            M[0][0] * M[2][2] * M[3][3] +
            M[0][2] * M[2][3] * M[3][0] +
            M[0][3] * M[2][0] * M[3][2] -
            M[0][0] * M[2][3] * M[3][2] -
            M[0][2] * M[2][0] * M[3][3] -
            M[0][3] * M[2][2] * M[3][0]
            );

        float C12 = -(
            M[0][0] * M[2][1] * M[3][3] +
            M[0][1] * M[2][3] * M[3][0] +
            M[0][3] * M[2][0] * M[3][1] -
            M[0][0] * M[2][3] * M[3][1] -
            M[0][1] * M[2][0] * M[3][3] -
            M[0][3] * M[2][1] * M[3][0]
            );

        float C13 = (
            M[0][0] * M[2][1] * M[3][2] +
            M[0][1] * M[2][2] * M[3][0] +
            M[0][2] * M[2][0] * M[3][1] -
            M[0][0] * M[2][2] * M[3][1] -
            M[0][1] * M[2][0] * M[3][2]-
            M[0][2] * M[2][1] * M[3][0]
            );

        float C20 = (
            M[0][1] * M[1][2] * M[3][3] +
            M[0][2] * M[1][3] * M[3][1] +
            M[0][3] * M[1][1] * M[3][2] -
            M[0][1] * M[1][3] * M[3][2] -
            M[0][2] * M[1][1] * M[3][3] -
            M[0][3] * M[1][2] * M[3][1]
            );

        float C21 = -(
            M[0][0] * M[1][2] * M[3][3] +
            M[0][2] * M[1][3] * M[3][0] +
            M[0][3] * M[1][0] * M[3][2] -
            M[0][0] * M[1][3] * M[3][2] -
            M[0][2] * M[1][0] * M[3][3] -
            M[0][3] * M[1][2] * M[3][0]
            );

        float C22 = (
            M[0][0] * M[1][1] * M[3][3] +
            M[0][1] * M[1][3] * M[3][0] +
            M[0][3] * M[1][0] * M[3][1] -
            M[0][0] * M[1][3] * M[3][1] -
            M[0][1] * M[1][0] * M[3][3] -
            M[0][3] * M[1][1] * M[3][0]
            );

        float C23 = -(
            M[0][0] * M[1][1] * M[3][2] +
            M[0][1] * M[1][2] * M[3][0] +
            M[0][2] * M[1][0] * M[3][1] -
            M[0][0] * M[1][2] * M[3][1] -
            M[0][1] * M[1][0] * M[3][2] -
            M[0][2] * M[1][1] * M[3][0]
            );

        float C30 = -(
            M[0][1] * M[1][2] * M[2][3] +
            M[0][2] * M[1][3] * M[2][1] +
            M[0][3] * M[1][1] * M[2][2] -
            M[0][1] * M[1][3] * M[2][2] -
            M[0][2] * M[1][1] * M[2][3] -
            M[0][3] * M[1][2] * M[2][1]
            );

        float C31 = (
            M[0][0] * M[1][2] * M[2][3] +
            M[0][2] * M[1][3] * M[2][0] +
            M[0][3] * M[1][0] * M[2][2] -
            M[0][0] * M[1][3] * M[2][2] -
            M[0][2] * M[1][0] * M[2][3] -
            M[0][3] * M[1][2] * M[2][0]
            );

        float C32 = -(
            M[0][0] * M[1][1] * M[2][3] +
            M[0][1] * M[1][3] * M[2][0] +
            M[0][3] * M[1][0] * M[2][1] -
            M[0][0] * M[1][3] * M[2][1] -
            M[0][1] * M[1][0] * M[2][3] -
            M[0][3] * M[1][1] * M[2][0]
            );

        float C33 = (
            M[0][0] * M[1][1] * M[2][2] +
            M[0][1] * M[1][2] * M[2][0] +
            M[0][2] * M[1][0] * M[2][1] -
            M[0][0] * M[1][2] * M[2][1] -
            M[0][1] * M[1][0] * M[2][2] -
            M[0][2] * M[1][1] * M[2][0]
            );

        FMatrix4x4 Adjoint = FMatrix4x4(
            C00, C10, C20, C30,
            C01, C11, C21, C31,
            C02, C12, C22, C32,
            C03, C13, C23, C33);

        float Determinant = GetDeterminant();

        FMatrix4x4 Inverse = FMatrix4x4(
            Adjoint.M[0][0] / Determinant, Adjoint.M[0][1] / Determinant, Adjoint.M[0][2] / Determinant, Adjoint.M[0][3] / Determinant,
            Adjoint.M[1][0] / Determinant, Adjoint.M[1][1] / Determinant, Adjoint.M[1][2] / Determinant, Adjoint.M[1][3] / Determinant,
            Adjoint.M[2][0] / Determinant, Adjoint.M[2][1] / Determinant, Adjoint.M[2][2] / Determinant, Adjoint.M[2][3] / Determinant,
            Adjoint.M[3][0] / Determinant, Adjoint.M[3][1] / Determinant, Adjoint.M[3][2] / Determinant, Adjoint.M[3][3] / Determinant);

        return Inverse;
    }

    FMatrix4x4 FMatrix4x4::GetTranspose() const
    {
        FMatrix4x4 Transpose;

        Transpose.M[0][0] = M[0][0];
        Transpose.M[0][1] = M[1][0];
        Transpose.M[0][2] = M[2][0];
        Transpose.M[0][3] = M[3][0];

        Transpose.M[1][0] = M[0][1];
        Transpose.M[1][1] = M[1][1];
        Transpose.M[1][2] = M[2][1];
        Transpose.M[1][3] = M[3][1];

        Transpose.M[2][0] = M[0][2];
        Transpose.M[2][1] = M[1][2];
        Transpose.M[2][2] = M[2][2];
        Transpose.M[2][3] = M[3][2];

        Transpose.M[3][0] = M[0][3];
        Transpose.M[3][1] = M[1][3];
        Transpose.M[3][2] = M[2][3];
        Transpose.M[3][3] = M[3][3];

        return Transpose;
    }

    FMatrix4x4 FMatrix4x4::GetRotationAroundX(float Radian)
    {
        FMatrix4x4 Mat(1.f);

        Mat.M[1][1] = cos(Radian);
        Mat.M[2][1] = sin(Radian);

        Mat.M[1][2] = -sin(Radian);
        Mat.M[2][2] = cos(Radian);

        return Mat;
    }

    FMatrix4x4 FMatrix4x4::GetRotationAroundY(float Radian)
    {
        FMatrix4x4 Mat(1.f);

        Mat.M[0][0] = cos(Radian);
        Mat.M[2][0] = -sin(Radian);

        Mat.M[0][2] = sin(Radian);
        Mat.M[2][2] = cos(Radian);

        return Mat;
    }

    FMatrix4x4 FMatrix4x4::GetRotationAroundZ(float Radian)
    {
        FMatrix4x4 Mat(1.f);

        Mat.M[0][0] = cos(Radian);
        Mat.M[1][0] = sin(Radian);

        Mat.M[0][1] = -sin(Radian);
        Mat.M[1][1] = cos(Radian);

        return Mat;
    }

    void FMatrix4x4::RotateAround(const FVector3& Axis, float Radian)
    {
        float SinTheta = sin(Radian);
        float CosTheta = cos(Radian);

        M[0][0] = CosTheta + (1 - CosTheta) * Axis.X * Axis.X;
        M[1][0] = (1 - CosTheta) * Axis.X * Axis.Y + SinTheta * Axis.Z;
        M[2][0] = (1 - CosTheta) * Axis.X * Axis.Z - SinTheta * Axis.Y;

        M[0][1] = (1 - CosTheta) * Axis.X * Axis.Y - SinTheta * Axis.Z;
        M[1][1] = CosTheta + (1 - CosTheta) * Axis.Y * Axis.Y;
        M[2][1] = (1 - CosTheta) * Axis.Y * Axis.Z + SinTheta * Axis.X;

        M[0][2] = (1 - CosTheta) * Axis.X * Axis.Z + SinTheta * Axis.Y;
        M[1][2] = (1- CosTheta) * Axis.Y * Axis.Z - SinTheta * Axis.X;
        M[2][2] = CosTheta + (1 - CosTheta) * Axis.Z * Axis.Z;
    }

    FMatrix4x4 FMatrix4x4::RotateAround(const FMatrix4x4& Matrix, const FVector3& Axis, float Radian)
    {
        FMatrix4x4 Rotation(1.0f);
        Rotation.RotateAround(Axis, Radian);

        FMatrix4x4 Result = Rotation * Matrix;

        return Result;
    }

    FVector3 FMatrix4x4::GetXAxis() const
    {
        return FVector3(M[0][0], M[1][0], M[2][0]);
    }

    FVector3 FMatrix4x4::GetYAxis() const
    {
        return FVector3(M[0][1], M[1][1], M[2][1]);
    }

    FVector3 FMatrix4x4::GetZAxis() const
    {
        return FVector3(M[0][2], M[1][2], M[2][2]);
    }

    void FMatrix4x4::SetTranslation(const FVector3& Position)
    {
        M[3][0] = Position.X;
        M[3][1] = Position.Y;
        M[3][2] = Position.Z;
    }

    void FMatrix4x4::SetRotation(const FVector3& Rotation)
    {
        FMatrix4x4 RotationAroundX = GetRotationAroundX(Rotation.X);
        FMatrix4x4 RotationAroundY = GetRotationAroundY(Rotation.Y);
        FMatrix4x4 RotationAroundZ = GetRotationAroundZ(Rotation.Z);
        FMatrix4x4 CombinedRotation = RotationAroundX * RotationAroundY * RotationAroundZ;

        (*this) *= CombinedRotation;
    }

    void FMatrix4x4::SetScale(const FVector3& Scale)
    {
        M[0][0] *= Scale.X;
        M[1][1] *= Scale.Y;
        M[2][2] *= Scale.Z;
    }

    void FMatrix4x4::LookAt(const FVector3& Postion, const FVector3& Center, const FVector3& Up)
    {
        FVector3 Forward = Center - Postion;
        Forward.Normalize();

        FVector3 Right = FVector3::Cross(Forward,  Up);
        Right.Normalize();

        FVector3 LocalUp = FVector3::Cross(Right, Forward);
        LocalUp.Normalize();

        M[0][0] = Right.X;
        M[1][0] = Right.Y;
        M[2][0] = Right.Z;

        M[0][1] = LocalUp.X;
        M[1][1] = LocalUp.Y;
        M[2][1] = LocalUp.Z;

        M[0][2] = Forward.X;
        M[1][2] = Forward.Y;
        M[2][2] = Forward.Z;

        M[0][3] = 0;
        M[1][3] = 0;
        M[2][3] = 0;

        M[3][0] = -FVector3::Dot(Postion, Right);
        M[3][1] = -FVector3::Dot(Postion, LocalUp);
        M[3][2] = -FVector3::Dot(Postion, Forward);
        M[3][3] = 1.0f;
    }

    FMatrix4x4 FMatrix4x4::operator*(const FMatrix4x4& Matrix)
    {
        FMatrix4x4 Mat(0.0);

        for (int32 Row = 0; Row < 4; ++Row)
        {
            for (int32 Column = 0; Column < 4; ++Column)
            {
                Mat.M[Row][Column] =
                        M[Row][0] * Matrix.M[0][Column] +
                        M[Row][1] * Matrix.M[1][Column] +
                        M[Row][2] * Matrix.M[2][Column] +
                        M[Row][3] * Matrix.M[3][Column];
            }
        }

        return Mat;
    }

    const FMatrix4x4& FMatrix4x4::operator*=(const FMatrix4x4& Matrix)
    {
        FMatrix4x4 Mat = (*this) * Matrix;

        memcpy(M, Mat.M, sizeof(float) * 16);

        return *this;
    }
}
