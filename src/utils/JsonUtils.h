#pragma once
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Container/Str.h>

using namespace Urho3D;

namespace Laugh {
	class JsonUtils {
	public:
		/// <summary>
		/// tries to add strings from the jsonArray to the string vector
		/// </summary>
		/// <param name="stringVec"></param>
		/// <param name="jsonArrayKey"></param>
		/// <returns>true if the jsonArray was found</returns>
		static bool FillStringVectorFromJsonArray(Vector<String>& stringVec, const JSONValue& source, const String& key);

		static Vector<String> CreateStringVectorFromJsonArray(const JSONValue& source, const String& key);

		static int GetIntFromSourceJson(const JSONValue& source, const String& key, int defaultValue = 0);

		static float GetFloatFromSourceJson(const JSONValue& source, const String& key, float defaultValue = 0.0f);

		static String GetStringFromSourceJson(const JSONValue& source, const String& key, const String& defaultValue = "");

		static bool GetBoolFromSourceJson(const JSONValue& source, const String& key, bool defaultValue = false);

		static Vector2 GetVector2FromSourceJson(const JSONValue& source, const String& key);

		static Vector3 GetVector3FromSourceJson(const JSONValue& source, const String& key);


		static JSONArray SetupJsonArrayFromStringVector(const Vector<String>& stringVec);

		static JSONValue SetupJsonValueFromVector2(const Vector2& vec);
		
		static JSONValue SetupJsonValueFromVector3(const Vector3& vec);
	};
}