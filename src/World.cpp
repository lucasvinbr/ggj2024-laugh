#pragma once
#include "Debug.h"
#include "GameConsts.h"
#include "World.h"
#include <Urho3D/Urho2D/Drawable2D.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>

using namespace Urho3D;

namespace Laugh {
    World* World::instance_ = nullptr;
    Camera* World::camera_ = nullptr;

    Laugh::World::World(Context* c) : Object(c),
        scene_(nullptr),
        dynamicContentParent_(nullptr),
        physWorld_(nullptr),
        effects_(c),
        curGameState_(GAMESTATE_ENDED)
    {
        auto fileSystem = GetSubsystem<FileSystem>();
        prefsFileDir_ = fileSystem->GetAppPreferencesDir("ggj2024-laugh", "blendastic");

        URHO3D_LOGDEBUG(prefsFileDir_);

        prefsFilePath_ = prefsFileDir_ + "userprefs.json";

        auto cache = GetSubsystem<ResourceCache>();

        auto prefsFile = cache->GetResource<UserSavedData>(prefsFilePath_);

        if (prefsFile) {
            userData_ = prefsFile;
        }
        else {
            userData_ = SharedPtr<UserSavedData>(new UserSavedData(context_));
            userData_->LoadFile(prefsFilePath_);
        }

        mixer_ = nullptr;
        instance_ = this;
    }

    World::~World()
    {
        delete camera_->GetNode();
        delete scene_;
    }

    void World::CreateScene()
    {
        scene_ = new Scene(context_);

        // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
        scene_->CreateComponent<Octree>();

        physWorld_ = scene_->CreateComponent<PhysicsWorld>();
        physWorld_->SetNumIterations(1);

        scene_->CreateComponent<DebugRenderer>();

        Node* cameraNode = new Node(context_);
        cameraNode->SetPosition(Vector3());

        camera_ = cameraNode->CreateComponent<Camera>();
        //camera_->SetFarClip(MAX_VIEW_DISTANCE);

        camera_->SetOrthographic(true);
        auto* graphics = GetSubsystem<Graphics>();
        camera_->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);
        camera_->SetZoom(1.5f * Min((float)graphics->GetWidth() / 1280.0f, (float)graphics->GetHeight() / 800.0f)); // Set zoom according to user's resolution to ensure full visibility (initial zoom (2.0) is set for full visibility at 1280x800 resolution)


        // apparently, this affects model lods and animation update frequency
        camera_->SetLodBias(1.0f);

        viewport_ = new Viewport(context_, scene_, camera_);

        auto renderer = GetSubsystem<Renderer>();
        renderer->SetViewport(0, viewport_);
        // background color for the scene
        renderer->GetDefaultZone()->SetFogColor(Color(0.2f, 0.2f, 0.2f));


        // set up view effects!
        auto cache = GetSubsystem<ResourceCache>();
        auto renderPath = viewport_->GetRenderPath();
        renderPath->Append(cache->GetResource<XMLFile>("PostProcess/Bloom.xml"));
        
        // change bloom settings
        renderPath->SetShaderParameter("BloomMix", Variant(Vector2(0.9f, 0.3f)));
        renderPath->SetEnabled("Bloom", true);

        recipesData_ = cache->GetResource<RecipesData>("Objects/ggj2024-laugh/recipes.json");

        URHO3D_LOGDEBUG("num ingredients: " + String(recipesData_->ingredients_.Size()));
        URHO3D_LOGDEBUG("num recipes: " + String(recipesData_->recipes_.Size()));

        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(World, HandlePostRenderUpdate));

        //in this game, the main scene begins loaded
        CreateDynamicContent();
    }

    void World::CreateDynamicContent()
    {
        curGameState_ = GAMESTATE_STARTING;

        /*worldDecals.Cleanup()

        worldEffects.Cleanup()*/

        dynamicContentParent_ = scene_->CreateChild("DynamicContent");

        effects_.Initialize(scene_);

        auto zoneNode = dynamicContentParent_->CreateChild("Zone");
        auto zone = zoneNode->CreateComponent<Zone>();
        zone->SetBoundingBox(BoundingBox(-WALKABLE_WORLD_BOUNDS * 2, WALKABLE_WORLD_BOUNDS * 2));
        zone->SetAmbientColor(Color(0.5f, 0.5f, 0.5f));
        zone->SetFogColor(Color(0.7f, 0.7f, 0.7f));
        zone->SetFogStart(100.0f);
        zone->SetFogEnd(MAX_VIEW_DISTANCE - 20.0f);

        auto lightNode = dynamicContentParent_->CreateChild("DirectionalLight");
        lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
        auto light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetCastShadows(true);
        light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
        light->SetShadowCascade(CascadeParameters(MAX_VIEW_DISTANCE / 20, MAX_VIEW_DISTANCE / 4, MAX_VIEW_DISTANCE, 0.0f, 0.8f));

        auto cache = GetSubsystem<ResourceCache>();

        auto bgNode = dynamicContentParent_->CreateChild("bg");
        auto bgSprite = bgNode->CreateComponent<StaticSprite2D>();
        bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/ggj2024-laugh/bg.png"));
        bgSprite->SetLayer(SPRITELAYER_BG);

        auto tentacleTLNode = dynamicContentParent_->CreateChild("tentacle_topLeft");
        topLeftArm_ = tentacleTLNode->CreateComponent<TentacleArm>();

        auto tentacleTRNode = tentacleTLNode->Clone();
        tentacleTRNode->SetName("tentacle_topRight");
        topRightArm_ = tentacleTRNode->GetComponent<TentacleArm>();

        auto tentacleBLNode = tentacleTLNode->Clone();
        tentacleBLNode->SetName("tentacle_botLeft");
        botLeftArm_ = tentacleBLNode->GetComponent<TentacleArm>();

        auto tentacleBRNode = tentacleTLNode->Clone();
        tentacleBRNode->SetName("tentacle_botRight");
        botRightArm_ = tentacleBRNode->GetComponent<TentacleArm>();

        topLeftArm_->Setup(true, true);
        topRightArm_->Setup(true, false);
        botLeftArm_->Setup(false, true);
        botRightArm_->Setup(false, false);

        auto mixerNode = dynamicContentParent_->CreateChild("mixer");
        mixer_ = mixerNode->CreateComponent<Mixer>();
    }

    void World::Cleanup()
    {
    }

    void World::SaveUserData()
    {
        userData_->SaveFile(prefsFilePath_);
    }

    void World::HandlePostRenderUpdate(StringHash, VariantMap& eventData)
    {
        if (Laugh::DebugUtils::drawDebug_) {
            physWorld_->DrawDebugGeometry(true);
        }
    }
}