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
//Local
#include <Std++/CodeAnalysis/Decoder.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CodeAnalysis;

/*
Most info from:
http://wiki.osdev.org/X86-64_Instruction_Encoding
*/
enum class OperandCoding
{
	NotAvailable,
	//immediates
	Imm32
};

enum class OperandType
{
	Immediate,
};

struct OperandInfo
{
	OperandType type;
	int64 imm;
};

class x86_64_Decoder : public Decoder
{
	//Constants
	static const uint32 MAX_OPERANDS = 4;
public:
	//Methods
	Instruction* DecodeInstruction(InputStream &input, uint8& decodedInstructionSize) override;

private:
	//Members
	InputStream* input;
	DataReader* reader;
	Instruction* instruction;
	uint8 decodedInstructionSize;
	uint16 operandSize;
	OperandInfo operandInfo[MAX_OPERANDS];

	//Methods
	void DecodeModRM(const OperandCoding (&opCoding)[MAX_OPERANDS]);
	void DecodeOpcode();
	void DecodeOpcode_1byte(byte opcode);
	void DecodeOperands(uint8 nOperands, ...);
	void DecodePrefixes();
	uint16 GetOperandSize();
	int64 ReadImmediate(uint16 operandSize);
};