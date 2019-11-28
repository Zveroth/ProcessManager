#include "ProcessManager.h"



ProcessManager::~ProcessManager()
{
	ClearAllProcesses();
}

void ProcessManager::Update(const float& DeltaTime)
{
	std::list<std::shared_ptr<ProcessBase>>::iterator Iter = m_ProcessList.begin();
	while (Iter != m_ProcessList.end())//Iterate over the process list
	{
		std::list<std::shared_ptr<ProcessBase>>::iterator CacheIter = Iter;//Cache the current iterator in case the current process needs to be removed
		Iter++;

		std::shared_ptr<ProcessBase> Process = *CacheIter;//Get the process

		if (Process->GetState() == ProcessBase::ProcessState::UNINITIALIZED)//Initialize if needed
			Process->OnInit();

		if (Process->GetState() == ProcessBase::ProcessState::RUNNING)//Update
			Process->OnUpdate(DeltaTime);

		if (Process->IsFinished())//Check whether a process is finished
		{
			switch(Process->GetState())//Check the result of a process
			{
			case ProcessBase::ProcessState::SUCCEDED:
			{
				Process->OnSuccess();
				if (std::shared_ptr<ProcessBase> ChildProc = Process->ReleaseChildProcess())
					AttachProcess(ChildProc);//If a process successfully finishes and has a child, then that child is added to the list
				break;
			}

			case ProcessBase::ProcessState::FAILED:
				Process->OnFailure();
				break;

			case ProcessBase::ProcessState::ABORTED:
				Process->OnAbort();
				break;
			}

			m_ProcessList.erase(CacheIter);//A process has finished it's thing and is now removed
			//Note that the process might still exist after since shared_ptr is used
		}
	}
}

std::weak_ptr<ProcessBase> ProcessManager::AttachProcess(std::shared_ptr<ProcessBase> AttachedProcess)
{
	m_ProcessList.push_front(AttachedProcess);
	return std::weak_ptr<ProcessBase>(AttachedProcess);//return a reference to a process, useful for processes that can be modified
}

void ProcessManager::AbortAllProcesses(bool bImmediately)
{
	std::list<std::shared_ptr<ProcessBase>>::iterator Iter = m_ProcessList.begin();
	while (Iter != m_ProcessList.end())
	{
		std::list<std::shared_ptr<ProcessBase>>::iterator CacheIter = Iter;
		Iter++;

		std::shared_ptr<ProcessBase> Process = *CacheIter;
		if (Process->IsActive())//Uninitialized and finished processes don't need to be aborted
		{
			Process->SetState(ProcessBase::ProcessState::ABORTED);
			if (bImmediately)
			{
				Process->OnAbort();
				m_ProcessList.erase(CacheIter);
				//Note: Uninitialized processes will persist in the list
				//,but child processes will not
			}
		}
	}
}

void ProcessManager::ClearAllProcesses()
{
	m_ProcessList.clear();
}