#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UIMixer.h"
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

	Laugh::UIMixer::UIMixer(Context* c) : UIGameScreen(c)
	{
		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_title.xml");

		instanceRoot_ = ui->LoadLayout(uiFile);
		ui->GetRoot()->AddChild(instanceRoot_);
		instanceRoot_->SetVisible(false);
		isShown_ = false;
	}

	UIMixer::~UIMixer()
	{
		instanceRoot_->Remove();
	}

	void UIMixer::Setup()
	{
		// create scenario stuff parent... and the scenario stuff
		scenarioStuffParent_ = World::instance_->dynamicContentParent_->CreateChild("uiMixer_scenario");


		// find ingredient buttons container
		// create buttons for each ingredient, and add the event to each of them

	}

	void UIMixer::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);
		scenarioStuffParent_->SetEnabledRecursive(true);
	}

	void UIMixer::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
		scenarioStuffParent_->SetEnabledRecursive(false);
	}

	void UIMixer::MixIngredients()
	{
		// find the recipe with most matching ingredients. that's the one we will show as result
	}

	void UIMixer::HandleStartMixButton(StringHash, VariantMap& eventData)
	{
		// if not yet mixing and got enough ingredients, start mixing
	}

	void UIMixer::HandlePickIngredientButton(StringHash, VariantMap& eventData)
	{
		// if not enough ingredients picked yet, add ingredient, do anim.
		// if enough ingredients picked now, hide ingredients list, show mix button
	}

}