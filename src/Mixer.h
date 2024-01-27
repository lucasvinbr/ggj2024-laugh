#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>

using namespace Urho3D;

namespace Laugh {
	enum MixerState {
		IDLE_LID_ON,
		IDLE_LID_OFF,
		REMOVING_LID,
		PUTTING_LID,
		SHAKING
	};

	URHO3D_EVENT(E_MIXER_ANIM_DONE, MixerAnimDone)
	{
		URHO3D_PARAM(P_FINISHED_ANIM, FinishedAnim);            // String
	}

	class Mixer : public LogicComponent {
		URHO3D_OBJECT(Mixer, LogicComponent);
	public:
		Mixer(Context* c);
		~Mixer() = default;

		static void RegisterObject(Context* context);

		void ToggleDisplay(bool show);

		void RemoveLid();
		void StartMixing();

		void PlaceIngredient(Sprite2D* ingSprite);

		void Start();
		void Update(float timeStep);

		void OnDoneAnimating(StringHash, VariantMap& eventData);

	private:
		StaticSprite2D* lidSprite_;
		StaticSprite2D* glassSprite_;
		StaticSprite2D* btnSprite_;
		StaticSprite2D* baseSprite_;
		Vector3 startLidPos_, startGlassPos_, startBtnPos_, startBasePos_, lidRemovedPos_;

		Vector<StaticSprite2D*> ingredientSprites_;

		Node* ingredientsParent_;
		Node* visualsParent_;

		float timeAnimating_;
		MixerState mixerState_;

		const float startShakeTime = 0.35f, finalShakeTime = 0.15f;
		const float startShakeIntensity = 0.15f, finalShakeIntensity = 0.55f, stoppingTime = 0.25f;

		const float MIX_ANIM_TIME = 2.0f;
		const float LID_ANIM_TIME = 0.5f;
};
}