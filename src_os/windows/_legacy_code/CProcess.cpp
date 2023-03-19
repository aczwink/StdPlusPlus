//Class header
#include "../../../headers/Multitasking/CProcess.h"
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;

//Constructor
CProcess::CProcess(const CString &refProgram, const CLinkedList<CString> &refArguments)
{
	CUTF16String commandLine;
	STARTUPINFOW startupInfo = {0};
	PROCESS_INFORMATION processInfo;
	
	//convert program name and args
	commandLine += refProgram.GetUTF16();
	for(const CString &refArg : refArguments)
	{
		commandLine += ' ';
		
		if(refArg.Contains(' '))
		{
			commandLine += '"';
			commandLine += refArg.GetUTF16();
			commandLine += '"';
		}
		else
		{
			commandLine += refArg.GetUTF16();
		}
	}

	//create process
	startupInfo.cb = sizeof(startupInfo);
	
	CreateProcessW(nullptr, (LPWSTR)commandLine.GetC_Str(), nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startupInfo, &processInfo);
	
	this->pOSHandle = processInfo.hProcess;
}