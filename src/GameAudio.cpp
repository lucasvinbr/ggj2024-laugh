#include "Debug.h"
#include "World.h"
#include "GameConsts.h"
#include "GameAudio.h"


using namespace Urho3D;

namespace Laugh {
	GameAudio* GameAudio::instance_ = nullptr;

	Laugh::GameAudio::GameAudio(Context* c) : Object(c)
	{
		instance_ = this;
	}

	void GameAudio::SetupSound()
	{
		musicSource_ = World::instance_->scene_->CreateComponent<SoundSource>();
		musicSource_->SetSoundType(SOUND_MUSIC);

		auto listener = World::instance_->camera_->GetNode()->CreateComponent<SoundListener>();
		GetSubsystem<Audio>()->SetListener(listener);
	}

	void GameAudio::StartMusic()
	{
	}

	void GameAudio::StopMusic()
	{
	}

	void GameAudio::PlayOneShotSoundWithFreqVariation(const String& soundFilePath, float soundFarDist, float gain, float freqVariation, bool is3dSound, Node* emittingNode) const
	{
		auto cache = GetSubsystem<ResourceCache>();
		auto sound = cache->GetResource<Sound>(soundFilePath);

		if (!emittingNode) {
			emittingNode = World::instance_->scene_;
		}

		SoundSource* source;

		if (is3dSound) {
			source = emittingNode->CreateComponent<SoundSource3D>();
			((SoundSource3D*)source)->SetFarDistance(soundFarDist);
		}
		else {
			source = emittingNode->CreateComponent<SoundSource>();
		}

		source->SetSoundType(SOUND_EFFECT);
		source->SetAutoRemoveMode(REMOVE_COMPONENT);

		source->Play(sound, MAIN_AUDIO_FREQUENCY + Random(-freqVariation, freqVariation), gain);
	}

	void GameAudio::PlayOneShotSoundWithFrequency(const String& soundFilePath, float soundFarDist, float gain, float frequency, bool is3dSound, Node* emittingNode)
	{
	}


}