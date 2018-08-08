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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include "../__InitAndShutdown.h"
#include "Extension.hpp"

namespace _stdxx_
{
	class ExtensionManager
	{
		friend void ::ShutdownStdPlusPlus();
	public:
		//Destructor
		inline ~ExtensionManager()
		{
			this->ReleaseAll();
		}

		//Inline
		inline void RegisterExtension(Extension *extension)
		{
			this->extensions.InsertTail(extension);
			extension->Load();
		}

		//Inline functions
		static ExtensionManager &GetRootInstance()
		{
			static ExtensionManager instance;

			return instance;
		}

	private:
		//Members
		StdXX::LinkedList<Extension *> extensions;

		inline void ReleaseAll()
		{
			while (!this->extensions.IsEmpty())
			{
				Extension *ext = this->extensions.PopFront();
				ext->Unload();
				delete ext;
			}
			this->extensions.Release();
		}
	};
}