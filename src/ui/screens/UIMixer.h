#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/UI/UIElement.h>
#include "../UIGameScreen.h"

using namespace Urho3D;

namespace Laugh {
	class UIMixer: public UIGameScreen {
		URHO3D_OBJECT(UIMixer, UIGameScreen);
	public:
		UIMixer(Context* c);
		~UIMixer() override;

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

		/// <summary>
		/// checks the ingredients provided and mixes them, doing anims to show the result
		/// </summary>
		void MixIngredients();

		void HandleStartMixButton(StringHash, VariantMap& eventData);
		void HandlePickIngredientButton(StringHash, VariantMap& eventData);
	private:
		Node* scenarioStuffParent_;

		Vector<Ingredient> pickedIngredients_;

		Vector<UIElement*> pickIngredientBtns_;
		UIElement* startMixingBtn_;
	};
}