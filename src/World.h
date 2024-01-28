#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include "GameConsts.h"
#include "WorldEffects.h"
#include "dataObjs/RecipeData.h"
#include "TentacleArm.h"
#include "Mixer.h"

using namespace Urho3D;

namespace Laugh {

	class World : public Object {
		URHO3D_OBJECT(World, Object);
	public:
		World(Context* c);
		~World() override;

		void CreateScene();

		void CreateDynamicContent();

		void Cleanup();

		void HandlePostRenderUpdate(StringHash, VariantMap& eventData);

		Scene* scene_;
		PhysicsWorld* physWorld_;
		Node* dynamicContentParent_;
		SharedPtr<Viewport> viewport_;
		SharedPtr<RecipesData> recipesData_;
		GameState curGameState_;
		WorldEffects effects_;

		SharedPtr<TentacleArm> topLeftArm_, topRightArm_, botLeftArm_, botRightArm_;

		Mixer* mixer_;

		/// <summary>
		/// singleton, initialized on MainApp startup
		/// </summary>
		static World* instance_;

		static Camera* camera_;

		/// <summary>
		/// if someone goes below this Y value, they should be despawned
		/// </summary>
		const int KILL_Y = -1000;

		Vector<String> pickedIngredients_;
};
}