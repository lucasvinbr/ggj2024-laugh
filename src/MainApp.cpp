#pragma once
#include <string>
#include <sstream>
#include "MainApp.h"
#include <Urho3D/Urho3DAll.h>
#include "World.h"
#include "Mixer.h"
#include "dataObjs/RecipeData.h"

// debugnew must always be included last!!!!!!!!
#include <Urho3D/DebugNew.h>

using namespace Urho3D;

using namespace Laugh;

MainApp::MainApp(Context* context) : Application(context)
{
	Mixer::RegisterObject(context);
	RecipesData::RegisterObject(context);
	TentacleArm::RegisterObject(context);
}
/**
* This method is called before the engine has been initialized.
* Thusly, we can setup the engine parameters before anything else
* of engine importance happens (such as windows, search paths,
* resolution and other things that might be user configurable).
*/
void MainApp::Setup()
{
	// These parameters should be self-explanatory.
	// See http://urho3d.github.io/documentation/1.7/_main_loop.html
	// for a more complete list.
	engineParameters_[EP_FULL_SCREEN] = false;
	engineParameters_[EP_LOG_NAME] = "blendastic.log";
	engineParameters_[EP_SOUND] = true;
	// Configuration not depending whether we compile for debug or release.
	engineParameters_[EP_WINDOW_WIDTH] = 1280;
	engineParameters_[EP_WINDOW_HEIGHT] = 720;

	// All 'EP_' constants are defined in ${URHO3D_INSTALL}/include/Urho3D/Engine/EngineDefs.h file
	URHO3D_LOGINFO("--SETUP DONE--");
}

/**
* This method is called after the engine has been initialized.
* This is where you set up your actual content, such as scenes,
* models, controls and what not. Basically, anything that needs
* the engine initialized and ready goes in here.
*/
void MainApp::Start()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Graphics* graphics = GetSubsystem<Graphics>();

	auto icon = cache->GetResource<Image>("Urho2D/ggj2024-laugh/ings_recipes/cat.png");
	graphics->SetWindowIcon(icon);
	graphics->SetWindowTitle("Blendastic");

	debugUtils_ = new DebugUtils(engine_, context_);

	mouseHandler_ = new MouseHandler(context_);
	mouseHandler_->SetupMouseEvents();

	world_ = new World(context_);
	world_->CreateScene();

	gameAudio_ = new GameAudio(context_);
	gameAudio_->SetupSound();

	gameUI_ = new GameUI(context_);
	gameUI_->SetupUI();

	mouseHandler_->SetMouseMode(MM_FREE);
}

/**
* Good place to get rid of any system resources that requires the
* engine still initialized. You could do the rest in the destructor,
* but there's no need, this method will get called when the engine stops,
* for whatever reason (short of a segfault).
*/
void MainApp::Stop()
{
	URHO3D_LOGINFO("--STOPPING--");

	delete gameUI_;
	delete gameAudio_;
	delete world_;
	delete mouseHandler_;
	delete debugUtils_;
}

/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* > Urho3D::SharedPtr<className> application(new className(context));
* > return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* > Urho3D::ParseArguments(argc, argv);
* > return function;
* > }
*/
URHO3D_DEFINE_APPLICATION_MAIN(MainApp)