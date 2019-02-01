/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Debugging/FunctionHook.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/CodeAnalysis/Program.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Debugging;

#ifdef XPC_ARCH_X86_64
static uint8 WriteJump(void* functionAddress, void* redirectTargetAddress)
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}

bool FunctionHook::Hook(void* redirectTargetAddress)
{
	//analyze function to hook
	BufferInputStream input((byte *)this->functionAddress, 50); //50 bytes should be enough
	CodeAnalysis::Program program(input, (uint32)input.GetSize(), CodeAnalysis::Architecture::x86_64);

	//redirect
	uint8 jumpSize = WriteJump(this->functionAddress, redirectTargetAddress);
	return jumpSize != 0;
}

void FunctionHook::UnHook()
{
}
#endif