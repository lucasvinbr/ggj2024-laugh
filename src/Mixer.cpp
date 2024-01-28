#pragma once
#include "Mixer.h"
#include "World.h"
#include "ui/GameUI.h"
#include "GameConsts.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Scene/ObjectAnimation.h>
#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Scene/SceneEvents.h>
#include "ui/screens/UIGotItem.h"

using namespace Urho3D;

namespace Laugh {
    Mixer::Mixer(Context* c) : LogicComponent(c)
    {
        baseSprite_ = nullptr;
        glassSprite_ = nullptr;
        btnSprite_ = nullptr;
        lidSprite_ = nullptr;
        ingredientsParent_ = nullptr;
        visualsParent_ = nullptr;

        mixerState_ = IDLE_LID_ON;
        timeAnimating_ = 0.0f;

        SetUpdateEventMask(USE_UPDATE);
    }

    void Mixer::RegisterObject(Context* context)
    {
        context->RegisterFactory<Mixer>();
    }

    void Mixer::ToggleDisplay(bool show)
    {
        
    }

    void Mixer::PlaceLid()
    {
        if (mixerState_ != IDLE_LID_ON && mixerState_ != PUTTING_LID) {
            mixerState_ = PUTTING_LID;
            timeAnimating_ = 0.0f;
        }
    }

    void Mixer::RemoveLid()
    {
        if (mixerState_ != IDLE_LID_OFF && mixerState_ != REMOVING_LID) {
            mixerState_ = REMOVING_LID;
            timeAnimating_ = 0.0f;

            // glass does slight move anim

            // scale animation
            SharedPtr<ValueAnimation> scaleAnim(new ValueAnimation(context_));
            // Use spline interpolation method
            scaleAnim->SetInterpolationMethod(IM_SPLINE);
            // Set spline tension
            scaleAnim->SetSplineTension(1.7f);
            scaleAnim->SetKeyFrame(0.0f, Vector3::ONE);
            scaleAnim->SetKeyFrame(0.1f, Vector3(1.05f, 1.0f, 1.0f));
            scaleAnim->SetKeyFrame(0.2f, Vector3(1.0f, 1.1f, 1.0f));
            scaleAnim->SetKeyFrame(0.3f, Vector3::ONE);
            glassSprite_->GetNode()->SetAttributeAnimation("Scale", scaleAnim, WM_ONCE);
            
        }
    }

    void Mixer::StartMixing()
    {
        // place all ingredients inside mixer then start anim!
        auto& ingredients = World::instance_->recipesData_->ingredients_;
        auto& pickedIngs = World::instance_->pickedIngredients_;
        auto cache = GetSubsystem<ResourceCache>();

        for (auto ingEntry = pickedIngs.Begin(); ingEntry != pickedIngs.End();) {
            auto& ingData = ingredients[*ingEntry];

            PlaceIngredient(cache->GetResource<Sprite2D>(ingData.imageFilePath_));

            ++ingEntry;
        }

        // place lid, then start mix anim!
        // subscribe to anim event, so that we know when we're done with the lid
        using namespace MixerAnimDone;
        SubscribeToEvent(E_MIXER_ANIM_DONE, URHO3D_HANDLER(Mixer, OnDoneAnimating));

        PlaceLid();
    }

    void Mixer::PlaceIngredient(Sprite2D* ingSprite)
    {
        auto glassNode = glassSprite_->GetNode();
        auto cache = GetSubsystem<ResourceCache>();

        auto newIngNode = glassNode->CreateChild("placedIng");
        auto ingBubbleSprite = newIngNode->CreateComponent<StaticSprite2D>();
        ingBubbleSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/ggj2024-laugh/bolha_0.png"));
        ingBubbleSprite->SetLayer(SPRITELAYER_INSIDE_MIXER);
        
        auto innerIngNode = newIngNode->CreateChild("ingIcon");
        auto ingIconSprite = innerIngNode->CreateComponent<StaticSprite2D>();
        ingIconSprite->SetSprite(ingSprite);
        ingIconSprite->SetLayer(SPRITELAYER_INSIDE_MIXER);
        innerIngNode->SetScale(0.4f);

        ingredientSprites_.Push(ingBubbleSprite);

        newIngNode->SetPosition(Vector3(Random() - 0.5f, Random() - 0.5f));
    }

    void Mixer::Cleanup()
    {
        for (auto placedIngEntry = ingredientSprites_.Begin(); placedIngEntry != ingredientSprites_.End();) {
            (*placedIngEntry)->GetNode()->Remove();

            ++placedIngEntry;
        }

        ingredientSprites_.Clear();

        lidSprite_->GetNode()->SetPosition(startLidPos_);
        glassSprite_->GetNode()->SetPosition(startGlassPos_);
        baseSprite_->GetNode()->SetPosition(startBasePos_);
        btnSprite_->GetNode()->SetPosition(startBtnPos_);
    }

    void Mixer::Start()
    {
        auto cache = GetSubsystem<ResourceCache>();
        auto graphics = GetSubsystem<Graphics>();

        auto mixerSheet = cache->GetResource<SpriteSheet2D>("Urho2D/ggj2024-laugh/mixer/mixer.xml");

        visualsParent_ = node_->CreateChild("mixerVisuals");
        visualsParent_->SetScale(0.5f);

        auto baseNode = visualsParent_->CreateChild("base");
        baseSprite_ = baseNode->CreateComponent<StaticSprite2D>();
        baseSprite_->SetSprite(mixerSheet->GetSprite("mix_base"));
        baseSprite_->SetLayer(SPRITELAYER_MIXER);

        // position base a bit below screen center
        auto& baseRect = mixerSheet->GetSprite("mix_base")->GetRectangle();

        float baseHeight = baseRect.Height() * PIXEL_SIZE;
        baseNode->Translate(Vector3::DOWN * baseHeight / 2);

        auto btnNode = baseNode->Clone();
        btnSprite_ = btnNode->GetComponent<StaticSprite2D>();
        btnSprite_->SetSprite(mixerSheet->GetSprite("mix_btn"));

        auto glassNode = baseNode->Clone();
        glassSprite_ = glassNode->GetComponent<StaticSprite2D>();
        glassSprite_->SetSprite(mixerSheet->GetSprite("mix_glass"));

        // glass should be exactly above base
        auto& glassRect = mixerSheet->GetSprite("mix_glass")->GetRectangle();
        glassNode->Translate(Vector3::UP * (glassRect.Height() * PIXEL_SIZE * 0.65f));

        // set up ingredients point now: the middle of the glass should be good
        ingredientsParent_ = glassNode->CreateChild("ingredientsParent");

        auto lidNode = glassNode->Clone();
        lidSprite_ = lidNode->GetComponent<StaticSprite2D>();
        lidSprite_->SetSprite(mixerSheet->GetSprite("mix_lid"));

        // lid should be exactly above glass... and a little to the left
        lidNode->Translate(Vector3(-20 * PIXEL_SIZE, (glassRect.Height() * PIXEL_SIZE * 0.45f), 0.0f));

        // sort layers: in order, lid, glass, base, btn
        lidSprite_->SetOrderInLayer(1);
        glassSprite_->SetOrderInLayer(2);
        baseSprite_->SetOrderInLayer(3);
        btnSprite_->SetOrderInLayer(4);

        // store the part positions, so that we can move them around and restore this pos later
        startBasePos_ = baseNode->GetPosition();
        startLidPos_ = lidNode->GetPosition();
        startGlassPos_ = glassNode->GetPosition();
        startBtnPos_ = btnNode->GetPosition();

        lidRemovedPos_ = startLidPos_ + Vector3::UP * 1000.0f * PIXEL_SIZE;
    }

    void Mixer::Update(float timeStep)
    {
        using namespace MixerAnimDone;

        timeAnimating_ += timeStep;
        switch (mixerState_)
        {
        case Laugh::IDLE_LID_ON:
            break;
        case Laugh::IDLE_LID_OFF:
            break;
        case Laugh::REMOVING_LID:
            lidSprite_->GetNode()->SetPosition(startLidPos_.Lerp(lidRemovedPos_, timeAnimating_ / LID_ANIM_TIME));
            lidSprite_->SetAlpha(1.0f - timeAnimating_ / LID_ANIM_TIME);
            if (timeAnimating_ >= LID_ANIM_TIME) {
                URHO3D_LOGDEBUG("done REMOVING_LID");
                mixerState_ = IDLE_LID_OFF;
                VariantMap doneAnimEventMap = VariantMap();
                doneAnimEventMap[P_FINISHED_ANIM] = "RemoveLid";
                SendEvent(E_MIXER_ANIM_DONE, doneAnimEventMap);
            }
            break;
        case Laugh::PUTTING_LID:
            lidSprite_->GetNode()->SetPosition(lidRemovedPos_.Lerp(startLidPos_, timeAnimating_ / LID_ANIM_TIME));
            lidSprite_->SetAlpha(timeAnimating_ / LID_ANIM_TIME);
            if (timeAnimating_ >= LID_ANIM_TIME) {
                URHO3D_LOGDEBUG("done PUTTING_LID");
                // scale animation
                SharedPtr<ValueAnimation> scaleAnim(new ValueAnimation(context_));
                // Use spline interpolation method
                scaleAnim->SetInterpolationMethod(IM_SPLINE);
                // Set spline tension
                scaleAnim->SetSplineTension(1.7f);
                scaleAnim->SetKeyFrame(0.0f, Vector3::ONE);
                scaleAnim->SetKeyFrame(0.1f, Vector3(1.05f, 1.0f, 1.0f));
                scaleAnim->SetKeyFrame(0.2f, Vector3(1.0f, 1.1f, 1.0f));
                scaleAnim->SetKeyFrame(0.3f, Vector3::ONE);
                glassSprite_->GetNode()->SetAttributeAnimation("Scale", scaleAnim, WM_ONCE);

                mixerState_ = IDLE_LID_ON;
                VariantMap doneAnimEventMap = VariantMap();
                doneAnimEventMap[P_FINISHED_ANIM] = "PutLid";
                SendEvent(E_MIXER_ANIM_DONE, doneAnimEventMap);
            }
            break;
        case Laugh::SHAKING:
            float curIntensity;

            if (MIX_ANIM_TIME - timeAnimating_ <= stoppingTime) {
                curIntensity = Lerp(0.0f, finalShakeIntensity, (MIX_ANIM_TIME - timeAnimating_ / stoppingTime));
            }
            else {
                curIntensity = Lerp(startShakeIntensity, finalShakeIntensity, timeAnimating_ / MIX_ANIM_TIME);
            }

            lidSprite_->GetNode()->SetPosition(startLidPos_ + Vector3(Random() - 0.5f, Random() - 0.5f) * curIntensity);
            glassSprite_->GetNode()->SetPosition(startGlassPos_ + Vector3(Random() - 0.5f, Random() - 0.5f) * curIntensity);
            baseSprite_->GetNode()->SetPosition(startBasePos_ + Vector3(Random() - 0.5f, Random() - 0.5f) * curIntensity);
            btnSprite_->GetNode()->SetPosition(startBtnPos_ + Vector3(Random() - 0.5f, Random() - 0.5f) * curIntensity);

            if (timeAnimating_ >= MIX_ANIM_TIME) {
                URHO3D_LOGDEBUG("done SHAKING");
                mixerState_ = IDLE_LID_ON;
                VariantMap doneAnimEventMap = VariantMap();
                doneAnimEventMap[P_FINISHED_ANIM] = "Mix";
                SendEvent(E_MIXER_ANIM_DONE, doneAnimEventMap);
            }

            break;
        default:
            break;
        }
    }

    void Mixer::OnDoneAnimating(StringHash, VariantMap& eventData)
    {
        using namespace MixerAnimDone;

        auto& animName = eventData[P_FINISHED_ANIM].GetString();

        // it's a one-shot event
        UnsubscribeFromEvent(this, E_MIXER_ANIM_DONE);

        if (animName == "PutLid") {
            // done putting lid, let's start mixing
            mixerState_ = SHAKING;
            timeAnimating_ = 0.0f;

            // subscribe to anim event, so that we know when we're done mixing
            SubscribeToEvent(E_MIXER_ANIM_DONE, URHO3D_HANDLER(Mixer, OnDoneAnimating));
        }
        else if (animName == "Mix") {
            // done mixing, open lid
            RemoveLid();

            // subscribe to anim event, so that we know when we're done removing the lid
            SubscribeToEvent(E_MIXER_ANIM_DONE, URHO3D_HANDLER(Mixer, OnDoneAnimating));
        }
        else if (animName == "RemoveLid") {
            // show got item UI with result
            GameUI::instance_->ShowScreen<UIGotItem>();
        }
    }

}