/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include "RegExLexer.hpp"
#include "RegExParser.hpp"
//Namespaces
using namespace StdXX;

//Constructor
RegEx::RegEx(const String &regEx)
{
    _stdxx_::RegExLexer lexer(regEx);
    _stdxx_::RegExParser parser(lexer);

    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    //_stdxx_::NFA<_stdxx_::CharacterClass>* nfa = parser.Execute();
    //_stdxx_::DFA<_stdxx_::CharacterClass>* dfa = nfa->ToDFA();
}
