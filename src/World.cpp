#pragma once
#include "Debug.h"
#include "GameConsts.h"
#include "World.h"

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
        cameraNode->SetPosition(Vector3::ONE * 5.0f);

        camera_ = cameraNode->CreateComponent<Camera>();
        camera_->SetFarClip(MAX_VIEW_DISTANCE);

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


    }

    void World::Cleanup()
    {
    }

    void World::HandlePostRenderUpdate(StringHash, VariantMap& eventData)
    {
        if (Laugh::DebugUtils::drawDebug_) {
            physWorld_->DrawDebugGeometry(true);
        }
    }
}