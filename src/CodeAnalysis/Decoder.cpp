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
#include <Std++/CodeAnalysis/Decoder.hpp>
//Local
#include <Std++/CodeAnalysis/Architecture.hpp>
#include "x86_64/x86_64_Decoder.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::CodeAnalysis;

//Class functions
Decoder *Decoder::CreateInstance(Architecture architecture)
{
	switch(architecture)
	{
		case Architecture::x86_64:
			return new x86_64_Decoder;
	}

	return nullptr;
}