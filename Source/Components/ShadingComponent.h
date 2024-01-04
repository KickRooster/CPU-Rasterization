#pragma once
#include <Core.h>
#include "IComponent.h"
#include "../Rasterizer/IrradianceBuffer.h"
#include "../Assets/Texture.h"
#include "../Math/Vector2.h"
#include "../Rasterizer/Sampler.h"

namespace core
{
    class UShadingComponent : public IComponent
    {
    private:
        std::unique_ptr<FTexture> Albedo;
        std::unique_ptr<USampler> AlbedoSampler;

    public:
        UShadingComponent();
        void OnRegister() override;
        void OnUnregister() override;
        void SetAlbedo(std::unique_ptr<FTexture> Albedo);
        FHDRColor Shading(const FVector2& UV0) const;
        ~UShadingComponent();
    };
}
