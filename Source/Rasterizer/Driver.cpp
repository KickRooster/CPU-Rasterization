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

    void UDriver::TransformLocal2World(FPrimitiveList& PrimitiveList, const FMatrix4x4& Local2World) const
    {
        Transform(PrimitiveList, Local2World);
    }

    void UDriver::TransformWorld2Camera(FPrimitiveList& PrimitiveList, const FMatrix4x4& World2Camera) const
    {
       Transform(PrimitiveList, World2Camera);
    }

    void UDriver::TransformCamera2CVV(FPrimitiveList& PrimitiveList, const FMatrix4x4& Projection) const
    {
        Transform(PrimitiveList, Projection);
    }

    void UDriver::ProjectCVV2NDC(FPrimitiveList& PrimitiveList) const
    {
        Project2NDC(PrimitiveList);
    }

    void UDriver::TransformNDC2Viewport(FPrimitiveList& PrimitiveList, const FMatrix4x4& Viewport) const
    {
        Transform(PrimitiveList, Viewport);
    }

    void UDriver::Rasterize(FPrimitiveList& PrimitiveList, FIrradianceBuffer& IrradianceBuffer) const
    {
        PrimitiveList.BeforeRasterizing();
        {
            while (PrimitiveList.TopIsValid())
            {
                FTriangle Triangle = PrimitiveList.PopTriangle();
                URasterizer::Instance()->RasterizeTriangle(Triangle, IrradianceBuffer);
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

    void UDriver::DrawActor(UActor& Actor, const UCamera& Camera, FIrradianceBuffer& IrradianceBuffer)
    {
        if (Actor.GetStaticMeshComponent())
        {
            FPrimitiveList TransformedPrimitiveList = Actor.GetStaticMeshComponent()->GetPrimitiveList();
            TransformLocal2World(TransformedPrimitiveList, Actor.GetLocal2WorldMatrix());
            TransformWorld2Camera(TransformedPrimitiveList, Camera.GetWorld2LocalMatrix());
            TransformCamera2CVV(TransformedPrimitiveList, Camera.GetPerspectiveProjectionMatrix());
            //  TODO:   Clip.
            ProjectCVV2NDC(TransformedPrimitiveList);
            TransformNDC2Viewport(TransformedPrimitiveList, ViewportMat);
            Rasterize(TransformedPrimitiveList, IrradianceBuffer);
        }
    }
}
