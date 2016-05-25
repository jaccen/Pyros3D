//============================================================================
// Name        : PickingPainterMethod.h
// Author      : Duarte Peixinho
// Version     :
// Copyright   : ;)
// Description : PickingExample With Painter Method
//============================================================================

#ifndef PICKINGPAINTERMETHOD_H
#define	PICKINGPAINTERMETHOD_H

#if defined(_SDL)
	#include "../WindowManagers/SDL/SDLContext.h"
	#define ClassName SDLContext
#elif defined(_SDL2)
	#include "../WindowManagers/SDL2/SDL2Context.h"
	#define ClassName SDL2Context
#else
	#include "../WindowManagers/SFML/SFMLContext.h"
	#define ClassName SFMLContext
#endif

#include <Pyros3D/Assets/Renderable/Primitives/Shapes/Sphere.h>
#include <Pyros3D/Assets/Renderable/Primitives/Shapes/Cube.h>
#include <Pyros3D/SceneGraph/SceneGraph.h>
#include <Pyros3D/Rendering/Renderer/ForwardRenderer/ForwardRenderer.h>
#include <Pyros3D/Utils/Colors/Colors.h>
#include <Pyros3D/Rendering/Components/Rendering/RenderingComponent.h>
#include <Pyros3D/Rendering/Components/Lights/DirectionalLight/DirectionalLight.h>
#include <Pyros3D/Rendering/Components/Rendering/RenderingComponent.h>
#include <Pyros3D/Utils/Mouse3D/PainterPick.h>
#include <Pyros3D/Rendering/PostEffects/PostEffectsManager.h>
#include <Pyros3D/Rendering/PostEffects/Effects/SSAOEffect.h>
#include <Pyros3D/Rendering/PostEffects/Effects/BlurXEffect.h>
#include <Pyros3D/Rendering/PostEffects/Effects/BlurYEffect.h>

using namespace p3d;

class PickingPainterMethod : public ClassName {

	public:
		
		PickingPainterMethod();
		virtual ~PickingPainterMethod();
		
		virtual void Init();
		virtual void Update();
		virtual void Shutdown();
		virtual void OnResize(const uint32 width, const uint32 height);
		
	private:
		
		// Scene
		SceneGraph* Scene;
		// Renderer
		ForwardRenderer* Renderer;
		// Projection
		Projection projection;
		// Camera - Its a regular GameObject
		GameObject* Camera;
		// GameObject
		std::vector<GameObject*> Cubes;
		std::vector<RenderingComponent*> rCubes;
		Renderable* cubeHandle;

		// Light
		GameObject* Light;
		DirectionalLight* dLight;
		
		// Event On Mouse Release
		void OnMouseRelease(Event::Input::Info e);
		
		// Painter Method
		PainterPick* picking;
		
		// Material for Selected Mesh
		GenericShaderMaterial *UnselectedMaterial, *SelectedMaterial;
		
		// Selected Mesh
		RenderingMesh* SelectedMesh;

		// Effect manager
		PostEffectsManager* EffectManager;


		// Events
		void MoveFrontPress(Event::Input::Info e);
		void MoveBackPress(Event::Input::Info e);
		void StrafeLeftPress(Event::Input::Info e);
		void StrafeRightPress(Event::Input::Info e);
		void MoveFrontRelease(Event::Input::Info e);
		void MoveBackRelease(Event::Input::Info e);
		void StrafeLeftRelease(Event::Input::Info e);
		void StrafeRightRelease(Event::Input::Info e);
		void LookTo(Event::Input::Info e);

		float counterX, counterY;
		Vec2 mouseCenter, mouseLastPosition, mousePosition;
		bool _moveFront, _moveBack, _strafeLeft, _strafeRight;
};

class SSAOEffectFinal : public IEffect {
public:
	SSAOEffectFinal(uint32 texture1, uint32 texture2);
	virtual ~SSAOEffectFinal() {}
};

#endif	/* PICKINGPAINTERMETHOD_H */
