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
	enum InputType {
		DISABLED,
		KEYBOARD,
		MOUSE,
		JOYSTICK
	};

	struct InputEntryHandler {
	public:
		bool IsPressed(Input* input) {
			bool pressed = false;

			if (bindingType_ != DISABLED) {
				switch (bindingType_)
				{
				case Laugh::DISABLED:
					break;
				case Laugh::KEYBOARD:
					pressed = input->GetKeyPress((Key)keybinding_);
					break;
				case Laugh::MOUSE:
					pressed = input->GetMouseButtonPress((MouseButton)keybinding_);
					break;
				case Laugh::JOYSTICK:
					// TOOD handle joystick
					//pressed = input->GetJoystick((joystickb)keybinding_);
					break;
				default:
					break;
				}
			}

			if (pressed) return true;

			if (bindingTypeAlt_ != DISABLED) {
				switch (bindingTypeAlt_)
				{
				case Laugh::DISABLED:
					break;
				case Laugh::KEYBOARD:
					pressed = input->GetKeyPress((Key)keybindingAlt_);
					break;
				case Laugh::MOUSE:
					pressed = input->GetMouseButtonPress((MouseButton)keybindingAlt_);
					break;
				case Laugh::JOYSTICK:
					// TOOD handle joystick
					//pressed = input->GetJoystick((joystickb)keybinding_);
					break;
				default:
					break;
				}
			}

			return pressed;
		}

		bool IsDown(Input* input) {
			bool down = false;

			if (bindingType_ != DISABLED) {
				switch (bindingType_)
				{
				case Laugh::DISABLED:
					break;
				case Laugh::KEYBOARD:
					down = input->GetKeyDown((Key)keybinding_);
					break;
				case Laugh::MOUSE:
					down = input->GetMouseButtonDown((MouseButton)keybinding_);
					break;
				case Laugh::JOYSTICK:
					// TOOD handle joystick
					//pressed = input->GetJoystick((joystickb)keybinding_);
					break;
				default:
					break;
				}
			}

			if (down) return true;

			if (bindingTypeAlt_ != DISABLED) {
				switch (bindingTypeAlt_)
				{
				case Laugh::DISABLED:
					break;
				case Laugh::KEYBOARD:
					down = input->GetKeyDown((Key)keybindingAlt_);
					break;
				case Laugh::MOUSE:
					down = input->GetMouseButtonDown((MouseButton)keybindingAlt_);
					break;
				case Laugh::JOYSTICK:
					// TOOD handle joystick
					//pressed = input->GetJoystick((joystickb)keybinding_);
					break;
				default:
					break;
				}
			}

			return down;
		}

		unsigned int keybinding_;
		InputType bindingType_;
		unsigned int keybindingAlt_;
		InputType bindingTypeAlt_;
	};
}