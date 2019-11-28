#pragma once
#include "list"
#include "ProcessBase.h"

class ProcessManager
{

public:

	//There is nothing to initialize so there is no need for a constructor
	~ProcessManager();

	//Initializes, uodates and ends processes
	void Update(const float& DeltaTime);
	//Attaches new process
	std::weak_ptr<ProcessBase> AttachProcess(std::shared_ptr<ProcessBase> AttachedProcess);
	//Abort all processes either during the next update or immediately
	void AbortAllProcesses(bool bImmediately = true);

	//Getters
	inline unsigned int GetNumOfProcesses() { return m_ProcessList.size(); }

private:

	/*Pointers to attached processes, this doesn't contain pointers to 
	thier childern. They are added after a process successfully finishes.*/
	std::list<std::shared_ptr<ProcessBase>> m_ProcessList;

	//A utility function called by the destructor
	void ClearAllProcesses();
};