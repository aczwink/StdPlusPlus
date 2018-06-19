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
#include "../Definitions.h"
#include "../Containers/Strings/String.hpp"

namespace StdPlusPlus
{
    class STDPLUSPLUS_API Locale
    {
	public:
		//Constructor
		/**
		* 'localeName' is currently an os-dependent (unfortunately) name for a locale.
		* It is strongly discouraged to call this constructor directly, because the same name will likely not work on another platform.
		* Until that is fixed, try to use only the GetUserLocale function, as it is cross-platform.
		*/
		inline Locale(const String &localeName) : localeName(localeName)
		{
		}

		//Functions
		/**
		* Retrieve the locale of the user, that started the process.
		*/
		static Locale GetUserLocale();
	
	private:
		//Members
		String localeName;
	};
}