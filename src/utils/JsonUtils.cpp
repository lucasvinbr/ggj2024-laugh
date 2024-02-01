#include "JsonUtils.h"
#pragma once

namespace Laugh {
	bool JsonUtils::FillStringVectorFromJsonArray(Vector<String>& stringVec, const JSONValue& source, const String& key)
	{
		if (source.Contains(key))
		{
			JSONArray typesArr = source.Get(key).GetArray();
			stringVec.Reserve(typesArr.Size());
			for (unsigned int i = 0; i < typesArr.Size(); i++) {
				stringVec.Push(typesArr[i].GetString());
			}

			return true;
		}

		return false;
	}

	Vector<String> JsonUtils::CreateStringVectorFromJsonArray(const JSONValue& source, const String& key)
	{
		auto returnedVec = Vector<String>();

		if (source.Contains(key))
		{
			JSONArray typesArr = source.Get(key).GetArray();
			returnedVec.Reserve(typesArr.Size());
			for (unsigned int i = 0; i < typesArr.Size(); i++) {
				returnedVec.Push(typesArr[i].GetString());
			}
		}

		return returnedVec;
	}

	int JsonUtils::GetIntFromSourceJson(const JSONValue& source, const String& key, int defaultValue)
	{
		if (source.Contains(key))
		{
			return source.Get(key).GetInt();
		}

		return defaultValue;
	}

	float JsonUtils::GetFloatFromSourceJson(const JSONValue& source, const String& key, float defaultValue)
	{
		if (source.Contains(key))
		{
			return source.Get(key).GetFloat();
		}

		return defaultValue;
	}

	String JsonUtils::GetStringFromSourceJson(const JSONValue& source, const String& key, const String& defaultValue)
	{
		if (source.Contains(key))
		{
			return source.Get(key).GetString();
		}

		return defaultValue;
	}

	bool JsonUtils::GetBoolFromSourceJson(const JSONValue& source, const String& key, bool defaultValue)
	{
		if (source.Contains(key))
		{
			return source.Get(key).GetBool();
		}

		return defaultValue;
	}

	Vector2 JsonUtils::GetVector2FromSourceJson(const JSONValue& source, const String& key)
	{
		if (source.Contains(key))
		{
			auto& baseObj = source.Get(key);
			return Vector2(
				baseObj.Get("x").GetFloat(),
				baseObj.Get("y").GetFloat()
			);
		}

		return Vector2();
	}

	Vector3 JsonUtils::GetVector3FromSourceJson(const JSONValue& source, const String& key)
	{
		if (source.Contains(key))
		{
			auto& baseObj = source.Get(key);

			return Vector3(
				baseObj.Get("x").GetFloat(),
				baseObj.Get("y").GetFloat(),
				baseObj.Get("z").GetFloat()
			);
		}

		return Vector3();
	}

	JSONArray JsonUtils::SetupJsonArrayFromStringVector(const Vector<String>& stringVec)
	{
		JSONArray returnedArr = JSONArray(stringVec.Size());
		int i = 0;
		for (auto iter = stringVec.Begin(); iter != stringVec.End(); ++i) {
			returnedArr[i] = JSONValue(*iter);
			++iter;
		}
		return returnedArr;
	}

	JSONValue JsonUtils::SetupJsonValueFromVector2(const Vector2& vec)
	{
		JSONValue returnedValue = JSONValue();

		returnedValue.Set("x", vec.x_);
		returnedValue.Set("y", vec.y_);

		return returnedValue;
	}

	JSONValue JsonUtils::SetupJsonValueFromVector3(const Vector3& vec)
	{
		JSONValue returnedValue = JSONValue();

		returnedValue.Set("x", vec.x_);
		returnedValue.Set("y", vec.y_);
		returnedValue.Set("z", vec.z_);

		return returnedValue;
	}
}