#include "Debug.h"
#include "GameConsts.h"
#include "World.h"

using namespace Urho3D;

namespace Laugh {
    bool DebugUtils::drawDebug_ = false;

    DebugUtils::DebugUtils(SharedPtr<Engine> e, Context* c) :
        Object(c),
        engine_(e)
    {
        drawDebug_ = false;

        CreateConsoleAndDebugHud(c);

        // Subscribe key down event
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(DebugUtils, HandleKeyDown));
        // Subscribe key up event
        SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(DebugUtils, HandleKeyUp));
    }

    void DebugUtils::CreateConsoleAndDebugHud(Context* context)
	{
        // Get default style
        ResourceCache* cache = context->GetSubsystem<ResourceCache>();
        XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

        // Create console
        Console* console = engine_->CreateConsole();
        console->SetDefaultStyle(xmlFile);
        console->GetBackground()->SetOpacity(0.8f);

        // Create debug HUD.
        DebugHud* debugHud = engine_->CreateDebugHud();
        debugHud->SetDefaultStyle(xmlFile);
	}

    void DebugUtils::HandleKeyUp(StringHash /*eventType*/, VariantMap& eventData)
    {
        using namespace KeyUp;

        int key = eventData[P_KEY].GetInt();

        // Close console (if open) or exit when ESC is pressed
        if (key == KEY_ESCAPE)
        {
            Console* console = GetSubsystem<Console>();
            if (console->IsVisible())
                console->SetVisible(false);
            else
            {
                if (GetPlatform() != "Web")
                {
                    engine_->Exit();
                }   
            }
        }
    }

    void DebugUtils::HandleKeyDown(StringHash /*eventType*/, VariantMap& eventData)
    {
        using namespace KeyDown;

        int key = eventData[P_KEY].GetInt();

        // Toggle console with F1
        if (key == KEY_F1)
            GetSubsystem<Console>()->Toggle();

        // Toggle debug HUD with F2
        else if (key == KEY_F2)
            GetSubsystem<DebugHud>()->ToggleAll();

        // Toggle debug geometry with F3
        else if (key == KEY_F3)
            drawDebug_ = !drawDebug_;

        else if (key == KEY_F5) {
            File saveFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Scenes/" + PROJECT_FOLDER + "/debugSave.xml", FILE_WRITE);
            World::instance_->scene_->SaveXML(saveFile);

            auto uiRoot = GetSubsystem<UI>()->GetRoot();
            File UiSaveFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/UI/" + PROJECT_FOLDER + "/debugSaveUI.xml", FILE_WRITE);
            uiRoot->SaveXML(UiSaveFile);
        }

    }
}