//============================================================================
// Name        : DeferredRenderer.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : Deferred Renderer
//============================================================================

#include "DeferredRenderer.h"
#ifdef ANDROID
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#else
    #include "GL/glew.h"
#endif

namespace p3d {
    
    DeferredRenderer::DeferredRenderer(const uint32& Width, const uint32& Height, FrameBuffer* fbo) : IRenderer(Width,Height) 
    {
    
        echo("SUCCESS: Deferred Renderer Created");
        
        ActivateCulling(CullingMode::FrustumCulling);

        shadowMaterial = new GenericShaderMaterial(ShaderUsage::CastShadows);
        shadowMaterial->SetCullFace(CullFace::DoubleSided);
        shadowSkinnedMaterial = new GenericShaderMaterial(ShaderUsage::CastShadows | ShaderUsage::Skinning);
        shadowSkinnedMaterial->SetCullFace(CullFace::DoubleSided);

        // Default View Port Init Values
        viewPortStartX = viewPortStartY = 0;

        // Save FrameBuffer
        FBO = fbo;


        // Create Second Pass Specifics
            pointLightMaterial = new GenericShaderMaterial(ShaderUsage::Color);
            pointLightMaterial->SetColor(Vec4(1,0,0,1));
            deferredMaterial = new GenericShaderMaterial(ShaderUsage::DeferredMaterial);

            // Set Textures
            deferredMaterial->AddTexture("tDifuse",FBO->GetAttachments()[FrameBufferAttachmentFormat::Color_Attachment0]->TexturePTR);
            deferredMaterial->AddTexture("tSpecular",FBO->GetAttachments()[FrameBufferAttachmentFormat::Color_Attachment1]->TexturePTR);
            deferredMaterial->AddTexture("tNormal",FBO->GetAttachments()[FrameBufferAttachmentFormat::Color_Attachment2]->TexturePTR);
            deferredMaterial->AddTexture("tPosition",FBO->GetAttachments()[FrameBufferAttachmentFormat::Color_Attachment3]->TexturePTR);
            deferredMaterial->AddTexture("tDepth",FBO->GetAttachments()[FrameBufferAttachmentFormat::Depth_Attachment]->TexturePTR);
            deferredMaterial->AddUniform(Uniform::Uniform("uScreenDimensions", Uniform::DataUsage::ScreenDimensions));
            // Quad
            // Dummy GO
            Quad = new GameObject();
            deferredQuad = new RenderingComponent(AssetManager::CreatePlane(2,2), deferredMaterial);

            // Light Volumes
            pointLight = new RenderingComponent(AssetManager::CreateSphere(1));
    }
    
    void DeferredRenderer::Resize(const uint32& Width, const uint32& Height)
    {
        IRenderer::Resize(Width,Height);
    }
    
    DeferredRenderer::~DeferredRenderer()
    {
        if (IsCulling)
        {
            delete culling;
        }
        
        delete shadowMaterial;
    }
    
    void DeferredRenderer::CreateQuad()
    {

    }
    
    std::vector<RenderingMesh*> DeferredRenderer::GroupAndSortAssets(SceneGraph* Scene, GameObject* Camera)
    {
        
        // Sort and Group Objects From Scene
        std::vector<RenderingMesh*> _OpaqueMeshes;
        std::map<f32,RenderingMesh*> _TranslucidMeshes;
        
        std::vector<RenderingMesh*> rmeshes = RenderingComponent::GetRenderingMeshes(Scene);
        
        for (std::vector<RenderingMesh*>::iterator k=rmeshes.begin();k!=rmeshes.end();k++)
        {
            if ((*k)->Material->IsTransparent())
            {
                f32 index = Camera->GetPosition().distanceSQR((*k)->renderingComponent->GetOwner()->GetWorldPosition());
                while(_TranslucidMeshes.find(index)!=_TranslucidMeshes.end()) index+=1.f;
                _TranslucidMeshes[index] = (*k);
            }
            else _OpaqueMeshes.push_back((*k));
        }
        
        for (std::map<f32,RenderingMesh*>::iterator i=_TranslucidMeshes.begin();i!=_TranslucidMeshes.end();i++)
        {
            _OpaqueMeshes.push_back((*i).second);
        }
        
        return _OpaqueMeshes;
         
    }
    
    void DeferredRenderer::RenderScene(const p3d::Projection& projection, GameObject* Camera, SceneGraph* Scene, const uint32 BufferOptions)
    {
        
        // Initialize Renderer
        InitRender();
        
        // Group and Sort Meshes
        std::vector<RenderingMesh*> rmesh = GroupAndSortAssets(Scene, Camera);
        
        // Get Lights List
        std::vector<IComponent*> lcomps = ILightComponent::GetLightsOnScene(Scene);
        
        // Save Time
        Timer = Scene->GetTime();

        // First Pass

            // Save Values for Cache
            // Saves Scene
            this->Scene = Scene;

            // Saves Camera
            this->Camera = Camera;
            this->CameraPosition = this->Camera->GetWorldPosition();

            // Saves Projection
            this->projection = projection;

            // Universal Cache
            ProjectionMatrix = projection.m;
            NearFarPlane = Vec2(projection.Near, projection.Far);

            // View Matrix and Position
            ViewMatrix = Camera->GetWorldTransformation().Inverse();
            CameraPosition = Camera->GetWorldPosition();

            // Update Culling
            UpdateCulling(ProjectionMatrix*ViewMatrix);

            // Flags
            ViewMatrixInverseIsDirty = true;
            ProjectionMatrixInverseIsDirty = true;
            ViewProjectionMatrixIsDirty = true;

            // Bind Frame Buffer
            FBO->Bind();

            // Set ViewPort
            if (viewPortEndX==0 || viewPortEndY==0) 
            { 
                viewPortEndX = Width;
                viewPortEndY = Height;
            }
            
            // Set Viewport
            _SetViewPort(viewPortStartX,viewPortStartY,viewPortEndX,viewPortEndY);

            // Enable Depth Masking
            glDepthMask(GL_TRUE);

            // Clear depth buffer
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            // Enable Depth Test
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glClearDepth(1.f);

            // Disable Blending
            glDisable(GL_BLEND);

            // Draw Background
            DrawBackground();

            // Render Scene with Objects Material
            for (std::vector<RenderingMesh*>::iterator i=rmesh.begin();i!=rmesh.end();i++)
            {

                if ((*i)->renderingComponent->GetOwner()!=NULL)
                {
                    // Culling Test
                    bool cullingTest = false;
                    switch((*i)->CullingGeometry)
                    {
                        case CullingGeometry::Box:
                            cullingTest = CullingBoxTest((*i),(*i)->renderingComponent->GetOwner());
                            break;
                        case CullingGeometry::Sphere:
                        default:
                            cullingTest = CullingSphereTest((*i), (*i)->renderingComponent->GetOwner());
                            break;
                    }
                    if (cullingTest && (*i)->renderingComponent->IsActive() && (*i)->Active == true)
                        RenderObject((*i),(*i)->renderingComponent->GetOwner(),(*i)->Material);
                }
            }
            // Disable Cull Face
            glDisable(GL_CULL_FACE);

    #ifndef ANDROID
            // Set Default Polygon Mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    #endif

            // End Rendering
            EndRender();

            InitRender();

            // Unbind FrameBuffer
            FBO->UnBind();

            // Disable Depth Masking
            glDepthMask(GL_FALSE);

            // Disable Depth Test
            glDisable(GL_DEPTH_TEST);

            // Second Pass

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);

            glClear(GL_COLOR_BUFFER_BIT);

            // Draw Quad
            // Render Scene with Objects Material
            for (std::vector<IComponent*>::iterator i=lcomps.begin();i!=lcomps.end();i++)
            {

                if ((*i)->GetOwner()!=NULL)
                {
                    if (PointLight* p = dynamic_cast<PointLight*>((*i))) {
                        // Point Lights
                            // Set Scale
                            pointLight->GetMeshes()[0]->Pivot.Scale(p->GetLightRadius(),p->GetLightRadius(),p->GetLightRadius());
                            RenderObject(pointLight->GetMeshes()[0],p->GetOwner(),deferredMaterial);
                    }
                    else if (SpotLight* s = dynamic_cast<SpotLight*>((*i))) {
                        // Spot Lights


                    }
                    else if (DirectionalLight* d = dynamic_cast<DirectionalLight*>((*i))) {
                        // Directional Lights


                    }
                }
            }

            glEnable (GL_DEPTH_TEST);
            glDepthMask (GL_TRUE);
            glDisable (GL_BLEND);

            EndRender();
    }

    void DeferredRenderer::SetFBO(FrameBuffer* fbo)
    {
        // Save FBO
        FBO = fbo;
    }
    
};
