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
#include <Std++/_Backends/UI/ClipboardBackend.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/_Backends/BackendManager.hpp>

namespace StdXX::UI
{
    class Clipboard
    {
    public:
        //Constructor
        inline Clipboard()
        {
            this->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateClipboardBackend();
        }

        //Methods
        inline void Store()
        {
            this->backend->Store();
        }

        inline void Store(const String& text)
        {
            this->backend->Store(text);
        }

    private:
        //Members
        UniquePointer<_stdxx_::ClipboardBackend> backend;
    };
}