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
#include "ACStdLib/Containers/Container.hpp"
#include "FiniteSetNode.hpp"
#include "FiniteSetIterator.hpp"
//Definitions
#define BALANCE(pNode) ((Node *)pNode)->balance

namespace ACStdLib
{
    //Implementation as AVL tree
    template<typename DataType>
    class FiniteSet : public Container
    {
        typedef FiniteSet<DataType> Set;
        typedef FiniteSetNode<DataType> Node;
        typedef CConstFiniteSetIterator<DataType> ConstIterator;
    private:
        //Members
        Node *pRoot;

        //Methods
        Node *FindNode(const DataType &refValue) const
        {
            Node *pNode;

            pNode = this->pRoot;
            while(pNode)
            {
                if(refValue < pNode->value)
                    pNode = (Node *)pNode->pLeft;
                else if(refValue > pNode->value)
                    pNode = (Node *)pNode->pRight;
                else
                    break;
            }

            return pNode;
        }
    public:
        //Constructors
        FiniteSet()
        {
            this->pRoot = nullptr;
        }

        FiniteSet(const Set &refSet)
        {
            this->pRoot = nullptr;

            *this = refSet;
        }

        FiniteSet(Set &&refSet)
        {
            this->pRoot = nullptr;

            *this = (Set &&)refSet;
        }

        //Destructor
        ~FiniteSet()
        {
            this->Release();
        }

        //Operators
        Set &operator=(const Set &refSet)
        {
            this->Release();

            for(const DataType &refValue : refSet)
            {
                this->Insert(refValue);
            }

            return *this;
        }

        Set &operator=(Set &&refSet)
        {
            this->Release();

            this->pRoot = refSet.pRoot;
            refSet.pRoot = nullptr;

            return *this;
        }

        inline bool operator==(const FiniteSet<DataType> &refRight) const
        {
            return this->GetNumberOfElements() == refRight.GetNumberOfElements() && this->IsSubsetOf(refRight);
        }

        //Methods
        bool Contains(const DataType &refElement) const
        {
            return this->FindNode(refElement) != nullptr;
        }

        /*
         * The implementation of this method is a modified version of a piece of code of the following work,
         * which was released under GNU Lesser General Public License
         * as published by the Free Software Foundation version 2.1:
         *
         * avltree - Implements an AVL tree with parent pointers.
         *
         * Copyright (C) 2010-2014 Franck Bui-Huu <fbuihuu@gmail.com>
         *
         * https://github.com/fbuihuu/libtree
         */
        void Insert(const DataType &refValue)
        {
            Node *pNode, *pInsertedNode, *pParent, *pUnbalanced;

            if(this->pRoot == NULL)
            {
                this->pRoot = new Node(refValue, NULL);
                this->nElements = 1;
                return;
            }

            pNode = this->pRoot;
            pInsertedNode = NULL;
            pUnbalanced = pNode;

            while(pNode)
            {
                if(pNode->balance != 0)
                    pUnbalanced = pNode;

                if(pNode->value < refValue) //insert in right subtree
                {
                    if(pNode->pRight) //continue in right subtree
                    {
                        pNode = (Node *)pNode->pRight;
                    }
                    else
                    {
                        //insert (key,value) as leaf
                        pNode->pRight = new Node(refValue, pNode);
                        pInsertedNode = (Node *)pNode->pRight;
                        break;
                    }
                }
                else if(pNode->value > refValue) //insert in left subtree
                {
                    if(pNode->pLeft) //continue in left subtree
                    {
                        pNode = (Node *)pNode->pLeft;
                    }
                    else
                    {
                        //insert (key,value) as leaf
                        pNode->pLeft = new Node(refValue, pNode);
                        pInsertedNode = (Node *)pNode->pLeft;
                        break;
                    }
                }
                else //overwrite old node and return it, no need to rebalance
                {
                    pNode->value = refValue;
                    return;
                }
            }

            this->nElements++;

            //Update Balances
            pNode = pInsertedNode;
            pParent = (Node *)pNode->pParent;
            while(true)
            {
                if(pParent->pLeft == pNode)
                    pParent->balance--;
                else
                    pParent->balance++;

                if(pParent == pUnbalanced)
                    break;

                pNode = pParent;
                pParent = (Node *)pParent->pParent;
            }

            if(pUnbalanced->balance == -2)
            {
                if(BALANCE(pUnbalanced->pLeft) == -1)
                {
                    pUnbalanced->balance = 0;
                    BALANCE(pUnbalanced->pLeft) = 0;
                }
                else
                {
                    if(BALANCE(pUnbalanced->pLeft->pRight) == 1)
                    {
                        pUnbalanced->balance = 0;
                        BALANCE(pUnbalanced->pLeft) = -1;
                    }
                    else if(BALANCE(pUnbalanced->pLeft->pRight) == 0)
                    {
                        pUnbalanced->balance = 0;
                        BALANCE(pUnbalanced->pLeft) = 0;
                    }
                    else if(BALANCE(pUnbalanced->pLeft->pRight) == -1)
                    {
                        pUnbalanced->balance = 1;
                        BALANCE(pUnbalanced->pLeft) = 0;
                    }
                    BALANCE(pUnbalanced->pLeft->pRight) = 0;

                    pUnbalanced->pLeft->RotateLeft();
                }
                pUnbalanced->RotateRight();
            }
            else if(pUnbalanced->balance == 2)
            {
                if(BALANCE(pUnbalanced->pRight) == 1)
                {
                    pUnbalanced->balance = 0;
                    BALANCE(pUnbalanced->pRight) = 0;
                }
                else
                {
                    if(BALANCE(pUnbalanced->pRight->pLeft) == 1)
                    {
                        pUnbalanced->balance = -1;
                        BALANCE(pUnbalanced->pRight) = 0;
                    }
                    else if(BALANCE(pUnbalanced->pRight->pLeft) == 0)
                    {
                        pUnbalanced->balance = 0;
                        BALANCE(pUnbalanced->pRight) = 0;
                    }
                    else if(BALANCE(pUnbalanced->pRight->pLeft) == -1)
                    {
                        pUnbalanced->balance = 0;
                        BALANCE(pUnbalanced->pRight) = 1;
                    }
                    BALANCE(pUnbalanced->pRight->pLeft) = 0;
                    pUnbalanced->pRight->RotateRight();
                }
                pUnbalanced->RotateLeft();
            }

            if(this->pRoot->pParent)
                this->pRoot = (Node *)this->pRoot->pParent;
        }

        bool IsSubsetOf(const Set &refSet) const
        {
            for(const DataType &refData : *this)
            {
                if(!refSet.Contains(refData))
                    return false;
            }

            return true;
        }

        void Release()
        {
            if(this->pRoot)
                delete this->pRoot;
            this->nElements = 0;
            this->pRoot = nullptr;
        }

        /*
         * The implementation of this method is a modified version of a piece of code of the following work,
         * which was released under GNU Lesser General Public License
         * as published by the Free Software Foundation version 2.1:
         *
         * avltree - Implements an AVL tree with parent pointers.
         *
         * Copyright (C) 2010-2014 Franck Bui-Huu <fbuihuu@gmail.com>
         *
         * https://github.com/fbuihuu/libtree
         */
        void Remove(const DataType &value)
        {
            Node *pNode, *pNodeToDelete;

            pNode = this->pRoot;
            while(pNode)
            {
                if(value < pNode->value)
                    pNode = pNode->pLeft;
                else if(value > pNode->value)
                    pNode = pNode->pRight;
                else
                    break;
            }
            if(pNode != NULL)
            {
                Node *pParent, *pLeft, *pRight, *pNext;
                bool isLeftChild;

                pNodeToDelete = pNode;
                pParent = pNode->pParent;
                pLeft = pNode->pLeft;
                pRight = pNode->pRight;

                if(!pLeft)
                    pNext = pRight;
                else if(!pRight)
                    pNext = pLeft;
                else
                    pNext = pRight->GetFirst();

                if(pParent)
                {
                    isLeftChild = pParent->pLeft == pNode;
                    if(isLeftChild)
                        pParent->pLeft = pNext;
                    else
                        pParent->pRight = pNext;
                }
                else
                {
                    this->pRoot = pNext;
                }

                if(pLeft && pRight) //Node has 2 subtrees
                {
                    pNext->balance = pNode->balance;
                    pNext->pLeft = pLeft;
                    pLeft->pParent = pNext;

                    if(pNext == pRight)
                    {
                        pNext->pParent = pParent;
                        pParent = pNext;
                        pNode = pParent->pRight;
                        isLeftChild = false;
                    }
                    else
                    {
                        pParent = pNext->pParent;
                        pNext->pParent = pNode->pParent;
                        pNode = pNext->pRight;
                        pParent->pLeft = pNode;
                        isLeftChild = true;
                        pNext->pRight = pRight;
                        pRight->pParent = pNext;
                    }
                }
                else
                {
                    pNode = pNext;
                }

                if(pNode)
                    pNode->pParent = pParent;

                while(pParent)
                {
                    pNode = pParent;
                    pParent = pParent->pParent;

                    if(isLeftChild)
                    {
                        isLeftChild = pParent && pParent->pLeft == pNode;

                        pNode->balance++;
                        if(pNode->balance == 0)
                            continue;
                        if(pNode->balance == 1)
                        {
                            if(this->pRoot->pParent)
                                this->pRoot = this->pRoot->pParent;
                            goto end;
                        }
                        pRight = pNode->pRight;

                        if(pRight->balance == 0)
                        {
                            pNode->balance = 1;
                            pRight->balance = -1;
                            pNode->RotateLeft();
                            if(this->pRoot->pParent)
                                this->pRoot = this->pRoot->pParent;
                            goto end;
                        }
                        else if(pRight->balance == 1)
                        {
                            pNode->balance = 0;
                            pRight->balance = 0;
                        }
                        else if(pRight->balance == -1)
                        {
                            if(pRight->pLeft->balance == 1)
                            {
                                pNode->balance = -1;
                                pRight->balance = 0;
                            }
                            else if(pRight->pLeft->balance == 0)
                            {
                                pNode->balance = 0;
                                pRight->balance = 0;
                            }
                            else if(pRight->pLeft->balance == -1)
                            {
                                pNode->balance = 0;
                                pRight->balance = 1;
                            }
                            pRight->pLeft->balance = 0;

                            pRight->RotateRight();
                        }
                        pNode->RotateLeft();
                    }
                    else
                    {
                        isLeftChild = pParent && pParent->pLeft == pNode;

                        pNode->balance--;
                        if(pNode->balance == 0)
                            continue;
                        if(pNode->balance == -1)
                        {
                            if(this->pRoot->pParent)
                                this->pRoot = this->pRoot->pParent;
                            goto end;
                        }
                        pLeft = pNode->pLeft;

                        if(pLeft->balance == 0)
                        {
                            pNode->balance = -1;
                            pLeft->balance = 1;
                            pNode->RotateRight();
                            if(this->pRoot->pParent)
                                this->pRoot = this->pRoot->pParent;
                            goto end;
                        }
                        else if(pLeft->balance == -1)
                        {
                            pNode->balance = 0;
                            pLeft->balance = 0;
                        }
                        else if(pLeft->balance == 1)
                        {
                            if(pLeft->pRight->balance == 1)
                            {
                                pNode->balance = 0;
                                pLeft->balance = -1;
                            }
                            else if(pLeft->pRight->balance == 0)
                            {
                                pNode->balance = 0;
                                pLeft->balance = 0;
                            }
                            else if(pLeft->pRight->balance == -1)
                            {
                                pNode->balance = 1;
                                pLeft->balance = 0;
                            }
                            pLeft->pRight->balance = 0;

                            pLeft->RotateLeft();
                        }
                        pNode->RotateRight();
                    }
                }
                end:;
                this->nElements--;
                pNodeToDelete->pLeft = NULL;
                pNodeToDelete->pRight = NULL;
                delete pNodeToDelete;
                if(this->pRoot && this->pRoot->pParent)
                    this->pRoot = this->pRoot->pParent;
            }
        }

        void Union(const Set &refSet)
        {
            for(const DataType &refData : refSet)
                this->Insert(refData);
        }

        //For range-based loop
        ConstIterator begin() const
        {
            if(!this->pRoot)
                return ConstIterator(*this, nullptr);

            return ConstIterator(*this, (Node *)this->pRoot->GetFirst());
        }

        ConstIterator end() const
        {
            return ConstIterator(*this, nullptr);
        }
    };
}

#undef BALANCE