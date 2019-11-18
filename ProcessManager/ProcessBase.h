#pragma once
#include "memory"



class ProcessBase
{

public:

	ProcessBase();
	/*Destructor is marked as virtual so the process 
	can be properly deleted without any memory leaks*/
	virtual ~ProcessBase();

	//Allow the manager to use the private and protected section
	friend class ProcessManager;

	//Possible states
	enum class ProcessState : unsigned char//This doesn't need more than 1 byte
	{
		//Not active
		UNINITIALIZED,//Just created
		PENDING,//Initialized and waiting to run
				//for example when this is a child process

		//Active
		RUNNING,//Currently active and running
		PAUSED,//Currently active but not running

		//Finished
		SUCCEDED,//Successfully finished
		FAILED,//Something went wrong
		ABORTED//Couldn't start
	};

	//Mark process as SUCCEDED
	void Succeed();
	//Mark process as FAILED
	void Fail();

	//Only running processes can be paused
	inline void Pause() { if(m_State == ProcessState::RUNNING) m_State = ProcessState::PAUSED; }
	//Only paused processes can be unpaused
	inline void UnPause() { if(m_State == ProcessState::PAUSED) m_State = ProcessState::RUNNING; }

	//Getters
	inline ProcessState GetState() const { return m_State; }
	inline bool IsActive() const { return (m_State == ProcessState::PAUSED || m_State == ProcessState::RUNNING); }
	inline bool IsFinished() const { return (m_State == ProcessState::SUCCEDED || m_State == ProcessState::FAILED || m_State == ProcessState::ABORTED); }
	inline bool IsPending() const { return (m_State == ProcessState::PENDING); }
	inline bool IsPaused() const { return (m_State == ProcessState::PAUSED); }

	//Child handling
	//Attaches the given process to the last child
	void AttachChild(std::shared_ptr<ProcessBase>& AttachedChild);
	//Gets and removes the child process
	std::shared_ptr<ProcessBase> ReleaseChildProcess();
	//Just get the child process
	inline std::shared_ptr<ProcessBase> GetChildProcess() { return m_ChildProcess; }

protected:

	//Interface section
	//These are the functions that define a process
	virtual void OnInit() { m_State = ProcessState::RUNNING; };//Called before the first update. Preapres the process
	virtual void OnUpdate(const float& DeltaTime) = 0;//This is where most of the logic happens
	virtual void OnSuccess() {};//Called after successfully finishing
	virtual void OnFailure() {};//Called after failing to finish
	virtual void OnAbort() {};//Called after aborting the process

private:

	ProcessState m_State;
	std::shared_ptr<ProcessBase> m_ChildProcess;

	void SetState(ProcessState State);
};