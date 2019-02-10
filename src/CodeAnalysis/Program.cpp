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
#include <Std++/CodeAnalysis/Decoder.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CodeAnalysis;

//Constructor
Program::Program(InputStream& inputStream, uint32 programSize, Architecture architecture)
{
	Decoder *pDecoder = Decoder::CreateInstance(architecture);
	while(programSize)
	{
		Instruction *pInstruction = pDecoder->DecodeInstruction(inputStream);
		if(pInstruction->GetSize() > programSize)
			break;
		programSize -= pInstruction->GetSize();

		this->instructions.Push(pInstruction);
	}

	delete pDecoder;
}

//Destructor
Program::~Program()
{
	for(Instruction *const& refpInstruction : this->instructions)
		delete refpInstruction;
}