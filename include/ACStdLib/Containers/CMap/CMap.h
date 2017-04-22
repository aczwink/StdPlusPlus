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
#include "../../Containers/AContainer.h"
#include "CMapNode.h"
#include "CMapIterator.h"
#include "../../Debug.h"

namespace ACStdLib
{
    //Implemented as Red-Black Tree
    template<typename KeyType, typename ValueType>
    class CMap : public AContainer
    {
        typedef CMap<KeyType, ValueType> Map;
        typedef CMapNode<KeyType, ValueType> Node;
        typedef CConstMapIterator<KeyType, ValueType> ConstIterator;
    private:
        //Members
        Node *pRoot;

        //Methods
        Node *FindNode(const KeyType &refKey) const
        {
            Node *pNode;

            pNode = this->pRoot;
            while(pNode)
            {
                if(refKey < pNode->keyValuePair.key)
                    pNode = (Node *)pNode->pLeft;
                else if(refKey > pNode->keyValuePair.key)
                    pNode = (Node *)pNode->pRight;
                else
                    break;
            }

            return pNode;
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
        void Recolorize(Node *pNode)
        {
            Node *pParent, *pGrandParent, *pUncle;

            while((pParent = (Node *)pNode->pParent) && pParent->IsRed())
            {
                pGrandParent = (Node *)pParent->pParent;

                if(pParent == pGrandParent->pLeft)
                {
                    pUncle = (Node *)pGrandParent->pRight;

                    if(pUncle && pUncle->IsRed())
                    {
                        pParent->isBlack = true;
                        pUncle->isBlack = true;
                        pGrandParent->isBlack = false;
                        pNode = pGrandParent;
                    }
                    else
                    {
                        if(pNode == pParent->pRight)
                        {
                            pParent->RotateLeft();
                            pNode = pParent;
                            pParent = (Node *)pNode->pParent;
                        }

                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        pGrandParent->RotateRight();
                    }
                }
                else
                {
                    pUncle = (Node *)pGrandParent->pLeft;

                    if(pUncle && pUncle->IsRed())
                    {
                        pParent->isBlack = true;
                        pUncle->isBlack = true;
                        pGrandParent->isBlack = false;
                        pNode = pGrandParent;
                    }
                    else
                    {
                        if(pNode == pParent->pLeft)
                        {
                            pParent->RotateRight();
                            pNode = pParent;
                            pParent = (Node *)pNode->pParent;
                        }

                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        pGrandParent->RotateLeft();
                    }
                }
            }
        }

    public:
        //Constructors
        CMap()
        {
            this->pRoot = nullptr;
        }

        CMap(const Map &refOther) //copy ctor
        {
            *this = refOther;
        }

        CMap(Map &&refOther) //move ctor
        {
            this->pRoot = nullptr;
            *this = (Map &&)refOther; //forward
        }

        //Destructor
        ~CMap()
        {
            this->Release();
        }

        //Operators
        ValueType &operator[](const KeyType &refKey)
        {
            Node *pNode;

            pNode = this->FindNode(refKey);
            if(!pNode)
            {
                this->Insert(refKey, ValueType());
                pNode = this->FindNode(refKey);
            }

            return pNode->keyValuePair.value;
        }

        const ValueType &operator[](const KeyType &refKey) const
        {
            Node *pNode;

            pNode = this->FindNode(refKey);
            ASSERT_MSG(pNode, "A non-existant key of const map was accessed.");

            return pNode->keyValuePair.value;
        }

        Map &operator=(const Map &refMap) //copy assign
        {
            this->Release();

            for(const auto &refKV : refMap)
            {
                this->Insert(refKV.key, refKV.value);
            }

            return *this;
        }

        Map &operator=(Map &&refMap) //move assign
        {
            this->Release();

            this->pRoot = refMap.pRoot;
            refMap.pRoot = nullptr;

            return *this;
        }

        //Methods
        void Insert(const KeyType &refKey, const ValueType &refValue)
        {
            Node *pNode, *pInsertedNode;

            pInsertedNode = NULL;

            //insert root
            if(this->pRoot == NULL)
            {
                this->pRoot = new Node(refKey, refValue, true, NULL);
                this->nElements = 1;
                return;
            }

            //binary-search tree insertion
            pNode = this->pRoot;
            while(pNode)
            {
                if(pNode->keyValuePair.key < refKey) //insert in right subtree
                {
                    if(pNode->pRight) //continue in right subtree
                    {
                        pNode = (Node *)pNode->pRight;
                    }
                    else
                    {
                        //insert (key,value) as leaf
                        pNode->pRight = new Node(refKey, refValue, false, pNode);
                        pInsertedNode = (Node *)pNode->pRight;
                        break;
                    }
                }
                else if(pNode->keyValuePair.key > refKey) //insert in left subtree
                {
                    if(pNode->pLeft) //continue in left subtree
                    {
                        pNode = (Node *)pNode->pLeft;
                    }
                    else
                    {
                        //insert (key,value) as leaf
                        pNode->pLeft = new Node(refKey, refValue, false, pNode);
                        pInsertedNode = (Node *)pNode->pLeft;
                        break;
                    }
                }
                else //overwrite old node and return it, no need to rebalance
                {
                    pNode->keyValuePair.value = refValue;
                    return;
                }
            }

            this->nElements++;

            //recolor the tree
            this->Recolorize(pInsertedNode);

            if(this->pRoot->pParent)
                this->pRoot = (Node *)this->pRoot->pParent;
            this->pRoot->isBlack = true;
        }

        void Release()
        {
            if(this->pRoot)
                delete this->pRoot;
            this->nElements = 0;
            this->pRoot = nullptr;
        }

        //Inline
        inline bool Contains(const KeyType &refKey) const
        {
            return this->FindNode(refKey) != nullptr;
        }

        inline ConstIterator Find(const KeyType &refKey) const
        {
            return ConstIterator(*this, this->FindNode(refKey));
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