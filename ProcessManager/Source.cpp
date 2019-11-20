#pragma once

#include "stdio.h"
#include "chrono"

#include "irrKlang.h"
#include "ProcessManager.h"
#include "DelayProcess.h"
#include "SFXPlayProcess.h"

int main()
{
	SoundEngine = irrklang::createIrrKlangDevice();
	ProcessManager* PManager = new ProcessManager();

	{
		std::shared_ptr<ProcessBase> DProcess(new DelayProcess(1.0f));
		std::shared_ptr<ProcessBase> DProcessA(new DelayProcess(1.0f));
		std::shared_ptr<ProcessBase> DProcessB(new DelayProcess(1.0f));
		PManager->AttachProcess(DProcess);
		DProcess->AttachChild(DProcessA);
		DProcess->AttachChild(DProcessB);

		std::shared_ptr<ProcessBase> DProcessC(new DelayProcess(2.0f));
		PManager->AttachProcess(DProcessC);

		std::shared_ptr<ProcessBase> DProcessD(new DelayProcess(5.0f));
		PManager->AttachProcess(DProcessD);

		std::shared_ptr<ProcessBase> DProcessE(new DelayProcess(5.0f));
		PManager->AttachProcess(DProcessE);

		SoundEffect Effect("sound.ogg", false, (SoundEffectsFlags::WAVES_REVERB));

		std::shared_ptr<ProcessBase> SoundProcess(new SFXPlayProcess(Effect));
		DProcessE->AttachChild(SoundProcess);
	}

	{
		SoundEffect Effect("sound.ogg", false);

		std::shared_ptr<ProcessBase> SoundProcess(new SFXPlayProcess(Effect));
		PManager->AttachProcess(SoundProcess);
	}


	std::chrono::time_point<std::chrono::high_resolution_clock> Point = std::chrono::high_resolution_clock::now();

	while (true)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> ThisFramePoint = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> DeltaTime = ThisFramePoint - Point;

		PManager->Update(DeltaTime.count());

		Point = ThisFramePoint;
	}

	delete PManager;
	SoundEngine->drop();
	return 0;
}