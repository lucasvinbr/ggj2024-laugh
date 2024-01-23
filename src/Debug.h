#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/UI/UI.h>


using namespace Urho3D;

namespace Laugh {
	class DebugUtils : public Object {
		URHO3D_OBJECT(DebugUtils, Object);
	public:
		DebugUtils(SharedPtr<Engine> e, Context* c);
		~DebugUtils() override = default;

		static bool drawDebug_;
	private:
		SharedPtr<Engine> engine_;
		void CreateConsoleAndDebugHud(Context* context);
		void HandleKeyUp(StringHash, VariantMap& eventData);
		void HandleKeyDown(StringHash, VariantMap& eventData);
};
}