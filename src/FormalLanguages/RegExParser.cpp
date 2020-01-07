/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "RegExParser.hpp"
//Namespaces
using namespace _stdxx_;

//Public methods
void RegExParser::Execute()
{
    this->Parse();
    this->BuildDisjointCharSets();

    ASSERT(this->partStack.GetNumberOfElements() == 1, u8"REPORT THIS PLEASE!");
}

//Private methods
void RegExParser::BuildDisjointCharSets()
{
    //put all classes
    for(const Part& part : partStack)
    {
        this->disjointCharSets.Push(part.characterClass);
    }

    for(const CharacterClass& cc1 : this->disjointCharSets)
    {
        for(const CharacterClass& cc2 : this->disjointCharSets)
        {
            if(&cc1 == &cc2)
                continue;

            CharacterClass intersection = cc1.Intersect(cc2);
            if(intersection.IsEmpty())
                continue;

            NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }
    }
}

void RegExParser::CreateNFA(CharacterClass&& characterClass)
{
    NFA<CharacterClass>* nfa = new NFA<CharacterClass>;

    NFAState<CharacterClass>* stateFrom = new NFAState<CharacterClass>();
    NFAState<CharacterClass>* stateTo = new NFAState<CharacterClass>();

    stateFrom->AddTransition(Move(characterClass), stateTo);

    nfa->AddState(stateFrom);
    nfa->AddState(stateTo);

    this->nfaStack.Push(nfa);
}

void RegExParser::Parse()
{
    bool run = true;
    while(run)
    {
        Token nextToken = this->lexer.MatchNextToken();
        switch (nextToken)
        {
            case Token::Char:
                this->partStack.Push(Part(
                    Token::Char,
                    this->lexer.tokenValueCodePoint
                ));
                break;
            case Token::End:
                run = false;
                break;
            default:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }
    }
}
