#pragma once
#include "MouseHandler.h"


using namespace Urho3D;

namespace Laugh {
	MouseHandler* MouseHandler::instance_ = nullptr;

	Laugh::MouseHandler::MouseHandler(Context* c) : Object(c),
		mouseMode_(MM_FREE)
	{
		instance_ = this;
	}

	void MouseHandler::HandleMouseModeRequest(const StringHash eventType, VariantMap& eventData)
	{
		auto console = context_->GetSubsystem<Console>();

		if (console && console->IsVisible()) {
			return;
		}

		auto input = context_->GetSubsystem<Input>();

		if (input->GetMouseMode() == MM_ABSOLUTE) {
			input->SetMouseVisible(false);
		}
		else if (input->GetMouseMode() == MM_FREE) {
			input->SetMouseVisible(true);
		}

		input->SetMouseMode(mouseMode_);
	}

	void MouseHandler::HandleMouseModeChange(const StringHash eventType, VariantMap& eventData)
	{
		bool mouseLocked = eventData[MouseModeChanged::P_MOUSELOCKED].GetBool();

		context_->GetSubsystem<Input>()->SetMouseVisible(!mouseLocked);
	}

	void MouseHandler::SetMouseMode(MouseMode newMode)
	{
		mouseMode_ = newMode;

		auto input = context_->GetSubsystem<Input>();

		if (mouseMode_ == MM_FREE) {
			input->SetMouseVisible(true);
		}

		input->SetMouseMode(mouseMode_);

		if (mouseMode_ != MM_ABSOLUTE) {
			auto console = context_->GetSubsystem<Console>();

			if (console && console->IsVisible()) {
				input->SetMouseMode(MM_ABSOLUTE, true);
			}
		}

		VariantMap emptyMap = VariantMap();
		HandleMouseModeRequest(StringHash::ZERO, emptyMap);
	}

	void MouseHandler::SetupMouseEvents()
	{
		SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(MouseHandler, HandleMouseModeRequest));
		SubscribeToEvent(E_MOUSEMODECHANGED, URHO3D_HANDLER(MouseHandler, HandleMouseModeChange));
	}
}
