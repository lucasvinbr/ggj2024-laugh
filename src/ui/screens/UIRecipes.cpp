#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UIRecipes.h"
#include "../GameUI.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Scene/ObjectAnimation.h>
#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Scene/SceneEvents.h>
#include "UIMixer.h"
#include "../../GameAudio.h"
#include "UIMainMenu.h"

using namespace Urho3D;

namespace Laugh {

	Laugh::UIRecipes::UIRecipes(Context* c) : UIGameScreen(c)
	{
		volumeSlider_ = nullptr;
		resetGameProgressBtn_ = nullptr;
		closeBtn_ = nullptr;

		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_recipes.xml");

		instanceRoot_ = ui->LoadLayout(uiFile);
		ui->GetRoot()->AddChild(instanceRoot_);
		instanceRoot_->SetVisible(false);
		isShown_ = false;
	}

	UIRecipes::~UIRecipes()
	{
		instanceRoot_->Remove();
	}

	void UIRecipes::Setup()
	{
		auto ui = context_->GetSubsystem<UI>();
		auto cache = context_->GetSubsystem<ResourceCache>();

		closeBtn_ = instanceRoot_->GetChild("saveSettsBtn", true);
		closeBtn_->SetStyleAuto();
		SubscribeToEvent(closeBtn_, E_RELEASED, URHO3D_HANDLER(UIRecipes, HandleCloseButton));

		resetGameProgressBtn_ = instanceRoot_->GetChild("resetGameProgressBtn", true);
		resetGameProgressBtn_->SetStyleAuto();
		SubscribeToEvent(resetGameProgressBtn_, E_RELEASED, URHO3D_HANDLER(UIRecipes, HandleResetSettingsButton));

		volumeSlider_ = static_cast<Slider*>(instanceRoot_->GetChild("audioSlider", true));
		volumeSlider_->SetStyleAuto();
		SubscribeToEvent(volumeSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(UIRecipes, HandleVolumeSlider));
	}

	void UIRecipes::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);
		auto cache = context_->GetSubsystem<ResourceCache>();

		volumeSlider_->SetValue(World::instance_->userData_->audioVolume_);
	}

	void UIRecipes::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
	}

	void UIRecipes::HandleCloseButton(StringHash, VariantMap& eventData)
	{
		World::instance_->SaveUserData();

		Hide();
		GameUI::instance_->ShowScreen<UIMainMenu>();
	}

	void UIRecipes::HandleResetSettingsButton(StringHash, VariantMap& eventData)
	{
	}

	void UIRecipes::HandleVolumeSlider(StringHash, VariantMap& eventData)
	{
		using namespace SliderChanged;

		float newValue = eventData[P_VALUE].GetFloat();
		GameAudio::instance_->SetSoundVolume(newValue);

		GameAudio::instance_->PlayClickSound();
	}


}