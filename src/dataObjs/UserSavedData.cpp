#pragma once
#include "UserSavedData.h"
#include <Urho3D/IO/Log.h>


using namespace Urho3D;

namespace Laugh {
    Laugh::UserSavedData::UserSavedData(Context* c) : Resource(c)
	{
        audioVolume_ = 1.0f;
	}

    void UserSavedData::RegisterObject(Context* context)
    {
        context->RegisterFactory<UserSavedData>();
    }

    bool UserSavedData::BeginLoad(Deserializer& source)
    {
        JSONFile file(context_);
        if (!file.Load(source))
        {
            URHO3D_LOGERROR("Load UserSavedData file failed");
            return false;
        }

        JSONValue rootElem = file.GetRoot();

        bool success = Load(rootElem);
        if (success)
            SetMemoryUse(source.GetSize());
        return success;
    }

    bool UserSavedData::EndLoad()
    {
        return true;
    }

    bool UserSavedData::Save(Serializer& dest) const
    {
        SharedPtr<JSONFile> json(new JSONFile(context_));
        JSONValue jsonRoot = json->GetRoot();

        Save(jsonRoot);
        return json->Save(dest);
    }

    bool UserSavedData::Save(JSONValue& dest) const
    {
        if (dest.IsNull())
        {
            URHO3D_LOGERROR("Can not save UserSavedData to null jsonValue");
            return false;
        }

        dest.Set("audiovolume", audioVolume_);
        dest.Set("knownrecipes", JsonUtils::SetupJsonArrayFromStringVector(knownRecipes_));

        return true;
    }

    bool UserSavedData::Load(const JSONValue& source)
    {
        // Reset to defaults first so that missing parameters in case of a live reload behave as expected
        audioVolume_ = 1.0f;
        knownRecipes_.Clear();

        if (source.IsNull())
        {
            URHO3D_LOGERROR("Can not load UserSavedData from null json element");
            return false;
        }

        audioVolume_ = JsonUtils::GetFloatFromSourceJson(source, "audiovolume");
        JsonUtils::FillStringVectorFromJsonArray(knownRecipes_, source, "knownrecipes");

        return true;
    }
}