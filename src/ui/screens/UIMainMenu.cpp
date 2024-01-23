#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UIMainMenu.h"
#include "../GameUI.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include "UILoading.h"

using namespace Urho3D;

namespace Laugh {

	Laugh::UIMainMenu::UIMainMenu(Context* c) : UIGameScreen(c)
	{
		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_title.xml");

		instanceRoot_ = ui->LoadLayout(uiFile);
		ui->GetRoot()->AddChild(instanceRoot_);
		instanceRoot_->SetVisible(false);
		isShown_ = false;
	}

	UIMainMenu::~UIMainMenu()
	{
		instanceRoot_->Remove();
	}

	void UIMainMenu::Setup()
	{
		auto buttonPlay = instanceRoot_->GetChild("ButtonPlay", true);
		SubscribeToEvent(buttonPlay, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandlePlayButton));

		auto buttonExit = instanceRoot_->GetChild("ButtonExit", true);
		SubscribeToEvent(buttonExit, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandleExitButton));
	}

	void UIMainMenu::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);
	}

	void UIMainMenu::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
	}

	void UIMainMenu::HandlePlayButton(StringHash, VariantMap& eventData)
	{
		Hide();
		GameUI::instance_->ShowScreen<UILoading>();
	}

	void UIMainMenu::HandleExitButton(StringHash, VariantMap& eventData)
	{
		if (GetPlatform() != "Web")
			context_->GetSubsystem<Engine>()->Exit();
	}

}