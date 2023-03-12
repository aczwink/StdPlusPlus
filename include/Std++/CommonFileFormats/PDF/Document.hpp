/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/SeekableOutputStream.hpp>
#include <Std++/FileSystem/Path.hpp>
#include "Catalog.hpp"
#include "CrossReferenceTable.hpp"

namespace StdXX::CommonFileFormats::PDF
{
    class Document
    {
    public:
        //State
        Catalog catalog;

        //Methods
        Font *AddFont(const FileSystem::Path& fontPath);
        void CreateXRefFromCatalog();
        void Write(SeekableOutputStream& outputStream);

        //Inline
        inline float64 MMToUserUnit(float64 coord)
        {
            return coord * 72 / 25.4;
        }

        inline float64 UserUnitToMM(float64 coord)
        {
            return coord * 25.4 / 72;
        }

    private:
        //Members
        LinkedList<Font *> fonts;
        CrossReferenceTable xref;

        //Methods
        void WriteHeader(OutputStream& outputStream);
        void WriteTrailer(SeekableOutputStream& outputStream, uint32 xrefOffset);
    };
}