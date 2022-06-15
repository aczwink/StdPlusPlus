/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>

namespace StdXX::FileTypes
{
    class FileType
    {
    public:
        //Abstract
        /**
         * Should be an ordered list of file extensions. More common extensions should have a smaller index.
         */
        virtual FixedArray<String> GetExtensions() const = 0;
        /**
         * Should be an ordered list of internet media types. More common media types should have a smaller index.
         */
        virtual FixedArray<String> GetMediaTypes() const = 0;
        /**
         * The non-technical name of the type.
         * @return
         */
        virtual String GetName() const = 0;
        /**
         * Returns a value between 0 and 1, determining how good the given input matches this file type.
         */
        virtual float32 Probe(SeekableInputStream& seekableInputStream) const = 0;

        //Inline
        inline String GetPreferredExtension() const
        {
            return this->GetExtensions()[0];
        }

        inline String GetPreferredMediaType() const
        {
            return this->GetMediaTypes()[0];
        }
    };
}