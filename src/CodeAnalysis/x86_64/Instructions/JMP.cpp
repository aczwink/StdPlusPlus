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
#include "JMP.hpp"
//Local
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Debug.hpp>
//Namespaces
using namespace x86_64;
using namespace StdXX;

//Public methods
String x86_64::JMP::ToString() const
{
	return u8"jmp " + String((this->relative && (this->offset >= 0)) ? u8"+" : u8"") + String::HexNumber(this->offset);
}
