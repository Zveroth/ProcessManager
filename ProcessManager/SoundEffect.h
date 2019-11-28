#pragma once

//Flags that decide what effects will be activated on a sound when it starts playing
struct SoundEffectsFlags
{
	enum Value : unsigned int//enum class isn't used since I use (ProvidedFlags & NeededFlag) and for some reason it doesn't work with class
	{
		NONE = 0,
		DISTORT = (1 << 0),//You can use bit shifting
		ECHO = 2,//just write the values
		WAVES_REVERB = 0x04//or write in hexadecimals
	};
};

class SoundEffect
{

	friend class SFXPlayProcess;

public:

	SoundEffect(const char* SFXFileName = nullptr, bool Looping = false, SoundEffectsFlags::Value Flags = SoundEffectsFlags::NONE) : FileName(SFXFileName), bLooping(false), SoundFlags(Flags) {};

	inline void SetFile(char* SFXFileName) { FileName = SFXFileName; }
	inline void SetLooping(bool Looping) { bLooping = Looping; }
	inline void SetEffectFlags(SoundEffectsFlags::Value Flags) { SoundFlags = Flags; }

private:

	const char* FileName;
	bool bLooping;
	//float InnerDistance;
	//Vector Position
	SoundEffectsFlags::Value SoundFlags;
};