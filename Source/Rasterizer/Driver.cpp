#include "Driver.h"

namespace core
{
    void UDriver::UpdateViewportMatrix()
    {
        ViewportMat.M[0][0] = static_cast<float>(ViewportRect.Width) / 2 ;
        ViewportMat.M[0][1] = 0;
        ViewportMat.M[0][2] = 0;
        ViewportMat.M[0][3] = 0 ;

        ViewportMat.M[1][0] = 0;
        ViewportMat.M[1][1] = static_cast<float>(ViewportRect.Height) / 2;
        ViewportMat.M[1][2] = 0;
        ViewportMat.M[1][3] = 0;

        ViewportMat.M[2][0] = 0;
        ViewportMat.M[2][1] = 0;
        ViewportMat.M[2][2] = 1.0;
        ViewportMat.M[2][3] = 0;

        ViewportMat.M[3][0] = static_cast<float>(ViewportRect.Width) / 2;
        ViewportMat.M[3][1] = static_cast<float>(ViewportRect.Height) / 2;
        ViewportMat.M[3][2] = 0;
        ViewportMat.M[3][3] = 1.0;
    }

    void UDriver::Transform(FPrimitiveList& PrimitiveList, const FMatrix4x4& Matrix) const
    {
        for (SIZE_T i = 0; i < PrimitiveList.VertexList.size(); ++i)
        {
            FVertex TransformedVertex;
            TransformedVertex.Position = PrimitiveList.VertexList[i].Position * Matrix;

            PrimitiveList.VertexList[i].Position = TransformedVertex.Position;
        }
    }

    void UDriver::RecordCameraSpaceZ(FPrimitiveList& PrimitiveList) const
    {
        for (SIZE_T i = 0; i < PrimitiveList.VertexList.size(); ++i)
        {
            PrimitiveList.VertexList[i].CameraSpaceZ = PrimitiveList.VertexList[i].Position.Z;
        }
    }

    void UDriver::Project2NDC(FPrimitiveList& PrimitiveList) const
    {
        for (SIZE_T i = 0; i < PrimitiveList.VertexList.size(); ++i)
        {
            PrimitiveList.VertexList[i].Position.X /= PrimitiveList.VertexList[i].Position.W;
            PrimitiveList.VertexList[i].Position.Y /= PrimitiveList.VertexList[i].Position.W;
            PrimitiveList.VertexList[i].Position.Z /= PrimitiveList.VertexList[i].Position.W;
            PrimitiveList.VertexList[i].Position.W /= PrimitiveList.VertexList[i].Position.W;
        }
    }

    void UDriver::Rasterize(FPrimitiveList& PrimitiveList, const UShadingComponent& ShadingComponent, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer) const
    {
        PrimitiveList.BeforeRasterizing();
        {
            while (PrimitiveList.TopIsValid())
            {
                FTriangle Triangle = PrimitiveList.PopTriangle();
                URasterizer::Instance()->RasterizeTriangle(Triangle, ShadingComponent, bPerspectiveCorrectInterpolation, IrradianceBuffer);
            }
        }
        PrimitiveList.PostRasterizing();
    }

    void UDriver::SetViewport(int32 X, int32 Y, int32 Width, int32 Height)
    {
        ViewportRect.X = X;
        ViewportRect.Y = Y;
        ViewportRect.Width = Width;
        ViewportRect.Height = Height;

        UpdateViewportMatrix();
    }

    void UDriver::DrawActor(UActor& Actor, const UCamera& Camera, bool bPerspectiveCorrectInterpolation, FIrradianceBuffer& IrradianceBuffer)
    {
        if (Actor.GetStaticMeshComponent() && Actor.GetShadingComponent())
        {
            FPrimitiveList TransformedPrimitiveList = Actor.GetStaticMeshComponent()->GetPrimitiveList();
            //  Transform local to world.
            Transform(TransformedPrimitiveList, Actor.GetLocal2WorldMatrix());
            //  Transform world to camera.
            Transform(TransformedPrimitiveList, Camera.GetWorld2LocalMatrix());
            //  Record camera space z used for perspective corrected interpolation later.
            RecordCameraSpaceZ(TransformedPrimitiveList);
            //  Transform camera to CVV.
            Transform(TransformedPrimitiveList, Camera.GetPerspectiveProjectionMatrix());
            //  TODO:   Clip.
            //  Project CVV to NDC.
            Project2NDC(TransformedPrimitiveList);
            //  Transform NDC to viewport.
            Transform(TransformedPrimitiveList, ViewportMat);
            Rasterize(TransformedPrimitiveList, *Actor.GetShadingComponent(), bPerspectiveCorrectInterpolation, IrradianceBuffer);
        }
    }
}
