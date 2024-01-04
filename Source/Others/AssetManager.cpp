#include "AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParay/stb/stb_image.h"

namespace core
{
    bool UAssetManager::LoadTexture(const std::string& TextureName, FTexture* OutTexture) const
    {
        std::string FullPathName = PathPrefix + TextureName;

        int32 Width;
        int32 Height;
        int32 ComponentCount;

        // Basic usage (see HDR discussion below for HDR usage):
        //    int x,y,n;
        //    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
        //    // ... process data if not NULL ...
        //    // ... x = width, y = height, n = # 8-bit components per pixel ...
        //    // ... replace '0' with '1'..'4' to force that many components per pixel
        //    // ... but 'n' will always be the number that it would have been if you said 0
        //    stbi_image_free(data);

        uint8 *TextureData = stbi_load(FullPathName.c_str(), &Width, &Height, &ComponentCount, 0);

        if (!TextureData)
        {
            return false;
        }

        OutTexture->Width = Width;
        OutTexture->Height = Height;
        OutTexture->ComponentCount = ComponentCount;

        OutTexture->Data = new uint8[Width * Height * ComponentCount];
        memcpy(OutTexture->Data, TextureData, sizeof(uint8) * Width * Height * ComponentCount);
        stbi_image_free(TextureData);

        return true;
    }
}
