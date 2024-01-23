#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Physics/RigidBody.h>

using namespace Urho3D;

namespace Laugh {
	class WorldEffects : public Object {
		URHO3D_OBJECT(WorldEffects, Object);
	public:
		WorldEffects(Context* context);
		~WorldEffects() override = default;

		void Initialize(Scene* scene);
		void Cleanup();
		void SpawnShotTracer(Vector3 startPos, Vector3 endPos, float travelDuration);
		void SpawnOneShotParticleEffect(const Vector3& worldPos, const String& effectPath) const;
		/// <summary>
		/// after the specified time, the prop will have ignore collisions and be removed after a short period.
		/// the idea is that the prop is "buried" before disappearing
		/// </summary>
		/// <param name="propBody"></param>
		/// <param name="timeToDestroy"></param>
		void RegisterPhysicsPropForTimedRemoval(RigidBody* propBody, float timeToDestroy);

		SharedPtr<Node> effectsParent_;

	private:
		struct DespawningPhysicsProp {
			RigidBody* propBody_;
			float timeToDestroy_;
		};

		void HandleUpdateEffects(StringHash, VariantMap& eventData);

		SharedPtr<Engine> engine_;
		PODVector<DespawningPhysicsProp> despawningProps_;

		const float TIME_BURYING_PROP_BEFORE_DESPAWN = 2.0f;
};
}