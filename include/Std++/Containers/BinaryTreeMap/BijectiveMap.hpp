/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include "BinaryTreeMap.hpp"

namespace StdXX
{
	template <typename T1, typename T2>
	class BijectiveMap
	{
	public:
		//Inline
		inline bool Contains(const T1 &key) const
		{
			return this->map.Contains(key);
		}

		inline const T2 &Get(const T1 &key) const
		{
			return this->map[key];
		}

		inline const T1 &GetReverse(const T2 &key) const
		{
			return this->back[key];
		}

		inline void Insert(const T1 &t1, const T2 &t2)
		{
			this->map.Insert(t1, t2);
			this->back.Insert(t2, t1);
		}

		//For range-based loop
		typename BinaryTreeMap<T1, T2>::Iterator begin()
		{
			return this->map.begin();
		}

		typename BinaryTreeMap<T1, T2>::ConstIterator begin() const
		{
			return this->map.begin();
		}

		typename BinaryTreeMap<T1, T2>::Iterator end()
		{
			return this->map.end();
		}

		typename BinaryTreeMap<T1, T2>::ConstIterator end() const
		{
			return this->map.end();
		}

	private:
		//Members
		BinaryTreeMap<T1, T2> map;
		BinaryTreeMap<T2, T1> back;
	};
}