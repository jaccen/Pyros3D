//============================================================================
// Name        : ShaderLib.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : ShaderLib
//============================================================================

#ifndef GENERICSHADERMATERIAL_H
#define GENERICSHADERMATERIAL_H

#include "../IMaterial.h"
#include "ShaderLib.h"

#include <iostream>
#include <map>

namespace p3d
{     
    class GenericShaderMaterial : public IMaterial
    {
        public:
            
            GenericShaderMaterial(const uint32 &options);
            virtual ~GenericShaderMaterial();
            // Set Colors
            void SetColor(const Vec4 &color);
            void SetSpecular(const Vec4 &specularColor);
            // Set Textures
            void SetColorMap(const Texture &colormap);
            void SetSpecularMap(const Texture &specular);
            void SetNormalMap(const Texture &normalmap);
            void SetEnvMap(const Texture &envmap);
            void SetRefractMap(const Texture &refractmap);
            void SetSkyboxMap(const Texture &skyboxmap);
            // Lights
            void ChangeLightingProperties(const Vec4 &Ke, const Vec4 &Ka, const Vec4 &Kd, const Vec4 &Ks, const f32 &shininess);

            // Render
            virtual void PreRender();
            virtual void AfterRender();
            
            // Bind
            void BindTextures();
            void UnbindTextures();
        
        private:
        
            // List of Tetxures
            std::map<uint32, Texture> Textures;
        
        protected:
            // Shaders List
            static std::map<uint32, Shaders* > ShadersList;
            // Save Shader Location on Shaders List
            uint32 shaderID;

            // Lighting Properties
            Vec4 Ke;
            Vec4 Ka;
            Vec4 Kd;
            Vec4 Ks;
            f32 Shininess, UseLights;
    };
}

#endif /* GENERICSHADERMATERIAL_H */
