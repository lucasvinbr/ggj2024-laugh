#pragma once
#include "Mixer.h"

using namespace Urho3D;

namespace Laugh {
    Mixer::Mixer(Context* c) : LogicComponent(c)
    {
        animatedSprite_ = nullptr;
        attachmentPoint_ = nullptr;

        SetUpdateEventMask(USE_UPDATE);
    }

    void Mixer::RegisterObject(Context* context)
    {
        context->RegisterFactory<Mixer>();
    }

    void Mixer::ToggleDisplay(bool show)
    {
        animatedSprite_->SetEnabled(show);
    }

    void Mixer::PlaceIngredient(Sprite2D* ingSprite)
    {
    }

    void Mixer::Start()
    {
    }

    void Mixer::Update(float timeStep)
    {
    }

}