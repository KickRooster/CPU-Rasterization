#include "Rasterizer.h"
#include "../Math/MathGlobal.h"
#include <algorithm>

namespace core
{
    //  Sort the three vertices by y-coordinate ascending so v0 is the topmost vertice
    FTriangle URasterizer::SortByY(const FTriangle& Triangle) const
    {
        FVertex Vertices[3] = {Triangle.V0, Triangle.V1, Triangle.V2};
        for (int32 i = 1; i < 3; ++i)
        {
            FVertex Key = Vertices[i];
            int32 j = i - 1;
            while(j >= 0 && Vertices[j].Position.Y > Key.Position.Y)
            {
                Vertices[j + 1] = Vertices[j];
                --j;
            }
            Vertices[j + 1] = Key;
        }

        FTriangle SortedTriangle = FTriangle(Vertices[0], Vertices[1], Vertices[2]);

        return SortedTriangle;
    }

    float URasterizer::GetInterpolatedCameraSpaceZ(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3) const
    {
        FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
        FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
        FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
        FVector2 PointP = FVector2(V3.Position.X, V3.Position.Y);

        float Area012 = GetArea(Point0, Point1, Point2);

        float Area1P2 = GetArea(PointP, Point1, Point2);
        float Point0Weight = Area1P2 / Area012;

        float Area2P0 = GetArea(PointP, Point2, Point0);
        float Point1Weight = Area2P0 / Area012;

        float Area0P1 = GetArea(PointP, Point0, Point1);
        float Point2Weight = Area0P1 / Area012;

        float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

        return Zt;
    }

    FHDRColor URasterizer::GetInterpolatedColorOfGeneralCaseTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, bool bPerspectiveCorrectInterpolation) const
    {
        FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
        FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
        FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
        FVector2 PointP = FVector2(V3.Position.X, V3.Position.Y);

        float Area012 = GetArea(Point0, Point1, Point2);

        float Area1P2 = GetArea(PointP, Point1, Point2);
        float Point0Weight = Area1P2 / Area012;

        float Area2P0 = GetArea(PointP, Point2, Point0);
        float Point1Weight = Area2P0 / Area012;

        float Area0P1 = GetArea(PointP, Point0, Point1);
        float Point2Weight = Area0P1 / Area012;

        FHDRColor InterpolatedColor;

        if (bPerspectiveCorrectInterpolation)
        {
            float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

            InterpolatedColor = (
                V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                V2.Color * (Point2Weight / V2.CameraSpaceZ)
                ) / (1 / Zt);
        }
        else
        {
            InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
        }

        return InterpolatedColor;
    }

    FVector2 URasterizer::GetInterpolatedUVOfGeneralCaseTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, bool bPerspectiveCorrectInterpolation) const
    {
        FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
        FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
        FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
        FVector2 PointP = FVector2(V3.Position.X, V3.Position.Y);

        float Area012 = GetArea(Point0, Point1, Point2);

        float Area1P2 = GetArea(PointP, Point1, Point2);
        float Point0Weight = Area1P2 / Area012;

        float Area2P0 = GetArea(PointP, Point2, Point0);
        float Point1Weight = Area2P0 / Area012;

        float Area0P1 = GetArea(PointP, Point0, Point1);
        float Point2Weight = Area0P1 / Area012;

        FVector2 InterpolatedUV;

        if (bPerspectiveCorrectInterpolation)
        {
            float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

            InterpolatedUV = (
                V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                V2.UV * (Point2Weight / V2.CameraSpaceZ)
                ) / (1 / Zt);
        }
        else
        {
            InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;
        }

        return InterpolatedUV;
    }

    //(0,0)-----------------
    //     |      V0-------V1
    //     |       \      /
    //     |        \    /
    //     |         \  /
    //     |          V2
    //  V0, V1 and V2 have been clipped, they layed in [0, 1].
    void URasterizer::FillUpBottomFlatTriangleL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope0 = (V0.Position.X - V2.Position.X) / (V0.Position.Y - V2.Position.Y);
        float InvSlope1 = (V1.Position.X - V2.Position.X) / (V1.Position.Y - V2.Position.Y);

        float CurrentX0 = V2.Position.X;
        float CurrentX1 = V2.Position.X;
        float CurrentY = V2.Position.Y;

        int32 V2Y = static_cast<int32>(roundf(V2.Position.Y));
        int32 V0V1Y = static_cast<int32>(roundf(V0.Position.Y));

        int32 ScanLineY = V2Y;
        do
        {
            int32 StartX = static_cast<int32>(roundf(CurrentX0));
            int32 EndX = static_cast<int32>(roundf(CurrentX1));

            for (int32 i = StartX; i <= EndX; ++i)
            {
                if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
                {
                    continue;
                }

                FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
                FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
                FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
                FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

                float Area012 = GetArea(Point0, Point1, Point2);

                float AreaP12 = GetArea(PointP, Point1, Point2);
                float Point0Weight = AreaP12 / Area012;

                float AreaP02 = GetArea(PointP, Point0, Point2);
                float Point1Weight = AreaP02 / Area012;

                float AreaP01 = GetArea(PointP, Point0, Point1);
                float Point2Weight = AreaP01 / Area012;

                if (bPerspectiveCorrectInterpolation)
                {
                    float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                    FHDRColor InterpolatedColor = (
                        V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                        V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                        V2.Color * (Point2Weight / V2.CameraSpaceZ)
                        ) / (1 / Zt);

                    FVector2 InterpolatedUV = (
                        V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                        V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                        V2.UV * (Point2Weight / V2.CameraSpaceZ)
                        ) / (1 / Zt);

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
                else
                {
                    FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                    FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
            }

            CurrentX0 -= InvSlope0;
            CurrentX1 -= InvSlope1;
            CurrentY -= 1.0f;
            --ScanLineY;
        }
        while(ScanLineY > V0V1Y);
    }

    //(0,0)-----------------
    //     |      V1-------V0
    //     |       \      /
    //     |        \    /
    //     |         \  /
    //     |          V2
    //  V0, V1 and V2 have been clipped, they layed in [0, 1].
    void URasterizer::FillUpBottomFlatTriangleR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope1 = (V1.Position.X - V2.Position.X) / (V1.Position.Y - V2.Position.Y);
        float InvSlope0 = (V0.Position.X - V2.Position.X) / (V0.Position.Y - V2.Position.Y);

        float CurrentX1 = V2.Position.X;
        float CurrentX0 = V2.Position.X;
        float CurrentY = V2.Position.Y;

        int32 V2Y = static_cast<int32>(roundf(V2.Position.Y));
        int32 V1V0Y = static_cast<int32>(roundf(V1.Position.Y));

        int32 ScanLineY = V2Y;
        do
        {
            int32 StartX = static_cast<int32>(roundf(CurrentX1));
            int32 EndX = static_cast<int32>(roundf(CurrentX0));

            for (int32 i = StartX; i <= EndX; ++i)
            {
                if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
                {
                    continue;
                }

                FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
                FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
                FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
                FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

                float Area102 = GetArea(Point1, Point0, Point2);

                float AreaP02 = GetArea(PointP, Point0, Point2);
                float Point1Weight = AreaP02 / Area102;

                float AreaP12 = GetArea(PointP, Point1, Point2);
                float Point0Weight = AreaP12 / Area102;

                float AreaP10 = GetArea(PointP, Point1, Point0);
                float Point2Weight = AreaP10 / Area102;

                if (bPerspectiveCorrectInterpolation)
                {
                    float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                    FHDRColor InterpolatedColor = (
                        V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                        V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                        V2.Color * (Point2Weight / V2.CameraSpaceZ)
                        ) / (1 / Zt);

                    FVector2 InterpolatedUV = (
                        V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                        V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                        V2.UV * (Point2Weight / V2.CameraSpaceZ)
                        ) / (1 / Zt);

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
                else
                {
                    FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                    FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
            }

            CurrentX0 -= InvSlope0;
            CurrentX1 -= InvSlope1;
            CurrentY -= 1.0f;
            --ScanLineY;
        }
        while(ScanLineY > V1V0Y);
    }

    //(0,0)-----------------
    //     |           V0
    //     |         / |
    //     |        /  |
    //     |       /   |
    //     |      V1---V3
    //     |       \   |
    //     |        \  |
    //     |         \ |
    //     |           V2
    //  V0, V1 and V2 have been clipped, they layed in [0, 1].
    void URasterizer::FillUpALineL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        int32 StartX = static_cast<int32>(roundf(V1.Position.X));
        int32 EndX = static_cast<int32>(roundf(V3.Position.X));

        float CurrentY = V1.Position.Y;
        int32 ScanLineY = static_cast<int32>(roundf(V1.Position.Y));

        for (int32 i = StartX; i <= EndX; ++i)
        {
            if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
            {
                continue;
            }

            FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
            FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
            FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
            FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

            float Area012 = GetArea(Point0, Point1, Point2);

            float Area1P2 = GetArea(PointP, Point1, Point2);
            float Point0Weight = Area1P2 / Area012;

            float Area2P0 = GetArea(PointP, Point2, Point0);
            float Point1Weight = Area2P0 / Area012;

            float Area0P1 = GetArea(PointP, Point0, Point1);
            float Point2Weight = Area0P1 / Area012;

            if (bPerspectiveCorrectInterpolation)
            {
                float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                FHDRColor InterpolatedColor = (
                    V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                    V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                    V2.Color * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                FVector2 InterpolatedUV = (
                    V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                    V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                    V2.UV * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
            }
            else
            {
                FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
            }
        }
    }

    //(0,0)-----------------
    //     |           V0
    //     |           | \
    //     |           |  \
    //     |           |   \
    //     |           V3---V1
    //     |           |   /
    //     |           |  /
    //     |           | /
    //     |           V2
    //  V0, V1 and V2 have been clipped, they layed in [0, 1].
    void URasterizer::FillUpALineR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        int32 StartX = static_cast<int32>(roundf(V3.Position.X));
        int32 EndX = static_cast<int32>(roundf(V1.Position.X));

        float CurrentY = V3.Position.Y;
        int32 ScanLineY = static_cast<int32>(roundf(V3.Position.Y));

        for (int32 i = StartX; i <= EndX; ++i)
        {
            if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
            {
                continue;
            }

            FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
            FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
            FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
            FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

            float Area012 = GetArea(Point0, Point1, Point2);

            float Area1P2 = GetArea(PointP, Point1, Point2);
            float Point0Weight = Area1P2 / Area012;

            float Area2P0 = GetArea(PointP, Point2, Point0);
            float Point1Weight = Area2P0 / Area012;

            float Area0P1 = GetArea(PointP, Point0, Point1);
            float Point2Weight = Area0P1 / Area012;

            if (bPerspectiveCorrectInterpolation)
            {
                float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                FHDRColor InterpolatedColor = (
                    V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                    V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                    V2.Color * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                FVector2 InterpolatedUV = (
                    V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                    V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                    V2.UV * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
            }
            else
            {
                FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
            }
        }
    }

    //(0,0)-----------------
    //     |          V0
    //     |         /  \
    //     |        /    \
    //     |       /      \
    //     |      V1-------V2
    void URasterizer::FillUpTopFlatTriangleL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope1 = (V1.Position.X - V0.Position.X) / (V1.Position.Y - V0.Position.Y);
        float InvSlope2 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);

        float CurrentX1 = V0.Position.X;
        float CurrentX2 = V0.Position.X;
        float CurrentY = V0.Position.Y;

        int32 V0Y = static_cast<int32>(roundf(V0.Position.Y));
        int32 V1V2Y = static_cast<int32>(roundf(V1.Position.Y));

        int32 ScanLineY = V0Y;
        do
        {
            int32 StartX = static_cast<int32>(roundf(CurrentX1));
            int32 EndX = static_cast<int32>(roundf(CurrentX2));

            for (int32 i = StartX; i <= EndX; ++i)
            {
                if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
                {
                    continue;
                }

                FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
                FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
                FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
                FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

                float Area012 = GetArea(Point0, Point1, Point2);

                float Area1P2 = GetArea(PointP, Point1, Point2);
                float Point0Weight = Area1P2 / Area012;

                float Area2P0 = GetArea(PointP, Point2, Point0);
                float Point1Weight = Area2P0 / Area012;

                float Area0P1 = GetArea(PointP, Point0, Point1);
                float Point2Weight = Area0P1 / Area012;

                if (bPerspectiveCorrectInterpolation)
                {
                    float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                    FHDRColor InterpolatedColor = (
                    V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                    V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                    V2.Color * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                    FVector2 InterpolatedUV = (
                    V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                    V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                    V2.UV * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);
                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
                else
                {
                    FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                    FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
            }

            CurrentX1 += InvSlope1;
            CurrentX2 += InvSlope2;
            CurrentY += 1.0f;
            ++ScanLineY;
        }
        while (ScanLineY < V1V2Y);
    }

    //(0,0)-----------------
    //     |          V0
    //     |         /  \
    //     |        /    \
    //     |       /      \
    //     |      V2-------V1
    void URasterizer::FillUpTopFlatTriangleR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        float InvSlope2 = (V2.Position.X - V0.Position.X) / (V2.Position.Y - V0.Position.Y);
        float InvSlope1 = (V1.Position.X - V0.Position.X) / (V1.Position.Y - V0.Position.Y);

        float CurrentX2 = V0.Position.X;
        float CurrentX1 = V0.Position.X;
        float CurrentY = V0.Position.Y;

        int32 V0Y = static_cast<int32>(roundf(V0.Position.Y));
        int32 V1V2Y = static_cast<int32>(roundf(V2.Position.Y));

        int32 ScanLineY = V0Y;
        do
        {
            int32 StartX = static_cast<int32>(roundf(CurrentX2));
            int32 EndX = static_cast<int32>(roundf(CurrentX1));

            for (int32 i = StartX; i <= EndX; ++i)
            {
                if (!IrradianceBuffer.PixelPostionValid(ScanLineY, i))
                {
                    continue;
                }

                FVector2 Point0 = FVector2(V0.Position.X, V0.Position.Y);
                FVector2 Point1 = FVector2(V1.Position.X, V1.Position.Y);
                FVector2 Point2 = FVector2(V2.Position.X, V2.Position.Y);
                FVector2 PointP = FVector2(static_cast<float>(i), CurrentY);

                float Area012 = GetArea(Point0, Point1, Point2);

                float Area1P2 = GetArea(PointP, Point1, Point2);
                float Point0Weight = Area1P2 / Area012;

                float Area2P0 = GetArea(PointP, Point2, Point0);
                float Point1Weight = Area2P0 / Area012;

                float Area0P1 = GetArea(PointP, Point0, Point1);
                float Point2Weight = Area0P1 / Area012;

                if (bPerspectiveCorrectInterpolation)
                {
                    float Zt = 1.0f / (Point0Weight / V0.CameraSpaceZ + Point1Weight / V1.CameraSpaceZ + Point2Weight / V2.CameraSpaceZ);

                    FHDRColor InterpolatedColor = (
                    V0.Color * (Point0Weight / V0.CameraSpaceZ) +
                    V1.Color * (Point1Weight / V1.CameraSpaceZ) +
                    V2.Color * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                    FVector2 InterpolatedUV = (
                    V0.UV * (Point0Weight / V0.CameraSpaceZ) +
                    V1.UV * (Point1Weight / V1.CameraSpaceZ) +
                    V2.UV * (Point2Weight / V2.CameraSpaceZ)
                    ) / (1 / Zt);

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
                else
                {
                    FHDRColor InterpolatedColor = V0.Color * Point0Weight + V1.Color * Point1Weight + V2.Color * Point2Weight;
                    FVector2 InterpolatedUV = V0.UV * Point0Weight + V1.UV * Point1Weight + V2.UV * Point2Weight;

                    FHDRColor Result = ShadingComponent.Shading(InterpolatedUV);

                    IrradianceBuffer.FillUpOnePixel(ScanLineY, i, Result);
                    //IrradianceBuffer.FillUpOnePixel(ScanLineY, i, InterpolatedColor);
                }
            }

            CurrentX1 += InvSlope1;
            CurrentX2 += InvSlope2;
            CurrentY += 1.0f;
            ++ScanLineY;
        }
        while (ScanLineY < V1V2Y);
    }

    void URasterizer::DoStandard(const FTriangle& Triangle, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        FTriangle SortedTriangle = SortByY(Triangle);

        //  Till now, the original of the coordinate of Triangle is top-left, after viewport transformation.

        //  Check for trivial case of bottom-flat triangle.
        if (Equal(SortedTriangle.V0.Position.Y, SortedTriangle.V1.Position.Y))
        {
            if (SortedTriangle.V0.Position.X <= SortedTriangle.V1.Position.X)
            {
                //(0,0)-----------------
                //     |      V0-------V1
                //     |       \      /
                //     |        \    /
                //     |         \  /
                //     |          V2
                FillUpBottomFlatTriangleL2R(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
            else
            {
                //(0,0)-----------------
                //     |      V1-------V0
                //     |       \      /
                //     |        \    /
                //     |         \  /
                //     |          V2
                FillUpBottomFlatTriangleR2L(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
        }
        //  Check for trivial case of top-flat triangle.
        else if (Equal(SortedTriangle.V1.Position.Y, SortedTriangle.V2.Position.Y))
        {
            if (SortedTriangle.V1.Position.X <= SortedTriangle.V2.Position.X)
            {
                //(0,0)-----------------
                //     |          V0
                //     |         /  \
                //     |        /    \
                //     |       /      \
                //     |      V1-------V2
                FillUpTopFlatTriangleL2R(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
            else
            {
                //(0,0)-----------------
                //     |          V0
                //     |         /  \
                //     |        /    \
                //     |       /      \
                //     |      V2-------V1
                FillUpTopFlatTriangleR2L(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
        }
        //  Gneral case - split the triangle in a top-flat and bottom-flat one.
        else
        {
            FVertex V3 = FVertex(
                SortedTriangle.V0.Position.X + (SortedTriangle.V1.Position.Y - SortedTriangle.V0.Position.Y) / (SortedTriangle.V2.Position.Y - SortedTriangle.V0.Position.Y) * (SortedTriangle.V2.Position.X - SortedTriangle.V0.Position.X),
                SortedTriangle.V1.Position.Y,
                0,   //  XXX:    Position.Z is not used during rasterizating.
                1.0f);
            V3.CameraSpaceZ = GetInterpolatedCameraSpaceZ(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, V3);
            V3.Color = GetInterpolatedColorOfGeneralCaseTriangle(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, V3, bPerspectiveCorrectInterpolation);
            V3.UV = GetInterpolatedUVOfGeneralCaseTriangle(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, V3, bPerspectiveCorrectInterpolation);

            if (SortedTriangle.V1.Position.X <= V3.Position.X)
            {
                //(0,0)-----------------
                //     |          V0
                //     |         /  \
                //     |        /    \
                //     |       /      \
                //     |      V1-------V3
                FillUpTopFlatTriangleL2R(SortedTriangle.V0, SortedTriangle.V1, V3, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
            else
            {
                //(0,0)-----------------
                //     |          V0
                //     |         /  \
                //     |        /    \
                //     |       /      \
                //     |      V3-------V1
                FillUpTopFlatTriangleR2L(SortedTriangle.V0, SortedTriangle.V1, V3, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }

            //  Patch a line between two triangles.
            {
                if (SortedTriangle.V1.Position.X <= V3.Position.X)
                {
                    FillUpALineL2R(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, V3, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
                }
                else
                {
                    FillUpALineR2L(SortedTriangle.V0, SortedTriangle.V1, SortedTriangle.V2, V3, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
                }
            }
            if (SortedTriangle.V1.Position.X <= V3.Position.X)
            {
                //(0,0)-----------------
                //     |      V1-------V3
                //     |       \      /
                //     |        \    /
                //     |         \  /
                //     |          V2
                FillUpBottomFlatTriangleL2R(SortedTriangle.V1, V3, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
            else
            {
                //(0,0)-----------------
                //     |      V3-------V1
                //     |       \      /
                //     |        \    /
                //     |         \  /
                //     |          V2
                FillUpBottomFlatTriangleR2L(SortedTriangle.V1, V3, SortedTriangle.V2, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
        }
    }

    void URasterizer::RasterizeTriangle(const FTriangle& Triangle, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        DoStandard(Triangle, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
    }
}
