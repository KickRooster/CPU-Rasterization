#include "ShadingComponent.h"

namespace core
{
    UShadingComponent::UShadingComponent()
    {
        AlbedoSampler = std::make_unique<USampler>();
    }

    void UShadingComponent::OnRegister()
    {

    }

    void UShadingComponent::OnUnregister()
    {

    }

    void UShadingComponent::SetAlbedo(std::unique_ptr<FTexture> Albedo)
    {
        this->Albedo = std::move(Albedo);
    }

    FHDRColor UShadingComponent::Shading(const FVector2& UV0) const
    {
        FLDRColor Result = AlbedoSampler->Sample(*Albedo, FE_Nearest, WE_ClmapToEdge, UV0);

        return Result.ToHDRColor();
    }

    UShadingComponent::~UShadingComponent()
    {

    }
}
