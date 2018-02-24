/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "../Debug.h"
#include "../Definitions.h"

namespace _stdpp
{
	struct AutoPointerControlBlock
	{
		uint32 referenceCount;
		bool expired;
	};
}

namespace StdPlusPlus
{
	/**
	 * IMPORTANT: The concept of this class is different from std::auto_ptr!
	 * A single-owned shared pointer.
	 * When a raw pointer is assigned to an AutoPointer, then it will take ownership of the pointer and assume,
	 * that it will have exclusive ownership over the pointer.
	 * Copies can be made of the pointer, but won't take ownership over the pointer.
	 * As soon as the owner goes out of scope, it deletes the pointer, even if copies still exist.
	 * This is for instance useful, if all shared pointers should become invalid, because a library is unloaded or so.
	 *
	 *
	 * @tparam ValueType
	 */
	template<typename ValueType>
	class AutoPointer
	{
		template <typename > friend class AutoPointer; //all AutoPointers are friends
	public:
		//Constructors
		inline AutoPointer() : pointer(nullptr), controlBlock(nullptr), isOwner(false)
		{
		}

		inline AutoPointer(ValueType *pointer) : pointer(pointer), isOwner(true)
		{
			this->controlBlock = new _stdpp::AutoPointerControlBlock;
			this->controlBlock->referenceCount = 1;
			this->controlBlock->expired = false;
		}

		inline AutoPointer(const AutoPointer &rhs) : pointer(rhs.pointer), controlBlock(rhs.controlBlock), isOwner(false) //Copy ctor
		{
			if(this->controlBlock)
				this->controlBlock->referenceCount++;
		}

		inline AutoPointer(AutoPointer &&rhs) : pointer(rhs.pointer), controlBlock(rhs.controlBlock), isOwner(rhs.isOwner) //Move ctor
		{
			rhs.pointer = nullptr;
			rhs.controlBlock = nullptr;
		}

		//Destructor
		inline ~AutoPointer()
		{
			this->Release();
		}

		//Operators
		inline AutoPointer &operator=(const AutoPointer &rhs) //Copy assign
		{
			this->Release();
			this->pointer = rhs.pointer;
			this->controlBlock = rhs.controlBlock;
			if(this->controlBlock)
				this->controlBlock->referenceCount++;
			this->isOwner = false;
			return *this;
		}

		inline AutoPointer &operator=(AutoPointer &&rhs) //Move assign
		{
			this->Release();
			this->pointer = rhs.pointer;
			rhs.pointer = nullptr;
			this->controlBlock = rhs.controlBlock;
			rhs.controlBlock = nullptr;
			this->isOwner = rhs.isOwner;

			return *this;
		}

		inline const ValueType &operator*() const
		{
			ASSERT(this->pointer, u8"Can't dereference nullptr");
			return *this->pointer;
		}

		inline ValueType *operator->()
		{
			ASSERT(this->pointer, u8"Can't access nullptr");
			ASSERT(!this->controlBlock->expired, u8"Can't access expired pointer");
			return this->pointer;
		}

		inline const ValueType *operator->() const
		{
			ASSERT(this->pointer, u8"Can't access nullptr");
			ASSERT(!this->controlBlock->expired, u8"Can't access expired pointer");
			return this->pointer;
		}

		//Inline
		template <typename CastToType>
		inline AutoPointer<CastToType> DynamicCast() const
		{
			AutoPointer<CastToType> result;
			result.pointer = dynamic_cast<CastToType *>(this->pointer);
			result.controlBlock = this->controlBlock;
			result.controlBlock->referenceCount++;

			return result;
		}

		template <typename CastToType>
		inline AutoPointer<CastToType> StaticCast() const
		{
			AutoPointer<CastToType> result;
			result.pointer = static_cast<CastToType *>(this->pointer);
			result.controlBlock = this->controlBlock;
			result.controlBlock->referenceCount++;

			return result;
		}

	private:
		//Members
		ValueType *pointer;
		_stdpp::AutoPointerControlBlock *controlBlock;
		bool isOwner;

		//Inline
		inline void Release()
		{
			if(this->controlBlock)
			{
				if(this->isOwner)
				{
					this->controlBlock->expired = true;
					delete this->pointer;
					this->pointer = nullptr;
				}
				if(--this->controlBlock->referenceCount == 0)
				{
					delete this->controlBlock;
					this->controlBlock = nullptr;
				}
			}
		}
	};
}