#pragma once
#include <Core.h>
#include <string>

#include "../Assets/Texture.h"
#include "../DesignPattern/SingletonPattern/Singleton.h"

namespace core
{
    class UAssetManager : public Singleton<UAssetManager>
    {
    private:
        std::string PathPrefix = "D:\\PathTracer\\Assets\\Textures\\";

    public:
        bool LoadTexture(const std::string& TextureName, FTexture* OutTexture) const;
    };
}
