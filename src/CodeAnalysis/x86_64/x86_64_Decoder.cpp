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
#include "x86_64_Decoder.hpp"
//Global
#include <cstdarg>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>

//Public methods
Instruction* x86_64_Decoder::DecodeInstruction(InputStream& input, uint8& decodedInstructionSize)
{
	//set decoder state to initial
	this->input = &input;
	DataReader reader(false, input);
	this->reader = &reader;

	this->decodedInstructionSize = 0;

	//begin decoding
	this->DecodePrefixes();
	this->DecodeOpcode();

	//finished decoding
	decodedInstructionSize = this->decodedInstructionSize;

	return this->instruction;
}

//Private methods
void x86_64_Decoder::DecodeModRM(const OperandCoding(&opCoding)[MAX_OPERANDS])
{
}

void x86_64_Decoder::DecodeOpcode()
{
	byte opcode = this->reader->ReadByte();
	this->decodedInstructionSize++;
	if(opcode == 0xF)
	{
		//2 or 3 byte opcode
		NOT_IMPLEMENTED_ERROR;
	}
	else
	{
		/*
		//we need to peek one byte here
		SeekableInputStream* seekableInputStream = dynamic_cast<SeekableInputStream *>(this->input);
		byte nextByte = this->reader->ReadByte();
		seekableInputStream->Rewind(1);

		byte opcodeExtension = static_cast<byte>((nextByte >> 3) & 7); //potentially needed
		*/
		this->DecodeOpcode_1byte(opcode);
	}
}

void x86_64_Decoder::DecodeOperands(uint8 nOperands, ...)
{
	uint8 i;
	va_list ap;
	OperandCoding coding[MAX_OPERANDS];

	//fetch args
	va_start(ap, nOperands);
	for(i = 0; i < nOperands; i++)
		coding[i] = va_arg(ap, OperandCoding);
	if(i < MAX_OPERANDS)
		coding[i] = OperandCoding::NotAvailable;
	va_end(ap);

	//set "default" op size, it is overwriteable by some instructions
	this->operandSize = this->GetOperandSize();

	//decode mod rm if needed
	this->DecodeModRM(coding);

	//now each operand
	for(i = 0; i < MAX_OPERANDS; i++)
	{
		OperandInfo &refOpInfo = this->operandInfo[i];

		switch(coding[i])
		{
			case OperandCoding::NotAvailable:
				return; //no more operands follow
			case OperandCoding::Imm32:
			{
				refOpInfo.type = OperandType::Immediate;
				this->operandSize = 32;
				refOpInfo.imm = this->ReadImmediate(32);
			}
				break;
			default:
				NOT_IMPLEMENTED_ERROR;
		}
	}
}

void x86_64_Decoder::DecodePrefixes()
{
}

uint16 x86_64_Decoder::GetOperandSize()
{
	uint16 size;

	//TODO: IN 16 BIT MODE THIS IS DIFFERENT
	size = 32;

	return size;
}

int64 x86_64_Decoder::ReadImmediate(uint16 operandSize)
{
	switch(operandSize)
	{
		case 8:
			this->decodedInstructionSize++;
			return this->reader->ReadByte();
		case 16:
			this->decodedInstructionSize += 2;
			return this->reader->ReadUInt16();
		case 32:
			this->decodedInstructionSize += 4;
			return this->reader->ReadUInt32();
	}

	NOT_IMPLEMENTED_ERROR;
	return -1;
}