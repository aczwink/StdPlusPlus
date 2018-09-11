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
#include "Utility.hpp"

namespace _stdxx_
{
	template <typename T>
	class OptionalStorage
	{
	public:
		//Members
		bool isInitialized;
		union
		{
			char dummy;
			T value;
		};

		//Constructors
		constexpr OptionalStorage() noexcept
			: dummy{}, isInitialized(false)
		{
		}

		//Destructor
		~OptionalStorage()
		{
			if (this->isInitialized)
				value.~T();
		}
	};

	template <typename T>
	class OptionalBase : public OptionalStorage<T>
	{
	public:
		//Methods
		template <typename U>
		void Assign(U&& rhs)
		{
			if (this->isInitialized)
				this->value = StdXX::Move(rhs);
			else
				this->Construct(StdXX::Move(rhs));
		}

		template <typename... Types>
		T& Construct(Types&&... args)
		{
			pnew(&this->value) T(StdXX::Move(args)...);
			this->isInitialized = true;
			return this->value;
		}
	};
}

namespace StdXX
{
	template <typename T>
	class Optional : private _stdxx_::OptionalBase<T>
	{
	public:
		//Constructor
		constexpr Optional() noexcept : _stdxx_::OptionalBase<T>()
		{
		}

		//Operators
		template <typename U = T>
		Optional &operator=(U&& rhs)
		{
			this->Assign(Move(rhs));
			return *this;
		}

		constexpr const T& operator*() const
		{
			return this->Value();
		}

		constexpr const T* operator->() const
		{
			return &this->Value();
		}

		//Methods
		constexpr bool HasValue() const noexcept
		{
			return this->isInitialized;
		}

		constexpr const T& Value() const
		{
			ASSERT(this->HasValue(), u8"Can't access optional that does not have a value.");
			return this->value;
		}
	};
}