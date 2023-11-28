#pragma once
#include <Core.h>
#include "../Components/StaticMeshComponent.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"

namespace core
{
    class UActor
    {
    protected:
        std::unique_ptr<UStaticMeshComponent> StaticMeshComponent;
        FMatrix4x4 Local2WorldMatrix;
        FMatrix4x4 World2LocalMatrix;

    public:
        FVector3 Position;
        FVector3 Rotation;
        FVector3 Scale;

        UActor();
        void RegisterStaticMeshComponent(std::unique_ptr<UStaticMeshComponent> StaticMeshComponent);
        UStaticMeshComponent* GetStaticMeshComponent() const;
        const FMatrix4x4& GetLocal2WorldMatrix() const;
        const FMatrix4x4& GetWorld2LocalMatrix() const ;
        virtual void Tick(float DeltaTime);
        //  Code for testing.
        virtual void RotateAroundZ(float DeltaTime);
        virtual ~UActor(){}
    };
}
