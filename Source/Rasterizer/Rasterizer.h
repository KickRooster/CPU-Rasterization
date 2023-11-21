#pragma once
#include "../DesignPattern/SingletonPattern/Singleton.h"
#include "Triangle.h"
#include "IrradianceBuffer.h"

namespace core
{
    //  Reference:  http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo2
    class URasterizer : public Singleton<URasterizer>
    {
        FTriangle SortByY(const FTriangle& Triangle) const;
        void FillUpBottomFlatTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, FIrradianceBuffer& IrradianceBuffer) const;
        void FillUpTopFlatTriangle(const FVertex& V0, const FVertex& V1, const FVertex& V2, FIrradianceBuffer& IrradianceBuffer) const;
        void DoStandard(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const;

    public:
        void RasterizeTriangle(const FTriangle& Triangle, FIrradianceBuffer& IrradianceBuffer) const ;
    };
}
