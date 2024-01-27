#include "GameUI.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/UI/UI.h>
#include "screens/UIMainMenu.h"

using namespace Urho3D;

namespace Laugh {
	GameUI* GameUI::instance_ = nullptr;

	Laugh::GameUI::GameUI(Context* c) : Object(c)
	{
		instance_ = this;
		cachedScreens_ = HashMap<String, UIGameScreen*>();
	}

	GameUI::~GameUI()
	{
		for (auto cachedScr = cachedScreens_.Begin(); cachedScr != cachedScreens_.End();)
		{
			delete cachedScr->second_;

			++cachedScr;
		}
		cachedScreens_.Clear();
	}

	void GameUI::SetupUI()
	{
		auto cache = context_->GetSubsystem<ResourceCache>();
		auto ui = context_->GetSubsystem<UI>();
		//ui->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
		ui->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/ggj2024-laugh/style_laugh.xml"));

		ShowScreen<UIMainMenu>();
	}

}