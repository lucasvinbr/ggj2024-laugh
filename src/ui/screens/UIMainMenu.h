#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/UI/UIElement.h>
#include "../UIGameScreen.h"

using namespace Urho3D;

namespace Laugh {
	class UIMainMenu: public UIGameScreen {
		URHO3D_OBJECT(UIMainMenu, UIGameScreen);
	public:
		UIMainMenu(Context* c);
		~UIMainMenu() override;

		/// <summary>
		/// caches references to relevant UI elements of this screen and any other initialization that doesn't depend on the data provided when showing the screen
		/// </summary>
		virtual void Setup() override;

		/// <summary>
		/// should set the screen as visible, optionally with extra configuration options defined in dataPassed
		/// </summary>
		/// <param name="dataPassed"></param>
		virtual void Show(String dataPassed = String::EMPTY) override;

		/// <summary>
		/// does any procedure necessary to make the screen no longer visible. Should also set isShown to false
		/// </summary>
		virtual void Hide() override;

		void HandlePlayButton(StringHash, VariantMap& eventData);
		void HandleExitButton(StringHash, VariantMap& eventData);
	private:

	};
}