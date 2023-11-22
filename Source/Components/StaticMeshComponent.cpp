#include "StaticMeshComponent.h"

namespace core
{
    UStaticMeshComponent::UStaticMeshComponent()
    {
        PrimitiveList = std::make_unique<FPrimitiveList>();
    }

    void UStaticMeshComponent::OnRegister()
    {

    }

    void UStaticMeshComponent::OnUnregister()
    {

    }

    void UStaticMeshComponent::AddVertex(FVertex Vertex)
    {
        PrimitiveList->AddVertex(Vertex);
    }

    void UStaticMeshComponent::AddIndex(int32 Index)
    {
        PrimitiveList->AddIndex(Index);
    }

    const FPrimitiveList& UStaticMeshComponent::GetPrimitiveList() const
    {
        return *PrimitiveList;
    }

    UStaticMeshComponent::~UStaticMeshComponent()
    {

    }
}
