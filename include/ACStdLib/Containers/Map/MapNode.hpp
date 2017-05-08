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
#include "../../Memory.h"
#include "../CKeyValuePair.h"

namespace ACStdLib
{
    //Forward declarations
    template<typename KeyType, typename ValueType>
    class Map;

    template<typename KeyType, typename ValueType>
    class MapNode
    {
        typedef MapNode<KeyType, ValueType> Node;
    public:
        //Members
        Node *pLeft;
        Node *pRight;
        Node *pParent;
        ACStdLib::CKeyValuePair<KeyType, ValueType> keyValuePair;
        bool isBlack;

        //Constructor
        MapNode(const KeyType &refKey, const ValueType &refValue, bool isBlack, Node *pParent) : keyValuePair(refKey, refValue)
        {
            this->pLeft = NULL;
            this->pRight = NULL;
            this->pParent = pParent;
            this->isBlack = isBlack;
        }

        //Destructor
        ~MapNode()
        {
            if(this->pLeft)
                delete this->pLeft;
            if(this->pRight)
                delete this->pRight;
        }

        //Methods
        Node *GetFirst() const
        {
            Node *pNode;

            pNode = (Node *)this;
            while(pNode->pLeft)
                pNode = pNode->pLeft;

            return pNode;
        }

        Node *GetLast() const
        {
            Node *pNode;

            pNode = (Node *)this;
            while(pNode->pRight)
                pNode = pNode->pRight;

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
        void RotateLeft()
        {
            Node *pNode, *pRight, *pParent;

            pNode = this;
            pRight = this->pRight;
            pParent = this->pParent;
            if(pParent)
            {
                if(pParent->pLeft == pNode)
                    pParent->pLeft = pRight;
                else
                    pParent->pRight = pRight;
            }
            pRight->pParent = pParent;
            pNode->pParent = pRight;
            pNode->pRight = pRight->pLeft;
            if(pNode->pRight)
                pNode->pRight->pParent = pNode;
            pRight->pLeft = pNode;
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
        void RotateRight()
        {
            Node *pNode, *pLeft, *pParent;

            pNode = this;
            pLeft = this->pLeft;
            pParent = this->pParent;
            if(pParent)
            {
                if(pParent->pLeft == pNode)
                    pParent->pLeft = pLeft;
                else
                    pParent->pRight = pLeft;
            }
            pLeft->pParent = pParent;
            pNode->pParent = pLeft;
            pNode->pLeft = pLeft->pRight;
            if(pNode->pLeft)
                pNode->pLeft->pParent = pNode;
            pLeft->pRight = pNode;
        }

        //Inline
        inline bool IsRed() const
        {
            return !this->isBlack;
        }
    };
}