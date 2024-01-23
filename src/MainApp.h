#include <string>
#include <sstream>
#include <Urho3D/Urho3DAll.h>
#include "GameConsts.h"
#include "Debug.h"
#include "World.h"
#include "GameAudio.h"
#include "ui/GameUI.h"
#include "input/MouseHandler.h"

using namespace Urho3D;

using namespace Laugh;

/**
* Using the convenient Application API we don't have
* to worry about initializing the engine or writing a main.
* You can probably mess around with initializing the engine
* and running a main manually, but this is convenient and portable.
*/
class MainApp : public Application
{

public:
	/**
	* This happens before the engine has been initialized
	* so it's usually minimal code setting defaults for
	* whatever instance variables you have.
	* You can also do this in the Setup method.
	*/
	MainApp(Context* context);

	~MainApp() override = default;

	/**
	* This method is called before the engine has been initialized.
	* Thusly, we can setup the engine parameters before anything else
	* of engine importance happens (such as windows, search paths,
	* resolution and other things that might be user configurable).
	*/
	virtual void Setup();

	/**
	* This method is called after the engine has been initialized.
	* This is where you set up your actual content, such as scenes,
	* models, controls and what not. Basically, anything that needs
	* the engine initialized and ready goes in here.
	*/
	virtual void Start();

	/**
	* Good place to get rid of any system resources that requires the
	* engine still initialized. You could do the rest in the destructor,
	* but there's no need, this method will get called when the engine stops,
	* for whatever reason (short of a segfault).
	*/
	virtual void Stop();

private:
	DebugUtils* debugUtils_;
	MouseHandler* mouseHandler_;
	World* world_;
	GameAudio* gameAudio_;
	GameUI* gameUI_;
};


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