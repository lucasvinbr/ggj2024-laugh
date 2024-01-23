#pragma once
#include "RecipeData.h"
#include <Urho3D/IO/Log.h>


using namespace Urho3D;

namespace Laugh {
    Laugh::RecipesData::RecipesData(Context* c) : Resource(c)
	{
	}

    void RecipesData::RegisterObject(Context* context)
    {
        context->RegisterFactory<RecipesData>();
    }

    bool RecipesData::BeginLoad(Deserializer& source)
    {
        JSONFile file(context_);
        if (!file.Load(source))
        {
            URHO3D_LOGERROR("Load RecipesData file failed");
            return false;
        }

        JSONValue rootElem = file.GetRoot();

        bool success = Load(rootElem);
        if (success)
            SetMemoryUse(source.GetSize());
        return success;
    }

    bool RecipesData::EndLoad()
    {
        return true;
    }

    bool RecipesData::Save(Serializer& dest) const
    {
        SharedPtr<JSONFile> json(new JSONFile(context_));
        JSONValue jsonRoot = json->GetRoot();

        Save(jsonRoot);
        return json->Save(dest);
    }

    bool RecipesData::Save(JSONValue& dest) const
    {
        if (dest.IsNull())
        {
            URHO3D_LOGERROR("Can not save RecipesData to null jsonValue");
            return false;
        }


        JSONValue ingsJson = JSONValue();
        auto& ingrs = ingredients_;
        for (auto entry = ingrs.Begin(); entry != ingrs.End();) {
            JSONValue parsedData = JSONValue();
            auto& entryData = entry->second_;
            parsedData.Set("name", entryData.name_);
            parsedData.Set("imagepath", entryData.ImageFilePath_);
            ingsJson.Set(entry->first_, parsedData);
            ++entry;
        }
        dest.Set("ingredients", ingsJson);

        JSONValue recipesJson = JSONValue();
        auto& recipes = recipes_;
        for (auto entry = recipes.Begin(); entry != recipes.End();) {
            JSONValue parsedData = JSONValue();
            auto& entryData = entry->second_;
            parsedData.Set("name", entryData.name_);
            parsedData.Set("imagepath", entryData.ImageFilePath_);
            parsedData.Set("ingredients", JsonUtils::SetupJsonArrayFromStringVector(entryData.ingredients_));
            recipesJson.Set(entry->first_, parsedData);
            ++entry;
        }
        dest.Set("recipes", recipesJson);

        return true;
    }

    bool RecipesData::Load(const JSONValue& source)
    {
        // Reset to defaults first so that missing parameters in case of a live reload behave as expected
        ingredients_.Clear();
        recipes_.Clear();

        if (source.IsNull())
        {
            URHO3D_LOGERROR("Can not load RecipesData from null json element");
            return false;
        }


        if (source.Contains("ingredients")) {
            JSONValue ingredientsJson = source.Get("ingredients");
            for (auto ingr = ingredientsJson.Begin(); ingr != ingredientsJson.End();) {
                JSONValue rawObj = ingr->second_;
                ingredients_.Populate(ingr->first_, {
                    JsonUtils::GetStringFromSourceJson(rawObj, "name"),
                    JsonUtils::GetStringFromSourceJson(rawObj, "imagepath")
                    });
                ++ingr;
            }
        }

        if (source.Contains("recipes")) {
            JSONValue recipesJson = source.Get("recipes");
            for (auto recp = recipesJson.Begin(); recp != recipesJson.End();) {
                JSONValue rawObj = recp->second_;
                recipes_.Populate(recp->first_, {
                    JsonUtils::GetStringFromSourceJson(rawObj, "name"),
                    JsonUtils::GetStringFromSourceJson(rawObj, "imagepath"),
                    JsonUtils::CreateStringVectorFromJsonArray(rawObj, "ingredients")
                    });
                ++recp;
            }
        }

        return true;
    }
}