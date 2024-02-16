#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UIMainMenu.h"
#include "../GameUI.h"
#include "../../GameAudio.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include "UILoading.h"
#include "UISettings.h"
#include "UIRecipes.h"

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
		buttonPlay->SetStyleAuto();
		SubscribeToEvent(buttonPlay, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandlePlayButton));

		auto buttonExit = instanceRoot_->GetChild("ButtonExit", true);
		buttonExit->SetStyleAuto();
		SubscribeToEvent(buttonExit, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandleExitButton));

		auto buttonConfig = instanceRoot_->GetChild("ButtonSettings", true);
		buttonConfig->SetStyleAuto();
		SubscribeToEvent(buttonConfig, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandleConfigButton));

		auto buttonRecipes = instanceRoot_->GetChild("ButtonrRecipes", true);
		buttonRecipes->SetStyleAuto();
		SubscribeToEvent(buttonRecipes, E_RELEASED, URHO3D_HANDLER(UIMainMenu, HandleRecipesButton));
	}

	void UIMainMenu::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);

		GameAudio::instance_->PlayTitleSound();

		World::instance_->topLeftArm_->SetOnScreen(true);
		World::instance_->topRightArm_->SetOnScreen(true);
		World::instance_->botLeftArm_->SetOnScreen(true);
		World::instance_->botRightArm_->SetOnScreen(true);
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

		World::instance_->topLeftArm_->SetOnScreen(false);
		World::instance_->topRightArm_->SetOnScreen(false);
		World::instance_->botLeftArm_->SetOnScreen(false);
		World::instance_->botRightArm_->SetOnScreen(false);

		GameAudio::instance_->PlayClickSound();

		World::instance_->mixer_->RemoveLid();
	}

	void UIMainMenu::HandleExitButton(StringHash, VariantMap& eventData)
	{
		if (GetPlatform() != "Web")
			context_->GetSubsystem<Engine>()->Exit();
	}

	void UIMainMenu::HandleConfigButton(StringHash, VariantMap& eventData)
	{
		Hide();
		GameUI::instance_->ShowScreen<UISettings>();

		GameAudio::instance_->PlayClickSound();
	}

	void UIMainMenu::HandleRecipesButton(StringHash, VariantMap& eventData)
	{
		Hide();
		GameUI::instance_->ShowScreen<UIRecipes>();

		GameAudio::instance_->PlayClickSound();
	}

}