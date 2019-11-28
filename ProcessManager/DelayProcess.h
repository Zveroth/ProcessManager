#pragma once
#include "ProcessBase.h"



//A simple process that doesn't do anything but delays others
class DelayProcess : public ProcessBase
{

public:

	explicit DelayProcess(float SecondsToDelay): m_TimeToDelay(SecondsToDelay), m_DelayedTime(0.0f) {};

	DelayProcess(const DelayProcess& Process) = delete;

protected:

	void OnUpdate(const float& DeltaTime) override;
	void OnSuccess() override;

private:

	//The amount of time that this process be delaying for
	float m_TimeToDelay;
	//Time delayed so far
	float m_DelayedTime;
};

void DelayProcess::OnUpdate(const float& DeltaTime)
{
	m_DelayedTime += DeltaTime;
	if (m_TimeToDelay <= m_DelayedTime)
		Succeed();//Succeed after a given amount of time
}

void DelayProcess::OnSuccess()
{
	//Utility call 
	printf("Delayed for %f sec (Real time: %f)\n", m_TimeToDelay, m_DelayedTime);
}