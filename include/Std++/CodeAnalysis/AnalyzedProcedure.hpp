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
#include "Instruction.hpp"

namespace StdXX
{
	namespace CodeAnalysis
	{
		class STDPLUSPLUS_API AnalyzedProcedure
		{
		public:
			//Inline
			inline void AddInstruction(const DecodedInstruction& instruction)
			{
				this->instructions.Push(&instruction);
			}

			inline uint32 ComputeSize() const
			{
				uint32 size = 0;
				for (const DecodedInstruction* instr : this->instructions)
					size += instr->size;
				return size;
			}

			inline const DecodedInstruction& GetInstruction(uint32 index) const
			{
				return *this->instructions[index];
			}

		private:
			//Members
			DynamicArray<const DecodedInstruction*> instructions;
		};
	}
}