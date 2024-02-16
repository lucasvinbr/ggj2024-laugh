#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UIMixer.h"
#include "../GameUI.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include "UILoading.h"
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Graphics/Texture2D.h>
#include "../../GameAudio.h"
#include "UIMainMenu.h"

using namespace Urho3D;

namespace Laugh {

	Laugh::UIMixer::UIMixer(Context* c) : UIGameScreen(c)
	{
		ingredientsPickerBar_ = nullptr;
		scenarioStuffParent_ = nullptr;
		startMixingBtn_ = nullptr;
		goBackBtn_ = nullptr;

		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_game.xml");

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
		auto ui = context_->GetSubsystem<UI>();
		auto cache = context_->GetSubsystem<ResourceCache>();

		startMixingBtn_ = instanceRoot_->GetChild("blendButton", true);
		startMixingBtn_->SetStyleAuto();
		SubscribeToEvent(startMixingBtn_, E_RELEASED, URHO3D_HANDLER(UIMixer, HandleStartMixButton));
		startMixingBtn_->SetVisible(false);

		goBackBtn_ = instanceRoot_->GetChild("backButton", true);
		goBackBtn_->SetStyleAuto();
		SubscribeToEvent(goBackBtn_, E_RELEASED, URHO3D_HANDLER(UIMixer, HandleGoBackButton));
		goBackBtn_->SetVisible(true);

		// find ingredient buttons container
		// create buttons for each ingredient, and add the event to each of them
		ingredientsPickerBar_ = instanceRoot_->GetChild("SV_ItemContainer_ings", true);
		auto ingButtonXml = cache->GetResource<XMLFile>("UI/ggj2024-laugh/elements/ingEntry_base.xml")->GetRoot();

		auto ingredients = World::instance_->recipesData_->ingredients_;
		for (auto ingEntry = ingredients.Begin(); ingEntry != ingredients.End();) {
			auto ingData = ingEntry->second_;
			auto createdBtn = ingredientsPickerBar_->LoadChildXML(ingButtonXml, nullptr);
			createdBtn->SetStyleAuto();

			auto btnSprite = static_cast<Sprite*>(createdBtn->GetChild("ingSprite", true));
			btnSprite->SetTexture(cache->GetResource<Texture2D>(ingData.imageFilePath_));
			// keep sprite name as ingredient's id
			btnSprite->SetName(ingEntry->first_	);

			pickIngredientBtns_.Push(createdBtn);
			SubscribeToEvent(createdBtn, E_RELEASED, URHO3D_HANDLER(UIMixer, HandlePickIngredientButton));

			++ingEntry;
		}

	}

	void UIMixer::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);

		World::instance_->mixer_->Cleanup();

		startMixingBtn_->SetVisible(false);
		goBackBtn_->SetVisible(true);
		ingredientsPickerBar_->SetVisible(true);
	}

	void UIMixer::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
	}

	void UIMixer::MixIngredients()
	{
		World::instance_->mixer_->StartMixing();

		World::instance_->topLeftArm_->GrabSprite(nullptr);
		World::instance_->topRightArm_->GrabSprite(nullptr);
		World::instance_->botLeftArm_->GrabSprite(nullptr);
		World::instance_->botRightArm_->GrabSprite(nullptr);

		World::instance_->topLeftArm_->SetOnScreen(false);
		World::instance_->topRightArm_->SetOnScreen(false);
		World::instance_->botLeftArm_->SetOnScreen(false);
		World::instance_->botRightArm_->SetOnScreen(false);

	}

	void UIMixer::HandleStartMixButton(StringHash, VariantMap& eventData)
	{
		// if not yet mixing and got enough ingredients, start mixing
		MixIngredients();
		startMixingBtn_->SetVisible(false);

		GameAudio::instance_->PlayClickSound();
	}

	void UIMixer::HandlePickIngredientButton(StringHash, VariantMap& eventData)
	{
		goBackBtn_->SetVisible(false);

		// if not enough ingredients picked yet, add ingredient, do anim.
		// if enough ingredients picked now, hide ingredients list, show mix button
		auto cache = GetSubsystem<ResourceCache>();
		using namespace Released;

		auto pressedBtn = static_cast<UIElement*>(eventData[P_ELEMENT].GetPtr());
		String ingredientId = pressedBtn->GetChild(0)->GetName();

		auto& ingredients = World::instance_->recipesData_->ingredients_;
		auto& pickedIngredient = ingredients[ingredientId];
		auto& pickedIngs = World::instance_->pickedIngredients_;

		URHO3D_LOGDEBUG("pressed ingredient: " + pickedIngredient.name_);
		GameAudio::instance_->PlayPickItemSound();
		

		if (pickedIngs.Size() < 4) {
			
			pickedIngs.Push(ingredientId);

			TentacleArm* holderArm;

			switch (pickedIngs.Size())
			{
			case 1:
				holderArm = World::instance_->topLeftArm_;
				break;
			case 2:
				holderArm = World::instance_->botLeftArm_;
				break;
			case 3:
				holderArm = World::instance_->topRightArm_;
				break;
			case 4:
				holderArm = World::instance_->botRightArm_;
				break;
			default:
				URHO3D_LOGDEBUG("too many ingredients picked?!");
				holderArm = nullptr;
				break;
			}

			holderArm->GrabSprite(cache->GetResource<Sprite2D>(pickedIngredient.imageFilePath_));
			holderArm->SetOnScreen(true);

			if (pickedIngs.Size() >= 4) {
				// hide ingredients ui, show blend button
				ingredientsPickerBar_->SetVisible(false);
				startMixingBtn_->SetVisible(true);
			}
		}
	}

	void UIMixer::HandleGoBackButton(StringHash, VariantMap& eventData)
	{
		Hide();
		GameUI::instance_->ShowScreen<UIMainMenu>();
	}

}