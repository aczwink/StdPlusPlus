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
//Local
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/CodeAnalysis/Program.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Signed.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Debugging;

static const uint16 c_max_func_size = 1024;

#ifdef XPC_ARCH_X86_64
static void WriteJump(void* functionAddress, void* targetAddress, uint8 jmpSize)
{
	int64 relativeDistance = int64(targetAddress) - int64(functionAddress) - jmpSize;

	MemoryProtect(functionAddress, jmpSize, MemoryProtection::Execute_Read_Write);

	switch (jmpSize)
	{
	case 2:
	{
		byte* writeTarget = (byte*)functionAddress;

		*writeTarget++ = 0xeb;
		*writeTarget = byte(relativeDistance & 0xFF);
	}
	break;
	case 5:
	{
		//write opcode
		byte* writeTarget = (byte*)functionAddress;
		*writeTarget++ = 0xe9;

		//write offset
		int32* writeTargetInt32 = (int32*)writeTarget;
		*writeTargetInt32 = int32(relativeDistance);
	}
	break;
	case 14:
	{
		byte* writeTarget = (byte*)functionAddress;
		//write jmp[rip+addr]

		//opcode
		*writeTarget++ = 0xFF;
		*writeTarget++ = 0x25;

		//addr (4byte size) is 0
		*writeTarget++ = 0;
		*writeTarget++ = 0;
		*writeTarget++ = 0;
		*writeTarget++ = 0;

		//rip
		*((uint64 *)writeTarget) = (uint64)targetAddress;
	}
	break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}

	MemoryProtect(functionAddress, jmpSize, MemoryProtection::Execute_Read);
}

#include <Std++/Streams/StdOut.hpp>

bool FunctionHook::Hook(void* redirectTargetAddress)
{
	//read in complete function
	BufferInputStream input((byte *)this->functionAddress, c_max_func_size);
	CodeAnalysis::Program program(input, (uint32)input.GetSize(), CodeAnalysis::Architecture::x86_64);
	
	CodeAnalysis::AnalyzedProcedure proc = program.AnalyzeProcedure(0); //analyze procedure directly at the offset that we should hook
	uint32 procSize = proc.ComputeSize();
	
	const uint8 jmpSizes[] = {2, 5, 14}; //TODO: a 64 bit jump is also available
	const int64 maxDist[] = { Signed<int8>::Max(), Signed<int32>::Max(), Signed<int64>::Max() };
	for (uint8 i = 0; i < sizeof(jmpSizes) / sizeof(jmpSizes[0]); i++)
	{
		if (procSize < jmpSizes[i])
			return false; //not hookable... we need more space for a jump

		int64 relativeDistance = int64(redirectTargetAddress) - int64(this->functionAddress) - jmpSizes[i];
		relativeDistance = Math::Abs(relativeDistance);
		if (relativeDistance < maxDist[i])
		{
			//backup original code
			this->origBytes = jmpSizes[i];
			MemCopy(this->origFuncCode, this->functionAddress, this->origBytes);

			//create trampoline
			if (this->trampoline)
				VirtualMemoryFree(this->trampoline, this->trampolineSize);

			//TODO: the detour jump might kill flags registers... in this case we would have to save the state, jump to detour
			//TODO: when calling trampoline, restore state, then jump to original code... but this again breaks flags...

			//TODO: the trampoline is also difficult since it destroys relative jumps...

			//copy instructions before jmpSize and a relative jump back
			const uint8 MAX_JUMP_SIZE = 14;
			uint8 prologSize = 0;
			uint8 j = 0;
			while (prologSize < jmpSizes[i])
			{
				prologSize += proc.GetInstruction(j).size;
				j++;
			}
			this->trampoline = VirtualMemoryAllocate(prologSize + MAX_JUMP_SIZE, MemoryProtection::Execute_Read_Write);
			MemCopy(this->trampoline, this->functionAddress, prologSize); //copy function prolog to trampoline //TODO: duh... this is problematic
			WriteJump((byte*)this->trampoline + prologSize, (byte*)this->functionAddress + prologSize, MAX_JUMP_SIZE);

			//now detour
			WriteJump(this->functionAddress, redirectTargetAddress, jmpSizes[i]);

			return true;
		}
	}

	return false;
}

void FunctionHook::UnHook()
{
	MemoryProtect(functionAddress, this->origBytes, MemoryProtection::Execute_Read_Write);
	MemCopy(this->functionAddress, this->origFuncCode, this->origBytes);
	MemoryProtect(functionAddress, this->origBytes, MemoryProtection::Execute_Read);
}
#endif