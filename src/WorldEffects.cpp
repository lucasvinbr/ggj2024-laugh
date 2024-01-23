#pragma once
#include "GameConsts.h"
#include "WorldEffects.h"
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;

namespace Laugh {
	Laugh::WorldEffects::WorldEffects(Context* context) : Object(context)
	{
	}

	void WorldEffects::Initialize(Scene* scene)
	{
		effectsParent_ = scene->CreateChild("effects");
		SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(WorldEffects, HandleUpdateEffects));
	}

	void WorldEffects::Cleanup()
	{
		UnsubscribeFromAllEvents();
		effectsParent_->Remove();
	}

	void WorldEffects::SpawnShotTracer(Vector3 startPos, Vector3 endPos, float travelDuration)
	{
	}

	void WorldEffects::SpawnOneShotParticleEffect(const Vector3& worldPos, const String& effectPath) const
	{
		auto particleNode = effectsParent_->CreateChild("emitter");
		particleNode->SetWorldPosition(worldPos);

		auto emitter = particleNode->CreateComponent<ParticleEmitter>();
		auto cache = GetSubsystem<ResourceCache>();
		emitter->SetEffect(cache->GetResource<ParticleEffect>(effectPath));

		emitter->SetAutoRemoveMode(REMOVE_NODE);
	}

	void WorldEffects::RegisterPhysicsPropForTimedRemoval(RigidBody* propBody, float timeToDestroy)
	{
		despawningProps_.Push({
			propBody,
			timeToDestroy
		});
	}

	void WorldEffects::HandleUpdateEffects(StringHash, VariantMap& eventData)
	{
		using namespace Update;

		float timeStep = eventData[P_TIMESTEP].GetFloat();

		for (auto timedProp = despawningProps_.Begin(); timedProp != despawningProps_.End();) {
			timedProp->timeToDestroy_ -= timeStep;

			if (timedProp->timeToDestroy_ < 0) {
				if (timedProp->timeToDestroy_ >= -timeStep) {
					// timeToDestroy has just reached negative amounts!
					// begin the "clip into ground" part of the procedure
					timedProp->propBody_->SetCollisionLayer(COLLAYER_IGNORE);

				}

				if (timedProp->timeToDestroy_ <= -TIME_BURYING_PROP_BEFORE_DESPAWN) {
					timedProp->propBody_->GetNode()->Remove();
					timedProp = despawningProps_.Erase(timedProp);
					continue;
				}
			}

			++timedProp;
		}
	}
}