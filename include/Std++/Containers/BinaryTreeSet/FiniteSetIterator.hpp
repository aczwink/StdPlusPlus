/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
    //Forward declarations
    template<typename ValueType>
    class BinaryTreeSetNode;
    template<typename ValueType>
    class BinaryTreeSet;

    template<typename ValueType>
    class CConstFiniteSetIterator
    {
        friend class BinaryTreeSet <ValueType >;
        typedef BinaryTreeSet<ValueType> Set;
        typedef BinaryTreeSetNode<ValueType> Node;
    private:
        //Members
        const Set &refSet;
        const Node *pCurrentNode;

        //Constructor
        CConstFiniteSetIterator(const Set &refSet, const Node *pNode) : refSet(refSet)
        {
            this->pCurrentNode = pNode;
        }
    public:

        //Operators
        inline bool operator!=(const CConstFiniteSetIterator<ValueType> &refIt) const
        {
            return (&this->refSet != &refIt.refSet) || (this->pCurrentNode != refIt.pCurrentNode);
        }

        CConstFiniteSetIterator<ValueType> &operator++() //Prefix ++
        {
            Node *pParent;

            if(this->pCurrentNode)
            {
                if(this->pCurrentNode->right)
                {
                    this->pCurrentNode = (Node *)this->pCurrentNode->right->GetFirst();
                }
                else
                {
                    while((pParent = (Node *)this->pCurrentNode->parent) && pParent->right == this->pCurrentNode)
                        this->pCurrentNode = pParent;
                    this->pCurrentNode = pParent;
                }
            }

            return *this;
        }

        CConstFiniteSetIterator<ValueType> operator--(int) //Postfix --
        {
            CConstFiniteSetIterator<ValueType> tmp(*this);
            Node *pParent;

            if(this->pCurrentNode->pLeft)
            {
                this->pCurrentNode = this->pCurrentNode->pLeft->GetFirst();
            }
            else
            {
                while((pParent = this->pCurrentNode->pParent) && pParent->pLeft == this->pCurrentNode)
                    this->pCurrentNode = pParent;
                this->pCurrentNode = pParent;
            }

            return tmp;
        }

        const ValueType& operator->()
        {
            return this->pCurrentNode->value;
        }

        const ValueType &operator*() const
        {
            return this->pCurrentNode->value;
        }
    };
}