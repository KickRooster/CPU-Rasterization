#pragma once
#include "../DesignPattern/SingletonPattern/Singleton.h"
#include "Triangle.h"
#include "IrradianceBuffer.h"
#include "../Components/ShadingComponent.h"

namespace core
{
    //  Reference:  http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo2
    class URasterizer : public Singleton<URasterizer>
    {
        FTriangle SortByY(const FTriangle& Triangle) const;
        float GetInterpolatedCameraSpaceZ(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3) const;
        FHDRColor GetInterpolatedColorOfGeneralCaseTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, bool bPerspectiveCorrectInterpolation) const;
        FVector2 GetInterpolatedUVOfGeneralCaseTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, bool bPerspectiveCorrectInterpolation) const;
        void FillUpBottomFlatTriangleL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpBottomFlatTriangleR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpALineL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpALineR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const FVertex& V3, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpTopFlatTriangleL2R(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpTopFlatTriangleR2L(const FVertex& V0, const FVertex& V1, const FVertex& V2, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;
        void DoStandard(const FTriangle& Triangle, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const;

    public:
        void RasterizeTriangle(const FTriangle& Triangle, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const ;
    };
}
