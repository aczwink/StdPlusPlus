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
//Class header
#include <Std++/FormalLanguages/RegEx.hpp>
//Local
#include "RegExParser.hpp"
#include "NFACreator.hpp"
#include "AlphabetCreator.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::FormalLanguages;

struct RegExInternal
{
    Alphabet alphabet;
    DFA* dfa;
};
//Definitions
#define THIS ((RegExInternal*)this->impl)

//Constructor
RegEx::RegEx(const String &regEx)
{
    this->impl = new RegExInternal;

    _stdxx_::RegExParser parser(regEx);
    auto ast = parser.Parse();

    _stdxx_::AlphabetCreator alphabetCreator;
    THIS->alphabet = alphabetCreator.Create(*ast);

    _stdxx_::NFACreator nfaCreator(THIS->alphabet);
    auto nfa = nfaCreator.Create(*ast);

    THIS->dfa = nfa->ToDFA(THIS->alphabet);
    THIS->dfa->Reduce();
}

//Destructor
RegEx::~RegEx()
{
    delete THIS->dfa;
    delete THIS;
}

//Public methods
bool RegEx::Matches(const String &string) const
{
    auto it = string.begin();

    const DFAState* currentState = THIS->dfa->GetStartState();
    while(!currentState->IsAccepting())
    {
        uint32 symbol = THIS->alphabet.Map(*it);
        if(it != string.end())
            ++it;

        currentState = currentState->GetTransition(symbol);
        if(currentState == nullptr)
            return false;
    }

    return true;
}
