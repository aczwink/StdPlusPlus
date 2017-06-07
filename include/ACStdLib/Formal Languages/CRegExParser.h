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
#include "../Definitions.h"
#include "../Containers/DynamicArray/DynamicArray.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Containers/LinkedList/LinkedListConstIterator.hpp"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "CCharSet.h"
#include "CNFA.h"

namespace ACStdLib
{
    class ACSTDLIB_API CRegExParser
    {
        enum ETokenType
        {
            TOKEN_END,
            TOKEN_CHAR,
            TOKEN_ANYCHAR,
            TOKEN_CHARCLASS_OPEN,
            TOKEN_CHARCLASS_CLOSE,
            TOKEN_CHARCLASS_RANGE,
            TOKEN_CHARCLASS_NEGATE,
            TOKEN_0_TO_N,
            TOKEN_1_TO_N,
            TOKEN_0_OR_1,
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
        LinkedList<CCharSet *> disjointCharSets;
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
        void Push(const CCharSet &refCharSet);
        void Star();
        void UpdateLookahead(ConstUTF8StringIterator &refIt);
        void ZeroOrOne();
    public:
        //Constructor
        CRegExParser();
        //Destructor
        ~CRegExParser();
        //Methods
        void BuildDisjointCharSets();
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
}