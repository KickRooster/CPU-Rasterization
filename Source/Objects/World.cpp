#include "World.h"

#include "../Rasterizer/Driver.h"

namespace core
{
    void UWorld::AddCameraActor(std::unique_ptr<UCamera> CameraActor)
    {
        DefaultCamera = std::move(CameraActor);
    }

    void UWorld::AddBasicActor(std::unique_ptr<UActor> BasicActor)
    {
        BasicActorList.push_back(std::move(BasicActor));
    }

    void UWorld::Tick(float DeltaTime)
    {
        DefaultCamera->Tick(DeltaTime);

        for (SIZE_T i = 0; i < BasicActorList.size(); ++i)
        {
            BasicActorList[i]->Tick(DeltaTime);
        }
    }

    void UWorld::Render(FIrradianceBuffer& IrradianceBuffer)
    {
        for (SIZE_T i = 0; i < BasicActorList.size(); ++i)
        {
            UDriver::Instance()->DrawActor(*BasicActorList[i], *DefaultCamera, IrradianceBuffer);
        }
    }

    void UWorld::Render_Debug(FIrradianceBuffer& IrradianceBuffer)
    {
        for (SIZE_T i = 0; i < BasicActorList.size(); ++i)
        {
            UDriver::Instance()->Debug(*BasicActorList[i], *DefaultCamera, IrradianceBuffer);
        }
    }
}
