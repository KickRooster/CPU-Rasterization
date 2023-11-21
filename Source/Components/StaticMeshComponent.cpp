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

    void UStaticMeshComponent::Transform(const FMatrix4x4& Matrix)
    {
        for (SIZE_T i = 0; i < PrimitiveList->VertexList.size(); ++i)
        {
            FVertex TransformedVertex;
            TransformedVertex.Position = PrimitiveList->VertexList[i].Position * Matrix;

            PrimitiveList->VertexList[i].Position.X = TransformedVertex.Position.X;
            PrimitiveList->VertexList[i].Position.Y = TransformedVertex.Position.Y;
            PrimitiveList->VertexList[i].Position.Z = TransformedVertex.Position.Z;
            PrimitiveList->VertexList[i].Position.W = TransformedVertex.Position.W;
        }
    }

    void UStaticMeshComponent::Project2NDC()
    {
        for (SIZE_T i = 0; i < PrimitiveList->VertexList.size(); ++i)
        {
            PrimitiveList->VertexList[i].Position.X /= PrimitiveList->VertexList[i].Position.W;
            PrimitiveList->VertexList[i].Position.Y /= PrimitiveList->VertexList[i].Position.W;
            PrimitiveList->VertexList[i].Position.Z /= PrimitiveList->VertexList[i].Position.W;
            PrimitiveList->VertexList[i].Position.W /= PrimitiveList->VertexList[i].Position.W;
        }
    }

    void UStaticMeshComponent::BeforeRasterizing() const
    {
        PrimitiveList->BeforeRasterizing();
    }

    bool UStaticMeshComponent::TopTriangleIsValid() const
    {
        return PrimitiveList->TopIsValid();
    }

    FTriangle UStaticMeshComponent::PopTriangle() const
    {
        return PrimitiveList->PopTriangle();
    }

    void UStaticMeshComponent::PostRasterizing() const
    {
        PrimitiveList->PostRasterizing();
    }

    UStaticMeshComponent::~UStaticMeshComponent()
    {

    }
}
