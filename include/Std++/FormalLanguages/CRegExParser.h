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
#include "../Definitions.h"
#include "../Containers/Array/DynamicArray.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Containers/LinkedList/LinkedListConstIterator.hpp"
/*
namespace StdXX
{
    class STDPLUSPLUS_API CRegExParser
    {
        enum ETokenType
        {
            TOKEN_CHARCLASS_OPEN,
            TOKEN_CHARCLASS_CLOSE,
            TOKEN_CHARCLASS_RANGE,
            TOKEN_CHARCLASS_NEGATE,
            TOKEN_BRACKET_OPEN,
            TOKEN_BRACKET_CLOSE,
            TOKEN_OR,
        };
    private:
        //Members
        ETokenType lookAhead;
        uint32 currentCodePoint;
        bool hasDot;
        CCharSet *pAnyCharSet;
        LinkedList<CCharSet *> orderedCharSets; //as they appear in the regex
        LinkedList<CNFA *> nfaStack;
        //Methods
        bool Accept(ETokenType token, ConstUTF8StringIterator &refIt);
        CNFA *BuildNFA();
        void Concat();
        void Except(ETokenType token, ConstUTF8StringIterator &refIt);
        CCharSet *ParseCharacterClass(ConstUTF8StringIterator &refIt);
        void ParseRegEx(ConstUTF8StringIterator &refIt, LinkedListConstIterator<CCharSet *> &refCharSetIt);
        bool Preprocess(ConstUTF8StringIterator &refIt);
        bool PreprocessCharClass(ConstUTF8StringIterator &refIt, CCharSet *pCharSet);
    public:
        //Methods
        CNFA *Parse(const UTF8String &refRegEx);
        bool Preprocess(const UTF8String &refRegEx);
        void Union();

        //Inline
        inline const LinkedList<CCharSet *> GetDisjointCharSets() const
        {
            return this->disjointCharSets;
        }

        inline CNFA *PopNFA()
        {
            return this->nfaStack.PopFront();
        }

        inline void PushNFA(CNFA *pNFA)
        {
            this->nfaStack.InsertFront(pNFA);
        }
    };
}*/