#pragma once
#include "irrKlang.h"
#include "ProcessBase.h"
#include "SoundEffect.h"

irrklang::ISoundEngine* SoundEngine = nullptr;
//Don't do this, this is terrible, if there was an audio manager/system 
//I would just pass a pointer to it which would be stored maybe in a static variable in a *AllManager* class

class SFXPlayProcess : public ProcessBase
{

public:

	SFXPlayProcess(SoundEffect& Sound) : OwnedSound(nullptr), SoundBuffer(&Sound) {};

	void OnUpdate(const float& DeltaTime) override;
	void OnInit() override;
	void OnAbort() override;

private:

	SoundEffect* SoundBuffer;
	irrklang::ISound* OwnedSound;
};

void SFXPlayProcess::OnInit()
{
	OwnedSound = SoundEngine->play2D(SoundBuffer->FileName, SoundBuffer->bLooping, false, true, irrklang::ESM_AUTO_DETECT, true);

	irrklang::ISoundEffectControl* SFXControl = OwnedSound->getSoundEffectControl();

	if (SoundBuffer->SoundFlags & SoundEffectsFlags::ECHO)
		SFXControl->enableEchoSoundEffect();

	if (SoundBuffer->SoundFlags & SoundEffectsFlags::DISTORT)
		SFXControl->enableDistortionSoundEffect();

	if (SoundBuffer->SoundFlags & SoundEffectsFlags::WAVES_REVERB)
		SFXControl->enableWavesReverbSoundEffect();

	ProcessBase::OnInit();
}

void SFXPlayProcess::OnUpdate(const float& DeltaTime)
{
	if (OwnedSound->isFinished())
	{
		OwnedSound->drop();
		Succeed();
	}
}

void SFXPlayProcess::OnAbort()
{
	if (OwnedSound)
		OwnedSound->drop();
}