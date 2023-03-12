/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/FormalLanguages/CharacterClass.hpp>
#include "RegExAST.hpp"

namespace _stdxx_
{
    class RegExParser
    {
    public:
        //Constructor
        inline RegExParser(const StdXX::String& string) : string(string), state(string.begin())
        {
        }

        //Methods
        StdXX::UniquePointer<RegExNode> Parse();

    private:
        //Variables
        StdXX::ConstStringIterator state;
        const StdXX::String& string;

        //Methods
        StdXX::UniquePointer<RegExNode> ParseCharacterClass();
        StdXX::UniquePointer<RegExNode> ParseExpression();
        StdXX::UniquePointer<RegExNode> ParseExpressionOrSequenceUntil(uint32 codePoint);

        //Inline
        inline bool Accept(uint32 codePoint)
        {
            if(*this->state == codePoint)
            {
                this->Consume();
                return true;
            }
            return false;
        }
        inline uint32 Consume()
        {
            uint32 codePoint = *this->state;
            ++this->state;
            return codePoint;
        }
    };
}