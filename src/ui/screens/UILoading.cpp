#pragma once
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include "UILoading.h"
#include "../GameUI.h"
#include "../../input/MouseHandler.h"
#include "UIMixer.h"


using namespace Urho3D;

namespace Laugh {

	Laugh::UILoading::UILoading(Context* c) : UIGameScreen(c)
	{
		auto cache = c->GetSubsystem<ResourceCache>();
		auto ui = c->GetSubsystem<UI>();
		auto uiFile = cache->GetResource<XMLFile>("UI/ggj2024-laugh/screen_loading.xml");

		instanceRoot_ = ui->LoadLayout(uiFile);
		ui->GetRoot()->AddChild(instanceRoot_);
		instanceRoot_->SetVisible(false);
		isShown_ = false;
	}

	UILoading::~UILoading()
	{
		instanceRoot_->Remove();
	}

	void UILoading::Setup()
	{
	}

	void UILoading::Show(String dataPassed)
	{
		isShown_ = true;
		instanceRoot_->SetVisible(true);

		// TODO set the data being loaded elsewhere
		// maybe make this screen handle loading situations other than "menu->game"

		//World::instance_->CreateDynamicContent();

		World::instance_->curGameState_ = GAMESTATE_PLAYING;

		URHO3D_LOGDEBUG("loading done!");

		Hide();

		GameUI::instance_->ShowScreen<UIMixer>();
	}

	void UILoading::Hide()
	{
		isShown_ = false;
		instanceRoot_->SetVisible(false);
	}

}