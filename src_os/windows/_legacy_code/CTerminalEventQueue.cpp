//Class header
#include "../../../headers/UI/CTerminalEventQueue.h"
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Public functions
bool CTerminalEventQueue::ProcessPendingEvents(int32 &refExitCode)
{
	DWORD nEvents, nEventsRead;
	HANDLE hStdIn;
	INPUT_RECORD ir;

	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	GetNumberOfConsoleInputEvents(hStdIn, &nEvents);

	while(nEvents)
	{
		//read event
		ReadConsoleInputW(hStdIn, &ir, 1, &nEventsRead);
		if(nEventsRead != 1)
			continue;

		nEvents--;
		if(nEvents == 0)
		{
			GetNumberOfConsoleInputEvents(hStdIn, &nEvents);
			continue;
		}

		//event type
		switch(ir.EventType)
		{
		case KEY_EVENT:
			OutputDebugString("KEY_EVENT");
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
			OutputDebugString("WINDOW_BUFFER_SIZE_EVENT");
			break;
		}
	}

	return true;
}