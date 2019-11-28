#include "assert.h"
#include "ProcessBase.h"


ProcessBase::ProcessBase() : m_ChildProcess(nullptr), m_State(ProcessState::UNINITIALIZED)
{
	
}

ProcessBase::~ProcessBase()
{
	if (m_ChildProcess)
		m_ChildProcess->OnAbort();

	//printf("\nProcess deleted\n\n");
}

void ProcessBase::AttachChild(std::shared_ptr<ProcessBase>& AttachedChild)
{
	if (m_ChildProcess)//If a process already has a child then we create a chain
		m_ChildProcess->AttachChild(AttachedChild);
	else
		m_ChildProcess = AttachedChild;
}

std::shared_ptr<ProcessBase> ProcessBase::ReleaseChildProcess()
{
	std::shared_ptr<ProcessBase> ReturnedChild = m_ChildProcess;
	m_ChildProcess.reset();
	return ReturnedChild;
}

void ProcessBase::SetState(ProcessState State)
{
	m_State = State;
}

void ProcessBase::Succeed()
{
	assert(m_State == ProcessState::RUNNING || m_State == ProcessState::PAUSED);//A process that isn't active can not finish
	m_State = ProcessState::SUCCEDED;
}

void ProcessBase::Fail()
{
	assert(m_State == ProcessState::RUNNING || m_State == ProcessState::PAUSED);//A process that isn't active can not finish
	m_State = ProcessState::FAILED;
}