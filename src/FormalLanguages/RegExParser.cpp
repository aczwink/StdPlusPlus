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
#include "RegExParser.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
UniquePointer<RegExNode> RegExParser::Parse()
{
    auto sequence = new SequenceNode;
    while(this->state != this->string.end())
    {
        auto expr = this->ParseExpression();
        sequence->nodes.Push(Move(expr));
    }

    return sequence;
}

//Private methods
UniquePointer<RegExNode> RegExParser::ParseCharacterClass()
{
    bool negate = this->Accept(u8'^');

    StdXX::FormalLanguages::CharacterClass characterClass;
    while(!this->Accept(u8']'))
    {
        uint32 from = this->Consume();
        uint32 to;

        if(this->Accept(u8'-'))
            to = this->Consume();
        else
            to = from;

        characterClass.Insert(from, to);
    }

    if(negate)
        return new InputNode(characterClass.Negated());

    return new InputNode(Move(characterClass));
}

UniquePointer<RegExNode> RegExParser::ParseExpression()
{
    UniquePointer<RegExNode> node;

    if(this->Accept(u8'.'))
        node = new InputNode(0, UNICODE_MAX);
    else if(this->Accept(u8'('))
        node = this->ParseExpressionOrSequenceUntil(u8')');
    else if(this->Accept(u8'['))
        node = this->ParseCharacterClass();
    else
        node = new InputNode(this->Consume());

    //left recursion rules
    switch(*this->state)
    {
        case u8'*':
            this->Consume();
            node = new UnaryNode(UnaryOperation::Star, Move(node));
            break;
        case u8'+':
            this->Consume();
            node = new UnaryNode(UnaryOperation::Plus, Move(node));
            break;
        case u8'?':
            this->Consume();
            node = new UnaryNode(UnaryOperation::QuestionMark, Move(node));
            break;
        case u8'|':
            this->Consume();
            node = new OrNode(Move(node), this->ParseExpression());
            break;
    }

    return node;
}

UniquePointer<RegExNode> RegExParser::ParseExpressionOrSequenceUntil(uint32 codePoint)
{
    auto sequence = new SequenceNode;
    while(!this->Accept(codePoint))
    {
        auto expr = this->ParseExpression();
        sequence->nodes.Push(Move(expr));
    }

    if(sequence->nodes.GetNumberOfElements() == 1)
        return Move(sequence->nodes[0]);

    return sequence;
}
