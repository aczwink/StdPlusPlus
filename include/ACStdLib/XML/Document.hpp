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
#include "ACStdLib/Streams/InputStream.hpp"
#include "Element.hpp"

namespace ACStdLib
{
    namespace XML
    {
        class ACSTDLIB_API Document
        {
        private:
            //Members
            Element *pRootElement;

        public:
            //Constructor
            Document();

            //Destructor
            ~Document();

            //Inline
            inline Element &GetRootElement()
            {
                return *this->pRootElement;
            }

            inline void SetRootElement(Element *pElement)
            {
                if (this->pRootElement)
                    delete this->pRootElement;

                this->pRootElement = pElement;
            }

            //Functions
            static Document *Parse(InputStream &refInput);
        };
    }
}