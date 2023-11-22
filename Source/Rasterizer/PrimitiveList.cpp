#include "PrimitiveList.h"

namespace core
{
    FPrimitiveList::FPrimitiveList():
    IndexCursor(0)
    {

    }

    FPrimitiveList& FPrimitiveList::operator=(const FPrimitiveList& PrimitiveList)
    {
        IndexCursor = PrimitiveList.IndexCursor;

        if (VertexList.size() != PrimitiveList.VertexList.size())
        {
            VertexList.clear();

            for (SIZE_T i = 0; i < PrimitiveList.VertexList.size(); ++i)
            {
                VertexList.push_back(PrimitiveList.VertexList[i]);
            }
        }
        else
        {
            for (SIZE_T i = 0; i < PrimitiveList.VertexList.size(); ++i)
            {
                VertexList[i] = PrimitiveList.VertexList[i];
            }
        }

        if (IndexList.size() != PrimitiveList.IndexList.size())
        {
            IndexList.clear();

            for (SIZE_T i = 0; i < PrimitiveList.IndexList.size(); ++i)
            {
                IndexList.push_back(PrimitiveList.IndexList[i]);
            }
        }
        else
        {
            for (SIZE_T i = 0; i < PrimitiveList.IndexList.size(); ++i)
            {
                IndexList[i] = PrimitiveList.IndexList[i];
            }
        }

        return *this;
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
