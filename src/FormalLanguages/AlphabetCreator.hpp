/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FormalLanguages/Alphabet.hpp>
#include "RegExAST.hpp"

namespace _stdxx_
{
    class AlphabetCreator : private RegExNodeVisitor
    {
    public:
        //Methods
        Alphabet Create(const RegExNode& node)
        {
            node.Visit(*this);
            return StdXX::Move(this->alphabet);
        }

    private:
        //State
        Alphabet alphabet;

        //Methods
        void Visit(const InputNode &inputNode) override
        {
            this->alphabet.Add(inputNode.characterClass);
        }

        void Visit(const OrNode &orNode) override
        {
            orNode.lhs->Visit(*this);
            orNode.rhs->Visit(*this);
        }

        void Visit(const SequenceNode& sequenceNode) override
        {
            for(const auto& node : sequenceNode.nodes)
                node->Visit(*this);
        }

        void Visit(const UnaryNode &unaryNode) override
        {
            unaryNode.subExpr->Visit(*this);
        }
    };
}