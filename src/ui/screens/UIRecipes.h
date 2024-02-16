#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Sprite.h>
#include "../../World.h"
#include "../UIGameScreen.h"
#include "../../dataObjs/RecipeData.h"
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Slider.h>

using namespace Urho3D;

namespace Laugh {
	class UIRecipes: public UIGameScreen {
		URHO3D_OBJECT(UIRecipes, UIGameScreen);
	public:
		UIRecipes(Context* c);
		~UIRecipes() override;

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

		void HandleCloseButton(StringHash, VariantMap& eventData);
		void HandleResetSettingsButton(StringHash, VariantMap& eventData);
		void HandleVolumeSlider(StringHash, VariantMap& eventData);
	private:
		Slider* volumeSlider_;
		UIElement* closeBtn_;
		UIElement* resetGameProgressBtn_;
	};
}