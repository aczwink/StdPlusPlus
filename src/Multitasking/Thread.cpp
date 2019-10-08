/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <Std++/Multitasking/Thread.hpp>
//Global
#ifdef XPC_OS_WINDOWS
#include <Windows.h>
#endif
//Local
#include <Std++/Errorhandling/Exception.hpp>
#include <Std++/Errorhandling/Error.hpp>
#include <Std++/Streams/Writers/StdOut.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static void ReportError(const String &message1, const String &message2)
{
	TextWriter textWriter(stdErr, TextCodecType::UTF8);
	textWriter << message1 << message2 << endl;
#ifdef XPC_OS_WINDOWS
	MessageBoxW(NULL, (LPCWSTR)message2.ToUTF16().GetRawZeroTerminatedData(), (LPCWSTR)message1.ToUTF16().GetRawZeroTerminatedData(), MB_ICONERROR | MB_TASKMODAL);
#endif
#ifdef XPC_COMPILER_MSVC
	OutputDebugStringW((LPCWSTR)(message1 + message2).ToUTF16().GetRawZeroTerminatedData());
#endif
	exit(EXIT_FAILURE);
}

//Private methods
void Thread::Run()
{
	if(this->function)
		this->function();
	else if(this->functor.IsBound())
		this->functor();
}

int32 Thread::ThreadMain()
{
	this->isAlive = true;
	try
	{
		this->Run();
	}
	catch(const Exception &e)
	{
		ReportError(u8"Uncaught exception: ", e.ToString());
	}
	catch(const Error &e)
	{
		ReportError(u8"ERROR: ", e.ToString());
	}
	catch(const BaseException &e)
	{
		ReportError(u8"BaseException: ", e.ToString());
	}
	catch(...)
	{
		ReportError(u8"Uncaught exception (not StdXX)", String());
	}
	this->isAlive = false;

	return EXIT_SUCCESS;
}