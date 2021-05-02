/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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

#include <Std++/Containers/Map/Map.hpp>
#include "Option.hpp"
#include "Argument.hpp"

namespace StdXX::CommandLine
{
	class MatchResult
	{
	public:
		enum class ArgumentParseError
		{
			MissingPositionalArgument,
			WrongArgumentFormat
		};

		enum class OptionParseError
		{
		};

	public:
		//Constructor
		inline MatchResult(const FixedArray<String> &args) : args(args), index(0)
		{
		}

		//Properties
		inline const DynamicArray<Tuple<ArgumentParseError, const Argument*>>& ArgumentParseErrors() const
		{
			return this->argParsingErrors;
		}

		inline const DynamicArray<String>& ExtraParameters() const
		{
			return this->extraParameters;
		}

		inline const String& Next() const
		{
			return this->args[this->index];
		}

		inline const DynamicArray<Tuple<OptionParseError, const Option*>>& OptionParseErrors() const
		{
			return this->optParsingErrors;
		}

		//Inline
		inline void Activate(const Argument* argument, const String& value = {})
		{
			this->activatedArgs[argument] = value;
		}

		inline void Activate(const Option* option, const String& value = {})
		{
			this->activatedOptions[option] = value;
		}

		inline void AddArgumentError(ArgumentParseError argumentParseError, const Argument* argument)
		{
			this->argParsingErrors.Push({argumentParseError, argument});
		}

		inline void Advance()
		{
			this->index++;
		}

		inline String ArgumentValue(const Argument& argument) const
		{
			return this->activatedArgs[&argument];
		}

		inline void Finished()
		{
			if(!this->IsAtEnd())
			{
				for(uint32 i = this->index; i < this->args.GetNumberOfElements(); i++)
					this->extraParameters.Push(this->args[i]);
			}
		}

		inline bool IsActivated(const Argument& argument) const
		{
			return this->activatedArgs.Contains(&argument);
		}

		inline bool IsActivated(const Option& option) const
		{
			return this->activatedOptions.Contains(&option);
		}

		inline bool IsAtEnd() const
		{
			return this->index >= this->args.GetNumberOfElements();
		}

		inline bool IsValid() const
		{
			return this->extraParameters.IsEmpty() and this->argParsingErrors.IsEmpty() and this->optParsingErrors.IsEmpty();
		}

		inline String Value(const Option& option) const
		{
			return this->activatedOptions[&option];
		}

	private:
		//Members
		uint32 index;
		const FixedArray<String> &args;
		Map<const Argument*, String> activatedArgs;
		Map<const Option*, String> activatedOptions;
		DynamicArray<String> extraParameters;
		DynamicArray<Tuple<ArgumentParseError, const Argument*>> argParsingErrors;
		DynamicArray<Tuple<OptionParseError, const Option*>> optParsingErrors;
	};
}