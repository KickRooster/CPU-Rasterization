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

    void UDriver::TransformLocal2World(const UActor& Actor, const FMatrix4x4& Local2World) const
    {
        Actor.GetStaticMeshComponent()->Transform(Local2World);
    }

    void UDriver::TransformWorld2Camera(const UActor& Actor, const FMatrix4x4& World2Camera) const
    {
        Actor.GetStaticMeshComponent()->Transform(World2Camera);
    }

    void UDriver::TransformCamera2CVV(const UActor& Actor, const FMatrix4x4& Projection) const
    {
        Actor.GetStaticMeshComponent()->Transform(Projection);
    }

    void UDriver::ProjectCVV2NDC(const UActor& Actor) const
    {
        Actor.GetStaticMeshComponent()->Project2NDC();
    }

    void UDriver::TransformNDC2Viewport(const UActor& Actor, const FMatrix4x4& Viewport) const
    {
        Actor.GetStaticMeshComponent()->Transform(Viewport);
    }

    void UDriver::Rasterize(const UActor& Actor, FIrradianceBuffer& IrradianceBuffer) const
    {
        Actor.GetStaticMeshComponent()->BeforeRasterizing();
        {
            while (Actor.GetStaticMeshComponent()->TopTriangleIsValid())
            {
                FTriangle Triangle = Actor.GetStaticMeshComponent()->PopTriangle();
                URasterizer::Instance()->RasterizeTriangle(Triangle, IrradianceBuffer);
            }
        }
        Actor.GetStaticMeshComponent()->PostRasterizing();
    }

    void UDriver::Rasterize_Debug(const UActor& Actor, FIrradianceBuffer& IrradianceBuffer) const
    {
        Actor.GetStaticMeshComponent()->BeforeRasterizing();
        {
            while (Actor.GetStaticMeshComponent()->TopTriangleIsValid())
            {
                FTriangle Triangle = Actor.GetStaticMeshComponent()->PopTriangle();

                Triangle.V0.Position.X = 100;
                Triangle.V0.Position.Y = 100;

                Triangle.V1.Position.X = 200;
                Triangle.V1.Position.Y = 400;

                Triangle.V2.Position.X = 400;
                Triangle.V2.Position.Y = 200;

                URasterizer::Instance()->RasterizeTriangle(Triangle, IrradianceBuffer);
            }
        }
        Actor.GetStaticMeshComponent()->PostRasterizing();
    }

    void UDriver::SetViewport(int32 X, int32 Y, int32 Width, int32 Height)
    {
        ViewportRect.X = X;
        ViewportRect.Y = Y;
        ViewportRect.Width = Width;
        ViewportRect.Height = Height;

        UpdateViewportMatrix();
    }

    void UDriver::DrawActor(const UActor& Actor, const UCamera& Camera, FIrradianceBuffer& IrradianceBuffer)
    {
        if (Actor.GetStaticMeshComponent())
        {
            TransformLocal2World(Actor, Actor.GetLocal2WorldMatrix());
            TransformWorld2Camera(Actor, Camera.GetWorld2LocalMatrix());
            TransformCamera2CVV(Actor, Camera.GetPerspectiveProjectionMatrix());
            //  TODO:   Clip.
            ProjectCVV2NDC(Actor);
            TransformNDC2Viewport(Actor, ViewportMat);
            Rasterize(Actor, IrradianceBuffer);
        }
    }

    void UDriver::Debug(const UActor& Actor, const UCamera& Camera, FIrradianceBuffer& IrradianceBuffer)
    {
        if (Actor.GetStaticMeshComponent())
        {
            Rasterize_Debug(Actor, IrradianceBuffer);
        }
    }
}
