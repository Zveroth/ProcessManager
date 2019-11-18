#pragma once
#include "ProcessBase.h"



class DelayProcess : public ProcessBase
{

public:

	explicit DelayProcess(float SecondsToDelay);

	DelayProcess(const DelayProcess& Process) = delete;

protected:

	void OnUpdate(const float& DeltaTime) override;
	void OnSuccess() override;

private:

	float m_TimeToDelay;
	float m_DelayedTime;
};

DelayProcess::DelayProcess(float SecondsToDelay) : m_TimeToDelay(SecondsToDelay), m_DelayedTime(0.0f)
{

}

void DelayProcess::OnUpdate(const float& DeltaTime)
{
	m_DelayedTime += DeltaTime;
	if (m_TimeToDelay <= m_DelayedTime)
		Succeed();
}

void DelayProcess::OnSuccess()
{
	printf("Delayed for %f sec (Real time: %f)\n", m_TimeToDelay, m_DelayedTime);
}