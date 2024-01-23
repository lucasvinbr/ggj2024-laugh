#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/SoundListener.h>

using namespace Urho3D;

namespace Laugh {
	class GameAudio : public Object {
		URHO3D_OBJECT(GameAudio, Object);
	public:
		GameAudio(Context* c);
		~GameAudio() override = default;

		/// <summary>
		/// create base music source and the sound listener attached to the game camera
		/// </summary>
		void SetupSound();

		void StartMusic();
		void StopMusic();

		void PlayOneShotSoundWithFreqVariation(const String& soundFilePath, float soundFarDist, float gain = 1.0f, float freqVariation = 0.0f, bool is3dSound = false, Node* emittingNode = nullptr) const;
		void PlayOneShotSoundWithFrequency(const String& soundFilePath, float soundFarDist, float gain = 1.0f, float frequency = MAIN_AUDIO_FREQUENCY, bool is3dSound = false, Node* emittingNode = nullptr);

		SharedPtr<SoundSource> musicSource_;

		static GameAudio* instance_;

		/// <summary>
		/// hertz frequency sounds are assumed to be if a frequency isn't passed
		/// </summary>
		constexpr static float MAIN_AUDIO_FREQUENCY = 22050.0f;

	private:

};
}