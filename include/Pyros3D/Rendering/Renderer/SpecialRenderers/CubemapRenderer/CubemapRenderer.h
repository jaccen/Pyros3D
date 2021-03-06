//============================================================================
// Name        : CubemapRenderer.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : Dynamic Cube Map aka Environment Map
//============================================================================

#ifndef CUBEMAPRENDERER_H
#define CUBEMAPRENDERER_H

#include <Pyros3D/Rendering/Renderer/IRenderer.h>
#include <Pyros3D/Core/Projection/Projection.h>

namespace p3d {

	class PYROS3D_API CubemapRenderer : public IRenderer {

	public:

		CubemapRenderer(const uint32 Width, const uint32 Height);

		~CubemapRenderer();
		
		void RenderCubeMap(SceneGraph* Scene, GameObject* AllSeeingEye, const f32 Near, const f32 Far);
		
		Texture* GetTexture();

	protected:
		
		virtual void RenderScene(const p3d::Projection &projection, GameObject* Camera, SceneGraph* Scene) {}

		GameObject* AllSeeingEye;
		Texture* environmentMap;
		FrameBuffer* fbo;

	};

};

#endif /* CUBEMAPRENDERER_H */
