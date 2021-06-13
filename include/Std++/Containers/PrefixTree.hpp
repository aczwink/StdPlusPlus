/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>

namespace _stdxx_
{
	template<typename CharType, typename ValueType>
	struct PrefixTreeNode
	{
		StdXX::BinaryTreeMap<CharType, PrefixTreeNode*> children;
		ValueType value;

		inline ~PrefixTreeNode()
		{
			for(const auto& kv : this->children)
				delete kv.value;
		}
	};
}

namespace StdXX
{
	template<typename CharType, typename StringType, typename ValueType>
	class PrefixTree
	{
		typedef _stdxx_::PrefixTreeNode<CharType, ValueType> Node;
	public:
		//Constructor
		inline PrefixTree() : root(nullptr)
		{
		}

		//Destructor
		inline ~PrefixTree()
		{
			delete this->root;
		}

		//Operators
		ValueType& operator[](const StringType& string)
		{
			if(!this->Contains(string))
				this->Insert(string, {});
			return this->Get(string);
		}

		//Methods
		void Insert(const StringType& string, const ValueType& value)
		{
			if(this->root == nullptr)
				this->root = new Node;

			auto it = string.begin();
			Node* node = this->root;

			while(it != string.end())
			{
				if(!node->children.Contains(*it))
					node->children[*it] = new Node;
				node = node->children[*it];
				++it;
			}
			node->value = value;
		}

		void Remove(const StringType& string)
		{
			auto it = string.begin();
			Node* node = this->root;
			Node* prev = nullptr;

			CharType lastChar;
			while(it != string.end())
			{
				lastChar = *it;
				prev = node;
				if(!node->children.Contains(lastChar))
					return;
				node = node->children[lastChar];
				++it;
			}

			delete node;
			prev->children.Remove(lastChar);
		}

		//Inline
		inline bool Contains(const StringType& string)
		{
			return this->FindNode(string) != nullptr;
		}

		inline ValueType& Get(const StringType& string)
		{
			return this->FindNode(string)->value;
		}

	private:
		//Members
		Node* root;

		//Methods
		Node* FindNode(const StringType& string)
		{
			auto it = string.begin();
			Node* node = this->root;
			while(node and (it != string.end()))
			{
				if(!node->children.Contains(*it))
					return nullptr;
				node = node->children[*it];
				++it;
			}

			return node;
		}
	};
}