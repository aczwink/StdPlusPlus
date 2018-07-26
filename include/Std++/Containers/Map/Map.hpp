/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "../Container.hpp"
#include "MapNode.hpp"
#include "MapIterator.hpp"
#include "Std++/Debug.hpp"

namespace StdXX
{
    //Implemented as Red-Black Tree
    template<typename KeyType, typename ValueType>
    class Map : public Container
    {
        //typedef Map<KeyType, ValueType> Map;
        typedef MapNode<KeyType, ValueType> Node;
		typedef MapIterator<KeyType, ValueType> Iterator;
        typedef ConstMapIterator<KeyType, ValueType> ConstIterator;

		friend class ConstMapIterator<KeyType, ValueType>;

    public:
        //Constructors
        Map()
        {
            this->root = nullptr;
        }

        Map(const Map &refOther) //copy ctor
        {
			this->root = nullptr;
            *this = refOther;
        }

        Map(Map &&refOther) //move ctor
        {
            this->root = nullptr;
            *this = (Map &&)refOther; //forward
        }

        //Destructor
        ~Map()
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
            ASSERT(pNode, "A non-existant key of const map was accessed.");

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

            this->root = refMap.root;
            refMap.root = nullptr;

            return *this;
        }

        //Methods
        void Insert(const KeyType &refKey, const ValueType &refValue)
        {
            Node *pNode, *pInsertedNode;

            pInsertedNode = NULL;

            //insert root
            if(this->root == NULL)
            {
                this->root = new Node(refKey, refValue, true, NULL);
                this->nElements = 1;
                return;
            }

            //binary-search tree insertion
            pNode = this->root;
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

            if(this->root->pParent)
                this->root = (Node *)this->root->pParent;
            this->root->isBlack = true;
        }

        void Release()
        {
            if(this->root)
                delete this->root;
            this->nElements = 0;
            this->root = nullptr;
        }

        //Inline
        inline bool Contains(const KeyType &refKey) const
        {
            return this->FindNode(refKey) != nullptr;
        }

        inline Iterator Find(const KeyType &key)
        {
            return Iterator(*this, this->FindNode(key));
        }

        inline ConstIterator Find(const KeyType &refKey) const
        {
            return ConstIterator(*this, this->FindNode(refKey));
        }

        //For range-based loop
        Iterator begin()
        {
            if(!this->root)
                return Iterator(*this, nullptr);

            return Iterator(*this, (Node *)this->root->GetFirst());
        }

        ConstIterator begin() const
        {
            if(!this->root)
                return ConstIterator(*this, nullptr);

            return ConstIterator(*this, (Node *)this->root->GetFirst());
        }

		Iterator end()
		{
			return Iterator(*this, nullptr);
		}

        ConstIterator end() const
        {
            return ConstIterator(*this, nullptr);
        }

    private:
        //Members
        Node *root;

        //Methods
        Node *FindNode(const KeyType &refKey) const
        {
            Node *pNode;

            pNode = this->root;
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
    };
}