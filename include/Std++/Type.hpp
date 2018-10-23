/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	//Traits
	template <typename T, T v>
	struct IntegralConstant
	{
		typedef T ValueType;
		static constexpr ValueType value = v;
		constexpr operator ValueType() const noexcept { return value; }
		constexpr ValueType operator()() const noexcept { return value; }
	};
	template<bool B>
	using BoolConstant = IntegralConstant<bool, B>;

	//EnableIf
	template <bool, typename T = void>
	struct EnableIf{};
	template <typename T>
	struct EnableIf<true, T> { typedef T type; };

	//IsSameType
	template<typename T1, typename T2> struct IsSameType : public BoolConstant<false>{};
	template<typename T> struct IsSameType<T, T> : public BoolConstant <true>{};

	//IsTrivial
#if defined(XPC_COMPILER_GCC) || defined(XPC_COMPILER_CLANG)
	template <typename T>
	struct IsTrivial : public BoolConstant<__is_trivial(T)>{};
#endif
#ifdef XPC_COMPILER_MSVC
	template <typename T>
	struct IsTrivial : public BoolConstant<__is_trivial(T)>{};
#endif

	//RemoveReference
	template<typename T>
	struct RemoveReference
	{
		using type = T;
	};
	template <typename T>
	struct RemoveReference<T&>
	{
		using type = T;
	};
	
	template <typename T>
	struct RemoveReference<T&&>
	{
		using type = T;
	};
}