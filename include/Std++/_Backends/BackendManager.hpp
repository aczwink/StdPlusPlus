/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Tuple.hpp>
#include "../Containers/PriorityQueue.hpp"
#include "../Containers/Map/Map.hpp"
#include "../__InitAndShutdown.h"

namespace StdXX
{
	template<typename BackendClassType>
	class BackendManager
	{
		friend void ::ShutdownStdPlusPlus();
	public:
		//Constructor
		inline BackendManager() : activeBackend(nullptr)
		{
		}

		//Destructor
		inline ~BackendManager()
		{
			this->ReleaseAll();
		}

		//Inline
		inline BackendClassType *GetActiveBackend()
		{
			if(this->activeBackend == nullptr)
				this->SetActiveBackend(this->GetPreferredBackend());
			return this->activeBackend;
		}

		inline void RegisterBackend(BackendClassType *backend, uint32 priority)
		{
			this->backends.Insert({priority, backend});
		}

		inline void SetActiveBackend(BackendClassType *backend)
		{
			if(this->activeBackend != nullptr)
				this->activeBackend->Unload();
			backend->Load();
			this->activeBackend = backend;
		}

		//Inline functions
		static BackendManager<BackendClassType> &GetRootInstance()
		{
			static BackendManager<BackendClassType> instance;

			return instance;
		}

	private:
		//Members
		PriorityQueue<Tuple<uint32, BackendClassType *>> backends;
		BackendClassType *activeBackend;

		//Inline
		inline BackendClassType *GetPreferredBackend() const
		{
		    ASSERT(!this->backends.IsEmpty(), u8"Tried to instantiate backend, but none is available");
			return this->backends.Top().template Get<1>();
		}

		inline void ReleaseAll()
		{
			if (this->activeBackend != nullptr)
				this->activeBackend->Unload();
			this->activeBackend = nullptr;

			while(!this->backends.IsEmpty())
				delete this->backends.PopTop().template Get<1>();
			this->backends.Release();
		}
	};
}