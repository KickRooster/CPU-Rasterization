#include "PrimitiveList.h"

namespace core
{
    FPrimitiveList::FPrimitiveList():
    IndexCursor(0)
    {

    }

    void FPrimitiveList::AddVertex(FVertex Vertex)
    {
        VertexList.push_back(Vertex);
    }

    void FPrimitiveList::AddIndex(int32 Index)
    {
        IndexList.push_back(Index);
    }

    void FPrimitiveList::BeforeRasterizing()
    {
        IndexCursor = 0;
    }

    bool FPrimitiveList::TopIsValid() const
    {
        if (static_cast<SIZE_T>(IndexCursor) + 2 <= IndexList.size())
        {
            return true ;
        }
        else
        {
            return false;
        }
    }

    FTriangle FPrimitiveList::PopTriangle()
    {
        int32 Index0 = IndexList[IndexCursor++];
        int32 Index1 = IndexList[IndexCursor++];
        int32 Index2 = IndexList[IndexCursor++];

        FTriangle Triangle(VertexList[Index0], VertexList[Index1], VertexList[Index2]);

        return Triangle;
    }

    void FPrimitiveList::PostRasterizing()
    {

    }
}
