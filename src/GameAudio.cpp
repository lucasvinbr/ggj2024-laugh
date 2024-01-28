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

	void GameAudio::SetSoundVolume(float volume) const
	{
		GetSubsystem<Audio>()->SetMasterGain(SOUND_MASTER, volume);
	}

	void GameAudio::StartMusic() const
	{
		String sndFilePath = "Sounds/ggj2024-laugh/mixing-" + String(Random(1, 4)) + ".wav";

		auto cache = GetSubsystem<ResourceCache>();
		auto sound = cache->GetResource<Sound>(sndFilePath);

		sound->SetLooped(true);

		musicSource_->Play(sound, MAIN_AUDIO_FREQUENCY + Random(-2000, 2000), 1.0f);
	}

	void GameAudio::StopMusic() const
	{
		// quickly fade out music volume
		SharedPtr<ValueAnimation> volumeAnim(new ValueAnimation(context_));
		volumeAnim->SetInterpolationMethod(IM_LINEAR);
		volumeAnim->SetKeyFrame(0.0f, 1.0f);
		volumeAnim->SetKeyFrame(0.5f, 0.0f);
		musicSource_->SetAttributeAnimation("Gain", volumeAnim, WM_ONCE);

	}

	void GameAudio::PlayTitleSound() const
	{
		String sndFilePath = "Sounds/ggj2024-laugh/title-" + String(Random(1, 6)) + ".wav";
		PlayOneShotSoundWithFreqVariation(sndFilePath, 1.0f, 2000);
	}

	void GameAudio::PlayClickSound() const
	{
		String sndFilePath = "Sounds/ggj2024-laugh/click-" + String(Random(1, 3)) + ".wav";
		PlayOneShotSoundWithFreqVariation(sndFilePath, 1.0f, 2000);
	}

	void GameAudio::PlayPickItemSound() const
	{
		String sndFilePath = "Sounds/ggj2024-laugh/pick-" + String(Random(1, 5)) + ".wav";
		PlayOneShotSoundWithFreqVariation(sndFilePath, 1.0f, 2000);
	}

	void GameAudio::PlayGotItemSound() const
	{
		String sndFilePath = "Sounds/ggj2024-laugh/tada-" + String(Random(1, 3)) + ".wav";
		PlayOneShotSoundWithFreqVariation(sndFilePath, 1.0f, 2000);
	}


	void GameAudio::PlayOneShotSoundWithFreqVariation(const String& soundFilePath, float gain, float freqVariation) const
	{
		auto cache = GetSubsystem<ResourceCache>();
		auto sound = cache->GetResource<Sound>(soundFilePath);

		auto emittingNode = World::instance_->scene_;
		auto source = emittingNode->CreateComponent<SoundSource>();

		source->SetSoundType(SOUND_EFFECT);
		source->SetAutoRemoveMode(REMOVE_COMPONENT);

		source->Play(sound, MAIN_AUDIO_FREQUENCY + Random(-freqVariation, freqVariation), gain);
	}

	void GameAudio::PlayOneShot3DSoundWithFreqVariation(const String& soundFilePath, float soundFarDist, float gain, float freqVariation, Node* emittingNode) const
	{
		auto cache = GetSubsystem<ResourceCache>();
		auto sound = cache->GetResource<Sound>(soundFilePath);

		if (!emittingNode) {
			emittingNode = World::instance_->scene_;
		}

		auto source = emittingNode->CreateComponent<SoundSource3D>();
		((SoundSource3D*)source)->SetFarDistance(soundFarDist);

		source->SetSoundType(SOUND_EFFECT);
		source->SetAutoRemoveMode(REMOVE_COMPONENT);

		source->Play(sound, MAIN_AUDIO_FREQUENCY + Random(-freqVariation, freqVariation), gain);
	}

	void GameAudio::PlayOneShotSoundWithFrequency(const String& soundFilePath, float soundFarDist, float gain, float frequency, bool is3dSound, Node* emittingNode)
	{
	}


}