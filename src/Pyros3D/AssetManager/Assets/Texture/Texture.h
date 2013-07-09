//============================================================================
// Name        : Texture.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : Asset Interface
//============================================================================

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../IAsset.h"
#include "../../../Core/Logs/Log.h"
#include <SFML/Graphics.hpp>
namespace p3d {
    
    class Texture : public IAsset {
        
        private:
            
            // Internal ID for GL
            int32 GL_ID;
            // FileName
            std::string FileName;
            uint32 Type;
            uint32 SubType;
            int32 Width;
            int32 Height;
            sf::Image Image;
            bool haveImage;
            bool isMipMap;

            // GL Properties
            uint32 Transparency;
            uint32 MinFilter;
            uint32 MagFilter;
            uint32 SRepeat;
            uint32 TRepeat;
            uint32 mode;
            uint32 subMode;
            uint32 internalFormat, internalFormat2, internalFormat3;
            f32 Anysotropic;
        
        
        public:
            // Constructor
            Texture();
            
            // Texture
            bool LoadTexture(const std::string& FileName, const uint32 &Type,const uint32 &SubType, bool Mipmapping = true); 
            bool CreateTexture(const uint32 &Type,const uint32 &SubType, const int32 &width = 0, const int32 &height = 0, bool Mipmapping = true);
            bool CreateTexture(bool Mipmapping = true);
            void SetMinMagFilter(const uint32 &MinFilter,const uint32 &MagFilter);
            void SetRepeat(const uint32 &WrapS,const uint32 &WrapT);
            void EnableCompareMode();
            void SetAnysotropy(const f32 &Anysotropic);
            void SetTransparency(const f32 &Transparency);
            void Resize(const uint32 &Width, const uint32 &Height);
            void UpdateData(void* srcPTR);
            void UpdateMipmap();
            
            const uint32 GetBindID() const;
            
            // Use Asset
            void Bind();
            void Unbind();
            void DeleteTexture();            
            
            // Get Last Binded Texture
            static uint32 GetLastBindedUnit();
                        
            virtual ~Texture();
            
            virtual void Dispose();
            
            // Keep Unit Binded
            static uint32 LastUnitBinded, UnitBinded;
        
    };
    
};

#endif /* TEXTURE_H */