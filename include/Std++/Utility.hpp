/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Type/Remove_cv.hpp>

namespace StdXX
{
	//Functions
	template <typename T>
	constexpr T&& Forward(typename Type::RemoveReference<T>::type& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}
	template <typename T>
	constexpr T&& Forward(typename Type::RemoveReference<T>::type&& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}

	template <typename T = void>
	struct GreaterThan
	{
		constexpr inline
		bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs > rhs;
		}
	};

	template <typename T = void>
	struct LessThan
	{
		constexpr inline
		bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs < rhs;
		}
	};

	template <typename T>
	constexpr typename Type::RemoveReference<T>::type&& Move(T&& reference) noexcept
	{
		return static_cast<typename Type::RemoveReference<T>::type&&>(reference);
	}

	template<typename T>
	void Swap(T &v1, T&v2)
	{
		T tmp(Move(v1));
		v1 = Move(v2);
		v2 = Move(tmp);
	}
}