#include "Actor.h"

namespace core
{
    UActor::UActor():
    StaticMeshComponent(nullptr),
    Local2WorldMatrix(1.f),
    World2LocalMatrix(1.0f),
    Position(0, 0, 0),
    Rotation(0, 0, 0),
    Scale(1,1,1)
    {
    }

    void UActor::RegisterStaticMeshComponent(std::unique_ptr<UStaticMeshComponent> StaticMeshComponent)
    {
        this->StaticMeshComponent = std::move(StaticMeshComponent);
    }

    UStaticMeshComponent* UActor::GetStaticMeshComponent() const
    {
        if (StaticMeshComponent)
        {
            return StaticMeshComponent.get();
        }

        return nullptr;
    }

    const FMatrix4x4& UActor::GetLocal2WorldMatrix() const
    {
        return Local2WorldMatrix;
    }

    const FMatrix4x4& UActor::GetWorld2LocalMatrix() const
    {
        return World2LocalMatrix;
    }

    void UActor::Tick(float DeltaTime)
    {
        //Local2WorldMatrix.SetTranslation(Position);
        //Local2WorldMatrix.SetRotation(Rotation);
        //Local2WorldMatrix.SetScale(Scale);

        World2LocalMatrix = Local2WorldMatrix.GetInverse();
    }

    void UActor::RotateAroundZ(float DeltaTime)
    {
        //FMatrix4x4 RotationMatrix = FMatrix4x4::GetRotationAroundY(Degree2Radian(0.04f) * DeltaTime);
        //Local2WorldMatrix *= RotationMatrix;

        FMatrix4x4 RotationMatrix(1.0f);
        RotationMatrix.RotateAround(FVector3::Forward, Degree2Radian(0.04f) * DeltaTime);
        //Local2WorldMatrix *= RotationMatrix;
    }
}
