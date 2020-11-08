/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/__Globaldependencies.h>

namespace StdXX::Type
{
	template <typename... T>
	struct CommonType : public std::common_type<T...> {};

    template <typename T, typename Arg>
    struct IsAssignable : public std::is_assignable<T, Arg> {};

	template <typename T>
	struct IsIntegral : public std::is_integral<T>{};

	template <typename T>
	struct IsSigned : public std::is_signed<T>{};

    template <typename T, typename Arg> struct IsTriviallyAssignable : public std::is_trivially_assignable<T, Arg> {};
}