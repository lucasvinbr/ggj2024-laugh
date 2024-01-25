#pragma once
#include "TentacleArm.h"

using namespace Urho3D;

namespace Laugh {
    TentacleArm::TentacleArm(Context* c) : LogicComponent(c)
    {
        armSprite_ = nullptr;
        attachmentPoint_ = nullptr;

        SetUpdateEventMask(USE_UPDATE);
    }

    void TentacleArm::RegisterObject(Context* context)
    {
        context->RegisterFactory<TentacleArm>();
    }

    void TentacleArm::Setup(bool isTop, bool isLeft)
    {
        
    }

    void TentacleArm::GrabSprite(Sprite2D* spriteToGrab)
    {
    }

    void TentacleArm::SpecialGrabLidAnim()
    {
    }

    void TentacleArm::Update(float timeStep)
    {
    }

}