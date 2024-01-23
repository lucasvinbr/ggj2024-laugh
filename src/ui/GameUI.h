#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Audio/SoundSource.h>
#include "UIGameScreen.h"

using namespace Urho3D;

namespace Laugh {
	class GameUI : public Object {
		URHO3D_OBJECT(GameUI, Object);
	public:
		GameUI(Context* c);
		~GameUI() override;

		/// <summary>
		/// sets up UI style and shows the initial screen
		/// </summary>
		void SetupUI();

		/// <summary>
		/// returns an instance of the uiScreen of the passed type. Will instantiate it if there isn't a cached one yet
		/// </summary>
		/// <returns></returns>
		template<typename T> T* GetScreen();

		/// <summary>
		/// sets the screen of the passed type to be visible, optionally passing some data to it
		/// </summary>
		/// <param name="uiTypeHash"></param>
		template<typename T> void ShowScreen(String passedData = String::EMPTY);

		static GameUI* instance_;
		HashMap<String, UIGameScreen*> cachedScreens_;

	private:

	};
	template<typename T>
	inline T* GameUI::GetScreen()
	{
		// Compile-time check
		static_assert(std::is_base_of<UIGameScreen, T>::value, "T parameter must derive from UIGameScreen");

		UIGameScreen* targetScreen = nullptr;
		String typeName = T::GetTypeNameStatic();

		if (cachedScreens_.Contains(typeName)) {
			targetScreen = cachedScreens_[typeName];
		}
		else {
			targetScreen = new T(context_);
			targetScreen->Setup();
			cachedScreens_.Insert(Pair<String, UIGameScreen*>(typeName, targetScreen));
		}

		return targetScreen;
	}

	template<typename T>
	inline void GameUI::ShowScreen(String passedData)
	{
		// Compile-time check
		static_assert(std::is_base_of<UIGameScreen, T>::value, "T parameter must derive from UIGameScreen");

		UIGameScreen* targetScreen = nullptr;
		String typeName = T::GetTypeNameStatic();

		if (cachedScreens_.Contains(typeName)) {
			targetScreen = cachedScreens_[typeName];
		}
		else {
			targetScreen = new T(context_);
			targetScreen->Setup();
			cachedScreens_.Insert(Pair<String, UIGameScreen*>(typeName, targetScreen));
		}

		targetScreen->Show(passedData);
	}
}