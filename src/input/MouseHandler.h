#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Core/ProcessUtils.h>


using namespace Urho3D;

namespace Laugh {
	class MouseHandler : public Object {
		URHO3D_OBJECT(MouseHandler, Object);
	public:
		MouseHandler(Context* c);
		~MouseHandler() override = default;

		// If the user clicks the canvas, attempt to switch to relative mouse mode on web platform
		void HandleMouseModeRequest(const StringHash eventType, VariantMap& eventData);
		void HandleMouseModeChange(const StringHash eventType, VariantMap& eventData);
		void SetMouseMode(MouseMode newMode);
		void SetupMouseEvents();

		MouseMode mouseMode_;

		static MouseHandler* instance_;
};
}