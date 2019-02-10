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
#include <Std++/CodeAnalysis/Program.hpp>
//Local
#include <Std++/CodeAnalysis/Instructions/JumpInstruction.hpp>
#include <Std++/CodeAnalysis/Decoder.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CodeAnalysis;

//Constructor
Program::Program(InputStream& inputStream, uint32 programSize, Architecture architecture)
{
	uint32 offset = 0;
	
	Decoder *pDecoder = Decoder::CreateInstance(architecture);
	while(programSize)
	{
		uint8 decInstructionSize;
		Instruction *instruction = pDecoder->DecodeInstruction(inputStream, decInstructionSize);
		if(decInstructionSize > programSize)
			break;
		
		this->instructions.Push({ offset, decInstructionSize, instruction });

		offset += decInstructionSize;
		programSize -= decInstructionSize;
	}

	delete pDecoder;
}

//Public methods
AnalyzedProcedure Program::AnalyzeProcedure(uint32 offset) const
{
	//find first instruction
	uint32 index = 0;
	while ((index < this->instructions.GetNumberOfElements()) && (this->instructions[index].offset < offset))
		index++;
	ASSERT((index < this->instructions.GetNumberOfElements()) && (this->instructions[index].offset == offset), u8"Procedure can't start at the middle of an instruction");

	//analyze procedure and in particular find end of proc
	AnalyzedProcedure result;
	
	bool foundEndOfFunc = false;
	while (!foundEndOfFunc)
	{
		const DecodedInstruction& instr = this->instructions[index++];
		result.AddInstruction(instr);

		//TODO: this does not analyze for branches etc. I.e. it only searches for the first terminating instruction. However, this may be false as it just termiantes one execution path (i.e. branches)
		
		if (IS_INSTANCE_OF(instr.instruction.operator->(), JumpInstruction))
		{
			//found unconditional jump. this terminates a proc/block
			foundEndOfFunc = true;
		}
	}

	return result;
}

String Program::ToString() const
{
	String result;

	for (const DecodedInstruction& instr : this->instructions)
		result += String::HexNumber(instr.offset, 8, false) + u8"  " + instr.instruction->ToString() + u8"\r\n";
	
	return result;
}