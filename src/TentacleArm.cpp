#include "TentacleArm.h"
#include "GameConsts.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/IO/Log.h>

using namespace Urho3D;

namespace Laugh {
    TentacleArm::TentacleArm(Context* c) : LogicComponent(c)
    {
        armSprite_ = nullptr;
        attachmentPoint_ = nullptr;
        breatheAnimTime_ = 0.0f;
        doBreathingAnim_ = true;

        SetUpdateEventMask(USE_UPDATE);
    }

    void TentacleArm::RegisterObject(Context* context)
    {
        context->RegisterFactory<TentacleArm>();
    }

    void TentacleArm::Setup(bool isTop, bool isLeft)
    {
        auto cache = GetSubsystem<ResourceCache>();

        auto scalerNode = node_->CreateChild("scaler");

        armSprite_ = scalerNode->CreateComponent<AnimatedSprite2D>();
        armSprite_->SetLayer(SPRITELAYER_MIXER);
        armSprite_->SetAnimationSet(cache->GetResource<AnimationSet2D>("Urho2D/ggj2024-laugh/tentacle/tentacle.scml"));
        armSprite_->SetAnimation("idle");

        auto baseTentacleImg = cache->GetResource<Texture2D>("Urho2D/ggj2024-laugh/tentacle/polvo_tentaculo_0.png");

        float tentaHeight = baseTentacleImg->GetHeight() * PIXEL_SIZE;
        float tentaWidth = baseTentacleImg->GetWidth() * PIXEL_SIZE;

        scalerNode->SetScale(0.5f);
        auto graphics = GetSubsystem<Graphics>();
        float scrnHeight = graphics->GetHeight() * PIXEL_SIZE;
        float scrnWidth = graphics->GetWidth() * PIXEL_SIZE;

        if (isTop) {
            // tentacles start at the center, rotated for topLeft
            node_->Translate(Vector3::UP * (scrnHeight - tentaHeight * 1.9f));
        }
        else {
            armSprite_->SetFlipY(true);
            node_->Translate(Vector3::DOWN * (scrnHeight - tentaHeight * 1.9f));
        }

        if (isLeft) {
            node_->Translate(Vector3::LEFT * (scrnWidth - tentaWidth * 1.5f));
            
            onScreenPos_ = node_->GetPosition();
            offScreenPos_ = node_->GetPosition() + Vector3::LEFT * tentaWidth * 2.0f;
        }
        else {
            armSprite_->SetFlipX(true);
            node_->Translate(Vector3::RIGHT * (scrnWidth - tentaWidth * 1.5f));

            onScreenPos_ = node_->GetPosition();
            offScreenPos_ = node_->GetPosition() + Vector3::RIGHT * tentaWidth * 2.0f;
        }

    }

    void TentacleArm::GrabSprite(Sprite2D* spriteToGrab)
    {
    }

    void TentacleArm::SpecialGrabLidAnim()
    {
    }

    void TentacleArm::Update(float timeStep)
    {
        breatheAnimTime_ += timeStep;

        if (breatheAnimTime_ >= M_PI * 2.0f) {
            breatheAnimTime_ = 0.0f;
        }

        if(doBreathingAnim_){
            node_->SetScale(Vector3(1.0f + sin(breatheAnimTime_ * 2.0f) * 0.15f, 1.0f + sin(breatheAnimTime_ * 1.0f) * 0.15f, 1.0f));
        }
        else {
            node_->SetScale(Vector3::ONE);
        }
        
        if (isOnScreen_) {
            if (node_->GetPosition() != onScreenPos_) {
                appearAnimTime_ += timeStep;
                node_->SetPosition(offScreenPos_.Lerp(onScreenPos_, appearAnimTime_ / ANIM_ENTER_DURATION));
            }
        }
        else {
            if (node_->GetPosition() != offScreenPos_) {
                appearAnimTime_ += timeStep;
                node_->SetPosition(onScreenPos_.Lerp(offScreenPos_, appearAnimTime_ / ANIM_ENTER_DURATION));
            }
        }
    }

    void TentacleArm::SetOnScreen(bool onScreen)
    {
        if (onScreen != isOnScreen_) {
            isOnScreen_ = onScreen;

            appearAnimTime_ = 0.0f;
        }
    }

}