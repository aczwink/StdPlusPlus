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
#pragma once

namespace _stdxx_
{
    //Forward declarations
    class InputNode;
    class OrNode;
    class SequenceNode;
    class UnaryNode;

    class RegExNodeVisitor
    {
    public:
        //Overridable
        virtual void Visit(const InputNode& inputNode) = 0;
        virtual void Visit(const OrNode& orNode) = 0;
        virtual void Visit(const SequenceNode& concatNode) = 0;
        virtual void Visit(const UnaryNode& unaryNode) = 0;
    };

    class RegExNode
    {
    public:
        //Destructor
        virtual ~RegExNode() = default;

        //Abstract
        virtual void Visit(RegExNodeVisitor& visitor) const = 0;
    };

    class InputNode : public RegExNode
    {
    public:
        //Variables
        StdXX::FormalLanguages::CharacterClass characterClass;

        //Constructors
        inline InputNode(uint32 codePoint)
        {
            this->characterClass.Insert(codePoint);
        }

        inline InputNode(uint32 first, uint32 last)
        {
            this->characterClass.Insert(first, last);
        }

        inline InputNode(StdXX::FormalLanguages::CharacterClass&& characterClass) : characterClass(Move(characterClass))
        {
        }

        //Methods
        void Visit(RegExNodeVisitor &visitor) const override
        {
            visitor.Visit(*this);
        }
    };

    class OrNode : public RegExNode
    {
    public:
        //State
        StdXX::UniquePointer<RegExNode> lhs;
        StdXX::UniquePointer<RegExNode> rhs;
        //Constructor
        inline OrNode(StdXX::UniquePointer<RegExNode>&& lhs, StdXX::UniquePointer<RegExNode>&& rhs) : lhs(Move(lhs)), rhs(Move(rhs))
        {
        }

        void Visit(RegExNodeVisitor &visitor) const override
        {
            visitor.Visit(*this);
        }
    };

    class SequenceNode : public RegExNode
    {
    public:
        //State
        StdXX::DynamicArray<StdXX::UniquePointer<RegExNode>> nodes;

        //Methods
        void Visit(RegExNodeVisitor &visitor) const override
        {
            visitor.Visit(*this);
        }
    };

    enum class UnaryOperation
    {
        Plus,
        QuestionMark,
        Star,
    };

    class UnaryNode : public RegExNode
    {
    public:
        //State
        UnaryOperation operation;
        StdXX::UniquePointer<RegExNode> subExpr;

        //Constructor
        inline UnaryNode(UnaryOperation operation, StdXX::UniquePointer<RegExNode>&& subExpr) : operation(operation), subExpr(Move(subExpr))
        {
        }

        void Visit(RegExNodeVisitor &visitor) const override
        {
            visitor.Visit(*this);
        }
    };
}