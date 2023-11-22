#pragma once
#include <Core.h>
#include <vector>
#include "IComponent.h"
#include "../Rasterizer/PrimitiveList.h"
#include "../Math/Matrix4x4.h"
#include "../Rasterizer/Vertex.h"
#include "../Rasterizer/Triangle.h"

namespace core
{
    class UStaticMeshComponent : public IComponent
    {
        std::unique_ptr<FPrimitiveList> PrimitiveList;

    public:
        UStaticMeshComponent();
        void OnRegister() override;
        void OnUnregister() override;
        void AddVertex(FVertex Vertex);
        void AddIndex(int32 Index);
        const FPrimitiveList& GetPrimitiveList() const;
        virtual ~UStaticMeshComponent();
    };
}
