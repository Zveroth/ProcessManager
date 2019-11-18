#include "ProcessManager.h"



ProcessManager::~ProcessManager()
{
	ClearAllProcesses();
}

void ProcessManager::Update(const float& DeltaTime)
{
	std::list<std::shared_ptr<ProcessBase>>::iterator Iter = m_ProcessList.begin();
	while (Iter != m_ProcessList.end())
	{
		std::list<std::shared_ptr<ProcessBase>>::iterator CacheIter = Iter;
		Iter++;

		std::shared_ptr<ProcessBase> Process = *CacheIter;

		if (Process->GetState() == ProcessBase::ProcessState::UNINITIALIZED)
			Process->OnInit();

		if (Process->GetState() == ProcessBase::ProcessState::RUNNING)
			Process->OnUpdate(DeltaTime);

		if (Process->IsFinished())
		{
			switch(Process->GetState())
			{
			case ProcessBase::ProcessState::SUCCEDED:
			{
				Process->OnSuccess();
				if (std::shared_ptr<ProcessBase> ChildProc = Process->ReleaseChildProcess())
					AttachProcess(ChildProc);
				break;
			}

			case ProcessBase::ProcessState::FAILED:
				Process->OnFailure();
				break;

			case ProcessBase::ProcessState::ABORTED:
				Process->OnAbort();
				break;
			}

			m_ProcessList.erase(CacheIter);
		}
	}
}

std::weak_ptr<ProcessBase> ProcessManager::AttachProcess(std::shared_ptr<ProcessBase> AttachedProcess)
{
	m_ProcessList.push_front(AttachedProcess);
	return std::weak_ptr<ProcessBase>(AttachedProcess);
}

void ProcessManager::AbortAllProcesses(bool bImmediately)
{
	std::list<std::shared_ptr<ProcessBase>>::iterator Iter = m_ProcessList.begin();
	while (Iter != m_ProcessList.end())
	{
		std::list<std::shared_ptr<ProcessBase>>::iterator CacheIter = Iter;
		Iter++;

		std::shared_ptr<ProcessBase> Process = *CacheIter;
		if (Process->IsActive())
		{
			Process->SetState(ProcessBase::ProcessState::ABORTED);
			if (bImmediately)
			{
				Process->OnAbort();
				m_ProcessList.erase(CacheIter);
			}
		}
	}
}

void ProcessManager::ClearAllProcesses()
{
	m_ProcessList.clear();
}