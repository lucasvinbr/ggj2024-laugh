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
	class TentacleArm : public LogicComponent {
		URHO3D_OBJECT(TentacleArm, LogicComponent);
	public:
		TentacleArm(Context* c);
		~TentacleArm() = default;

		static void RegisterObject(Context* context);

		void Setup(bool isTop, bool isLeft);
		void GrabSprite(Sprite2D* spriteToGrab);

		void SpecialGrabLidAnim();

		void Update(float timeStep);

	private:
		AnimatedSprite2D* armSprite_;
		Node* attachmentPoint_;
};
}