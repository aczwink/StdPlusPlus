/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	inline uint32 CombineHashes(uint32 h1, uint32 h2)
	{
		return h1 ^ (h2 << 1);
	}

	template<typename T>
	struct HashFunction
	{
	};

	template <>
	struct HashFunction<uint32>
	{
		uint32 operator()(uint32 v) const
		{
			return v;
		}
	};

	template <>
	struct HashFunction<String>
	{
		uint32 operator()(const String& string) const
		{
			uint32 seed = 0xC70F6907;
			for(uint32 c : string)
			{
				seed = CombineHashes(seed, c);
			}
			return seed;
		}
	};
}