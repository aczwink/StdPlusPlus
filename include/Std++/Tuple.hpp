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
#include "Definitions.h"
#include "Type.hpp"
#include "Utility.hpp"

namespace StdXX
{
    //Forward declarations
    template<typename... ElementTypes>
    class Tuple;
}

namespace _stdxx_
{
	template<typename FirstType, typename... RestTypes>
	class TupleBase : private TupleBase<RestTypes...>
	{
	public:
		//Constructors
		inline TupleBase()
		{
		}

		inline TupleBase(const FirstType& first, const RestTypes&... rest) : TupleBase<RestTypes...>(rest...), value(first)
		{
		}

		inline TupleBase(FirstType&& first, RestTypes&&... rest) : TupleBase<RestTypes...>(StdXX::Forward<RestTypes>(rest)...), value(first)
		{
		}

		//Operators
		inline bool operator<(const TupleBase& other) const
		{
			if(value < other.value)
				return true;
			if(value > other.value)
				return false;
			return TupleBase<RestTypes...>::operator<(other);
		}
		inline bool operator<=(const TupleBase& other) const
		{
			if(value < other.value)
				return true;
			if(value > other.value)
				return false;
			return TupleBase<RestTypes...>::operator<=(other);
		}
		inline bool operator==(const TupleBase& other) const
		{
			if(value == other.value)
				return TupleBase<RestTypes...>::operator<=(other);
			return false;
		}

		//Inline
		template<uint32 index>
		inline const auto& GetValue(typename StdXX::EnableIf<(index > 0), int>::type dummy = 0) const
		{
			return TupleBase<RestTypes...>::template GetValue<index - 1>();
		}
		template<uint32 index>
		inline const typename StdXX::EnableIf<index == 0, FirstType>::type& GetValue() const
		{
			return this->value;
		}

	private:
		//Members
		FirstType value;
	};

	template <typename LastType>
	class TupleBase<LastType>
	{
	public:
		//Constructors
		inline TupleBase()
		{
		}

		inline TupleBase(const LastType& last) : value(last)
		{
		}

		inline TupleBase(LastType&& last) : value(last)
		{
		}

		//Operators
		inline bool operator<(const TupleBase& other) const
		{
			return value < other.value;
		}
		inline bool operator<=(const TupleBase& other) const
		{
			return value <= other.value;
		}
		inline bool operator==(const TupleBase& other) const
		{
			return value == other.value;
		}

		//Inline
		template<uint32 index>
		inline const typename StdXX::EnableIf<index == 0, LastType>::type& GetValue() const
		{
			return this->value;
		}

	private:
		//Members
		LastType value;
	};
}

namespace StdXX
{
	template<typename... ElementTypes>
	class Tuple : private _stdxx_::TupleBase<ElementTypes...>
	{
	public:
		//Constructors
		inline Tuple()
		{
		}

		inline Tuple(const ElementTypes&... values) : _stdxx_::TupleBase<ElementTypes...>(values...)
		{
		}

		template<typename... InitTypes>
		inline Tuple(InitTypes&&... values) : _stdxx_::TupleBase<ElementTypes...>(Forward<InitTypes>(values)...)
		{
		}

		Tuple( const Tuple& ) = default;
		Tuple( Tuple&& ) = default;

		//Operators
		Tuple& operator=( const Tuple& other ) = default;

		inline bool operator<(const Tuple& other) const
		{
			return _stdxx_::TupleBase<ElementTypes...>::operator<(other);
		}
		inline bool operator<=(const Tuple& other) const
		{
			return _stdxx_::TupleBase<ElementTypes...>::operator<=(other);
		}
		inline bool operator>(const Tuple& other) const
		{
			return other < *this;
		}
		inline bool operator==(const Tuple& other) const
		{
			return _stdxx_::TupleBase<ElementTypes...>::operator==(other);
		}

		//Inline
		template<uint32 index>
		inline const auto& Get() const
		{
			return this->template GetValue<index>();
		}
	};

	template<>
	class Tuple<> //empty tuple
	{
	};

	//Deduction guides
	template<typename... ElementTypes>
	Tuple(ElementTypes...) -> Tuple<ElementTypes...>;
}