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
#include <Std++/CodeAnalysis/Instructions/JumpInstruction.hpp>
//Namespaces
using namespace StdXX::CodeAnalysis;

namespace x86_64
{
	class JMP : public JumpInstruction
	{
	public:
		//Members
		bool relative;
		int64 offset;

		//Constructor
		inline JMP(bool relative, int64 offset)
		{
			this->relative = relative;
			this->offset = offset;
		}

		//Methods
		uint8 GetSize() const;
	};
}