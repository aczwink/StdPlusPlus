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
//Local
#include "Global.hpp"

namespace StdXX::Type
{
    template <typename T> struct IsMoveAssignable : public IsAssignable<typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type> {};
    template< typename T > inline constexpr bool IsMoveAssignable_v = IsMoveAssignable<T>::value;

    template< typename T> struct IsTriviallyMoveAssignable : public IsTriviallyAssignable< typename std::add_lvalue_reference<T>::type, typename std::add_rvalue_reference<T>::type> {};
    template< typename T > inline constexpr bool IsTriviallyMoveAssignable_v = IsTriviallyMoveAssignable<T>::value;
}