#pragma once
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Deserializer.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/Resource.h>
#include "../utils/JsonUtils.h"

using namespace Urho3D;

namespace Laugh {

	/// <summary>
	/// data representing all info coming from the user that should persist between sessions
	/// </summary>
	struct UserSavedData : public Resource {

		URHO3D_OBJECT(UserSavedData, Resource);

	public:
		UserSavedData(Context* c);
		~UserSavedData() = default;

		static void RegisterObject(Context* context);

		/// Load resource from stream. May be called from a worker thread. Return true if successful.
		bool BeginLoad(Deserializer& source) override;
		/// Finish resource loading. Always called from the main thread. Return true if successful.
		bool EndLoad() override;
		/// Save resource. Return true if successful.
		bool Save(Serializer& dest) const override;

		using Resource::Load;

		/// Save resource to Json. Return true if successful.
		bool Save(JSONValue& dest) const;
		/// Load resource from Json synchronously. Return true if successful.
		bool Load(const JSONValue& source);

		float audioVolume_;
		Vector<String> knownRecipes_;
};
}