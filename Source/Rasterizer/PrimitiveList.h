#pragma once
#include <Core.h>
#include "Vertex.h"
#include <vector>
#include "Triangle.h"

namespace core
{
    struct FPrimitiveList
    {
    private:
        int32 IndexCursor;

    public:
        std::vector<FVertex> VertexList;
        std::vector<int32> IndexList;

        FPrimitiveList();

        void AddVertex(FVertex Vertex);
        void AddIndex(int32 Index);
        void BeforeRasterizing();
        bool TopIsValid() const;
        FTriangle PopTriangle();
        void PostRasterizing();
    };
}
