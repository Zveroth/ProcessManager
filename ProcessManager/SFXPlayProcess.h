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

	//The sound doesn't start playing here. It starts during OnInit()
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
	if(SoundBuffer->SoundFlags == SoundEffectsFlags::NONE)
	{
		//If no flags are set then there is no need to get the effect controller and bUseEffects can be set to false
		OwnedSound = SoundEngine->play2D(SoundBuffer->FileName, SoundBuffer->bLooping, false, true);
	}
	else
	{
		//Play the sound
		OwnedSound = SoundEngine->play2D(SoundBuffer->FileName, SoundBuffer->bLooping, false, true, irrklang::ESM_AUTO_DETECT, true);

		//Get the effects controller
		irrklang::ISoundEffectControl* SFXControl = OwnedSound->getSoundEffectControl();

		//Apply effects based on flags
		if (SoundBuffer->SoundFlags & SoundEffectsFlags::ECHO)
			SFXControl->enableEchoSoundEffect();

		if (SoundBuffer->SoundFlags & SoundEffectsFlags::DISTORT)
			SFXControl->enableDistortionSoundEffect();

		if (SoundBuffer->SoundFlags & SoundEffectsFlags::WAVES_REVERB)
			SFXControl->enableWavesReverbSoundEffect();
	}

	//Remember to class this or just set the m_State to running
	ProcessBase::OnInit();
}

void SFXPlayProcess::OnUpdate(const float& DeltaTime)
{
	if (OwnedSound->isFinished())//This process succeeds after the played sound has finished
	{
		OwnedSound->drop();
		Succeed();
	}
}

void SFXPlayProcess::OnAbort()
{
	if (OwnedSound)//The sound is removed when this process is aborted
		OwnedSound->drop();
}