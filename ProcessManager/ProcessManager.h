#pragma once
#include "list"
#include "ProcessBase.h"

class ProcessManager
{

public:

	//There is nothing to initialize so there is no need for a constructor
	~ProcessManager();

	void Update(const float& DeltaTime);
	std::weak_ptr<ProcessBase> AttachProcess(std::shared_ptr<ProcessBase> AttachedProcess);
	void AbortAllProcesses(bool bImmediately = true);

	//Getters
	inline unsigned int GetNumOfProcesses() { return m_ProcessList.size(); };


private:

	std::list<std::shared_ptr<ProcessBase>> m_ProcessList;

	void ClearAllProcesses();
};