/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "Std++/Errorhandling/Exception.hpp"
#include "Std++/FileSystem/Path.hpp"

namespace StdXX::ErrorHandling
{
    class STDPLUSPLUS_API FileNotFoundException : public Exception
    {
    public:
        //Members
		FileSystem::Path path;

        //Constructor
        inline FileNotFoundException(const FileSystem::Path &refPath)
        {
            this->path = refPath;
        }

        //Inline
        inline String Description() const
        {
            return u8"No such file or directory: \"" + this->path.GetString() + u8"\"";
        }
    };
}