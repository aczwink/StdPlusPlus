/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Enumeration/SourceEnumeratorBuilder.hpp>

//Forward declarations
namespace StdXX
{
	template<typename KeyType, typename ValueType>
	class BinaryTreeMap;
}

namespace _stdxx_
{
	template<typename KeyType, typename ValueType>
	class BinaryTreeMapEnumerator : public StdXX::Enumerator<const StdXX::KeyValuePair<KeyType, ValueType>&>
	{
	public:
		//Constructor
		inline BinaryTreeMapEnumerator(const StdXX::BinaryTreeMap<KeyType, ValueType>& map) : it(map.begin()), end(map.end()), first(true)
		{
		}

		const StdXX::KeyValuePair<KeyType, ValueType>& GetCurrent() const override
		{
			return *this->it;
		}

		bool MoveForward() override
		{
			if(this->it == this->end)
				return false;
			if(this->first)
			{
				this->first = false;
				return true;
			}
			++this->it;

			return this->it != this->end;
		}

	private:
		//Members
		StdXX::ConstMapIterator<KeyType, ValueType> it;
		StdXX::ConstMapIterator<KeyType, ValueType> end;
		bool first;
	};

	template<typename KeyType, typename ValueType>
	class BinaryTreeMapEnumeratorBuilder : public StdXX::SourceEnumeratorBuilder<const StdXX::KeyValuePair<KeyType, ValueType>&>
	{
	public:
		//Constructor
		inline BinaryTreeMapEnumeratorBuilder(const StdXX::BinaryTreeMap<KeyType, ValueType>& map) : map(map)
		{
		}

		StdXX::Enumerator<const StdXX::KeyValuePair<KeyType, ValueType>&>* CreateSourceEnumerator() const override
		{
			return new BinaryTreeMapEnumerator<KeyType, ValueType>(this->map);
		}

	private:
		//Members
		const StdXX::BinaryTreeMap<KeyType, ValueType>& map;
	};
}