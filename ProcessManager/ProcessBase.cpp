#include "assert.h"
#include "ProcessBase.h"


ProcessBase::ProcessBase() : m_ChildProcess(nullptr), m_State(ProcessState::UNINITIALIZED)
{
	
}

ProcessBase::~ProcessBase()
{
	if (m_ChildProcess)
		m_ChildProcess->OnAbort();
}

void ProcessBase::AttachChild(std::shared_ptr<ProcessBase>& AttachedChild)
{
	if (m_ChildProcess)
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
	assert(m_State == ProcessState::PENDING || m_State == ProcessState::PAUSED);
	m_State = ProcessState::SUCCEDED;
}

void ProcessBase::Fail()
{
	assert(m_State == ProcessState::PENDING || m_State == ProcessState::PAUSED);
	m_State = ProcessState::FAILED;
}