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
#pragma once
//Local
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Function.hpp>

namespace StdXX::UI
{
    template <typename... Args>
    class Signal
    {
    public:
        //Methods
        inline void Connect(Function<void(Args...)>&& function)
        {
            this->slots.Push(Move(function));
        }

        template <typename T>
        inline void Connect(T* instance, void (T::*method)(Args...))
        {
            this->slots.Push(Function<void(Args...)>(method, instance));
        }

        inline void Emit(const Args&... args)
        {
            for(const auto& slot : this->slots)
                slot(args...);
        }

    private:
        //Members
        DynamicArray<Function<void(Args...)>> slots;
    };
}