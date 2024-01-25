#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>

using namespace Urho3D;

namespace Laugh {
	class Mixer : public LogicComponent {
		URHO3D_OBJECT(Mixer, LogicComponent);
	public:
		Mixer(Context* c);
		~Mixer() = default;

		static void RegisterObject(Context* context);

		void ToggleDisplay(bool show);

		void PlaceIngredient(Sprite2D* ingSprite);

		void Start();
		void Update(float timeStep);

	private:
		AnimatedSprite2D* animatedSprite_;
		Node* attachmentPoint_;
};
}