//Class Header
#include "../../../headers/Time/CTimer.h"
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;

static void TimerCallback(void *pTimerCallbackFunc, BOOLEAN timerOrWaitFired)
{
	(*(CFunction<void()> *)pTimerCallbackFunc)();
}

//Constructor
CTimer::CTimer(const CFunction<void()> &refTimerCallback)
{
	this->pOSHandle = nullptr;
	this->timerCallback = refTimerCallback;
}

//Destructor
CTimer::~CTimer()
{
	this->Stop();
}

//Public methods
void CTimer::Start(uint32 timeOut_msec)
{
	if(this->pOSHandle)
		ChangeTimerQueueTimer(nullptr, this->pOSHandle, timeOut_msec, timeOut_msec);
	else
		CreateTimerQueueTimer(&this->pOSHandle, nullptr, (WAITORTIMERCALLBACK)TimerCallback, &this->timerCallback, timeOut_msec, timeOut_msec, WT_EXECUTEDEFAULT);
}

void CTimer::StartOnce(uint32 timeOut_msec)
{
	/*
		according to https://msdn.microsoft.com/de-de/library/windows/desktop/ms682004(v=vs.85).aspx
		//we can't update one-shot timers
	*/
	this->Stop();
	
	CreateTimerQueueTimer(&this->pOSHandle, nullptr, (WAITORTIMERCALLBACK)TimerCallback, &this->timerCallback, timeOut_msec, 0, WT_EXECUTEDEFAULT);
}

void CTimer::Stop()
{
	if(this->pOSHandle)
	{
		DeleteTimerQueueTimer(nullptr, this->pOSHandle, nullptr);
		this->pOSHandle = nullptr;
	}
}