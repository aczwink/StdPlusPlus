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
//Class header
#include <Std++/_Backends/BackendManager.hpp>
//Backends
#include "gtk/GTKBackend.hpp"
#include "OpenCL/OpenCLBackend.hpp"
//Namespaces
using namespace StdPlusPlus;

#define PRIORITY_HIGH 0
#define PRIORITY_NORMAL 1

//Global functions
void RegisterComputeBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<ComputeBackend>::GetRootInstance().RegisterBackend(backend, priority);
#ifdef _STDPLUSPLUS_BACKEND_OPENCL
	{
		for(auto platformId : OpenCLBackend::GetPlatformIds())
		{
			char buffer[4096];
			clGetPlatformInfo(platformId, CL_PLATFORM_NAME, sizeof(buffer), buffer, nullptr);

			uint32 priority = PRIORITY_NORMAL;
			if(MemCmp(buffer, u8"NVIDIA CUDA", 11) == 0)
				priority = PRIORITY_HIGH;

			OpenCLBackend *openclBackend = new OpenCLBackend(platformId);
			ADD_BACKEND(openclBackend, priority);
		}
	}
#endif
#undef ADD_BACKEND
}

void RegisterUIBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<UIBackend>::GetRootInstance().RegisterBackend(backend, priority);
	//UI backends
#ifdef _STDPLUSPLUS_BACKEND_GTK3
	GTKBackend *gtkBackend = new GTKBackend;
	ADD_BACKEND(gtkBackend, PRIORITY_HIGH);
#endif
#undef ADD_BACKEND
}