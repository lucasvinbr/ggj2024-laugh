#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include "UISettings.h"
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

using namespace Urho3D;

namespace Laugh {

	Laugh::UISettings::UISettings(Context* c) : UIGameScreen(c)
	{
		glowSprite_ = nullptr;
		obtainedItemNameText_ = nullptr;
		obtainedItemSprite_ = nullptr;
		closeBtn_ = nullptr;

		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_endgame.xml");

		instanceRoot_ = ui->LoadLayout(uiFile);
		ui->GetRoot()->AddChild(instanceRoot_);
		instanceRoot_->SetVisible(false);
		isShown_ = false;
	}

	UISettings::~UISettings()
	{
		instanceRoot_->Remove();
	}

	void UISettings::Setup()
	{
		auto ui = context_->GetSubsystem<UI>();
		auto cache = context_->GetSubsystem<ResourceCache>();

		closeBtn_ = instanceRoot_->GetChild("doneBtn", true);
		closeBtn_->SetStyleAuto();
		SubscribeToEvent(closeBtn_, E_RELEASED, URHO3D_HANDLER(UISettings, HandleDoneButton));

		glowSprite_ = static_cast<Sprite*>(instanceRoot_->GetChild("shine", true));

		SharedPtr<ValueAnimation> rotAnim(new ValueAnimation(context_));
		rotAnim->SetInterpolationMethod(IM_LINEAR);
		rotAnim->SetKeyFrame(0.0f, 0.0f);
		rotAnim->SetKeyFrame(1.0f, 360.0f);
		glowSprite_->SetAttributeAnimation("Rotation", rotAnim, WM_LOOP);

		obtainedItemSprite_ = static_cast<Sprite*>(instanceRoot_->GetChild("itemSprite", true));
		obtainedItemSprite_->SetScale(Vector2::ZERO);

		obtainedItemNameText_ = static_cast<Text*>(instanceRoot_->GetChild("winnerText", true));
		obtainedItemNameText_->SetColor(Color::TRANSPARENT_BLACK);
	}

	void UISettings::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);
		auto cache = context_->GetSubsystem<ResourceCache>();

		GameAudio::instance_->StopMusic();
		GameAudio::instance_->PlayGotItemSound();

		// find the recipe with most matching ingredients. that's the one we will show as result
		auto& recipes = World::instance_->recipesData_->recipes_;
		auto& ingredients = World::instance_->recipesData_->ingredients_;
		auto& pickedIngs = World::instance_->pickedIngredients_;
		Recipe* bestRecipe;
		int recipeScore = 0, topScore = 0;
		for (auto recpEntry = recipes.Begin(); recpEntry != recipes.End();) {
			recipeScore = 0;
			Vector<String> recipeIngs = recpEntry->second_.ingredients_;

			for (int i = 0; i < 4; i++) {
				int foundIngIndex = recipeIngs.IndexOf(pickedIngs[i]);
				if (foundIngIndex != recipeIngs.Size()) {
					recipeScore++;
					recipeIngs.Erase(foundIngIndex);
				}
			}

			if (recipeScore > topScore) {
				bestRecipe = &(recpEntry->second_);
				topScore = recipeScore;
			}

			++recpEntry;
		}

		obtainedItemNameText_->SetText(bestRecipe->name_);
		SharedPtr<ValueAnimation> alphaAnim(new ValueAnimation(context_));
		alphaAnim->SetInterpolationMethod(IM_LINEAR);
		alphaAnim->SetKeyFrame(0.0f, Color::TRANSPARENT_BLACK);
		alphaAnim->SetKeyFrame(1.0f, Color());
		obtainedItemNameText_->SetAttributeAnimation("Color", alphaAnim, WM_ONCE);

		glowSprite_->SetAttributeAnimation("Color", alphaAnim, WM_ONCE);

		SharedPtr<ValueAnimation> scaleAnim(new ValueAnimation(context_));
		scaleAnim->SetInterpolationMethod(IM_LINEAR);
		scaleAnim->SetKeyFrame(0.0f, Vector2::ZERO);
		scaleAnim->SetKeyFrame(1.0f, Vector2::ONE);
		obtainedItemSprite_->SetAttributeAnimation("Scale", scaleAnim, WM_ONCE);

		obtainedItemSprite_->SetTexture(cache->GetResource<Texture2D>(bestRecipe->imageFilePath_));

		World::instance_->topLeftArm_->SetOnScreen(true);
		World::instance_->topRightArm_->SetOnScreen(true);
		World::instance_->botLeftArm_->SetOnScreen(true);
		World::instance_->botRightArm_->SetOnScreen(true);
	}

	void UISettings::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
	}

	void UISettings::HandleDoneButton(StringHash, VariantMap& eventData)
	{
		GameAudio::instance_->PlayClickSound();

		World::instance_->topLeftArm_->SetOnScreen(false);
		World::instance_->topRightArm_->SetOnScreen(false);
		World::instance_->botLeftArm_->SetOnScreen(false);
		World::instance_->botRightArm_->SetOnScreen(false);

		World::instance_->pickedIngredients_.Clear();

		Hide();
		GameUI::instance_->ShowScreen<UIMixer>();
	}


}