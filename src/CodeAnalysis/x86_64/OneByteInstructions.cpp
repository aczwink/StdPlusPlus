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
//Local
#include <Std++/Debug.hpp>
#include "Instructions/INT3.hpp"
#include "Instructions/JMP.hpp"

//Public methods
void x86_64_Decoder::DecodeOpcode_1byte(byte opcode)
{
	switch(opcode)
	{
	case 0xcc:
		this->instruction = new x86_64::INT3();
		break;
	case 0xe9:
	{
		this->DecodeOperands(1, OperandCoding::Imm32);
		this->instruction = new x86_64::JMP(true, this->operandInfo[0].imm);
	}
	break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
}