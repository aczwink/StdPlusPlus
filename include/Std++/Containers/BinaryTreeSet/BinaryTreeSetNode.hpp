/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>

namespace StdXX
{
    //Forward declarations
    template<typename DataType>
    class BinaryTreeSet;

    template<typename DataType>
    class BinaryTreeSetNode
    {
        typedef BinaryTreeSetNode<DataType> Node;
    public:
        //Members
        Node* left;
        Node* right;
        Node* parent;
        DataType value;
        int8 balance;

        //Constructors
        BinaryTreeSetNode(const DataType& value, Node* parent) : value(value)
        {
            this->left = nullptr;
            this->right = nullptr;
            this->parent = parent;
            this->balance = 0;
        }

		BinaryTreeSetNode(DataType&& value, Node* parent) : value(Move(value))
		{
			this->left = nullptr;
			this->right = nullptr;
			this->parent = parent;
			this->balance = 0;
		}

        //Destructor
        ~BinaryTreeSetNode()
        {
            if(this->left)
                delete this->left;
            if(this->right)
                delete this->right;
        }

        //Methods
        Node *GetFirst() const
        {
            Node *pNode;

            pNode = (Node *)this;
            while(pNode->left)
                pNode = pNode->left;

            return pNode;
        }

        Node *GetLast() const
        {
            Node *pNode;

            pNode = (Node *)this;
            while(pNode->right)
                pNode = pNode->right;

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
            pRight = this->right;
            pParent = this->parent;
            if(pParent)
            {
                if(pParent->left == pNode)
                    pParent->left = pRight;
                else
                    pParent->right = pRight;
            }
            pRight->parent = pParent;
            pNode->parent = pRight;
            pNode->right = pRight->left;
            if(pNode->right)
                pNode->right->parent = pNode;
            pRight->left = pNode;
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
            pLeft = this->left;
            pParent = this->parent;
            if(pParent)
            {
                if(pParent->left == pNode)
                    pParent->left = pLeft;
                else
                    pParent->right = pLeft;
            }
            pLeft->parent = pParent;
            pNode->parent = pLeft;
            pNode->left = pLeft->right;
            if(pNode->left)
                pNode->left->parent = pNode;
            pLeft->right = pNode;
        }
    };
}