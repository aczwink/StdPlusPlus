/*
 * Copyright (c) 2021-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Function.hpp>
#include "PipeEnumerator.hpp"

namespace _stdxx_
{
	template<typename InputType, typename OutputType, bool isDefaultConstructible>
	class MapEnumerator;

	template<typename InputType, typename OutputType>
	class MapEnumerator<InputType, OutputType, true> : public StdXX::Enumerator<OutputType>
	{
	public:
		//Constructor
		inline MapEnumerator(StdXX::UniquePointer<StdXX::Enumerator<InputType>>&& input, const StdXX::Function<OutputType(InputType)>& mappingFunction)
				: input(StdXX::Move(input)), mappingFunction(mappingFunction)
		{
		}

		//Methods
		const OutputType &GetCurrent() const override
		{
			return this->current;
		}

		bool MoveForward() override
		{
			if(this->input->MoveForward())
			{
				this->current = this->mappingFunction(this->input->GetCurrent());
				return true;
			}
			return false;
		}

	private:
		//Members
		StdXX::UniquePointer<StdXX::Enumerator<InputType>> input;
		const StdXX::Function<OutputType(InputType)>& mappingFunction;
		OutputType current;
	};

	template<typename InputType, typename OutputType>
	class MapEnumerator<InputType, OutputType, false> : public StdXX::Enumerator<OutputType>
	{
	public:
		//Constructor
		inline MapEnumerator(StdXX::UniquePointer<StdXX::Enumerator<InputType>>&& input, const StdXX::Function<OutputType(InputType)>& mappingFunction)
			: input(StdXX::Move(input)), mappingFunction(mappingFunction)
		{
		}

		//Methods
		const OutputType &GetCurrent() const override
		{
			return *this->current;
		}

		bool MoveForward() override
		{
			if(this->input->MoveForward())
			{
				this->current.template Emplace(this->mappingFunction(this->input->GetCurrent()));
				return true;
			}
			return false;
		}

	private:
		//Members
		StdXX::UniquePointer<StdXX::Enumerator<InputType>> input;
		const StdXX::Function<OutputType(InputType)>& mappingFunction;
		StdXX::Optional<OutputType> current;
	};

	template<typename InputType, typename OutputType>
	class MapBuilder : public StdXX::EnumeratorBuilder<InputType, OutputType>
	{
	public:
		typedef OutputType outputType;

		//Constructor
		inline MapBuilder(StdXX::Function<OutputType(InputType)>&& mappingFunction) : mappingFunction(StdXX::Move(mappingFunction))
		{
		}

		StdXX::Enumerator<OutputType> *CreateEnumerator(StdXX::Enumerator<InputType> *input) const override
		{
			return new MapEnumerator<InputType, OutputType, StdXX::Type::IsDefaultConstructible<OutputType>::value>(input, this->mappingFunction);
		}

	private:
		//Members
		StdXX::Function<OutputType(InputType)> mappingFunction;
	};
}

namespace StdXX
{
	template<typename InputType, typename OutputType>
	_stdxx_::MapBuilder<InputType, OutputType> Map(Function<OutputType(InputType)>&& mappingFunction)
	{
		return _stdxx_::MapBuilder<InputType, OutputType>(Move(mappingFunction));
	}
}