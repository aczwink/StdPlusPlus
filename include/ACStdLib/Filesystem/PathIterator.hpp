/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "Path.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API PathIterator
    {
    private:
        //Members
        void *pOSHandle;
        Path currentPath;

    public:
        //Constructors
        PathIterator(const Path &refPath, bool end = false);

        inline PathIterator(PathIterator &&refIt) //move ctor
        {
            this->pOSHandle = refIt.pOSHandle;
            refIt.pOSHandle = nullptr;
        }

        //Destructor
        ~PathIterator();

        //Operators
        PathIterator &operator++(); //Prefix++
        bool operator!=(const PathIterator &refOther) const;

        //Inline operators
        inline Path operator*()
        {
            return this->currentPath;
        }
    };
}