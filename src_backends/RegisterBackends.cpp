/*
 * Copyright (c) 2017-2020,2022 Amir Czwink (amir130@hotmail.de)
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
#include "ALSA/ALSABackend.hpp"
#include "Cocoa/CocoaBackend.hh"
#include "CommCtrl/CommCtrlBackend.hpp"
#include "gtk3/Gtk3Backend.hpp"
#include "OpenCL1/OpenCL1Backend.hpp"
#include "OpenALSoft/OpenALSoftBackend.hpp"
#include "XAudio2/XAudio2Backend.hpp"
#ifdef _STDXX_BACKEND_XCB_XLIB
#include "xcb_xlib/XcbXlibBackend.hpp"
#endif
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

#define PRIORITY_HIGH 2
#define PRIORITY_NORMAL 1
#define PRIORITY_LOW 0

//Local functions
static void RegisterAudioBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<AudioBackend>::GetRootInstance().RegisterBackend(backend, priority);
#ifdef _STDPLUSPLUS_BACKEND_OPENALSOFT
	{
		OpenALSoftBackend *backend = new OpenALSoftBackend;
		ADD_BACKEND(backend, PRIORITY_NORMAL);
	}
#endif

#ifdef _STDXX_BACKEND_XAUDIO2
	{
		XAudio2Backend* backend = new XAudio2Backend;
		ADD_BACKEND(backend, PRIORITY_NORMAL);
	}
#endif

#undef ADD_BACKEND
}

static void RegisterComputeBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<ComputeBackend>::GetRootInstance().RegisterBackend(backend, priority);
#ifdef _STDPLUSPLUS_BACKEND_OPENCL
	{
		for(auto platformId : OpenCL1Backend::GetPlatformIds())
		{
			char buffer[4096];
			clGetPlatformInfo(platformId, CL_PLATFORM_NAME, sizeof(buffer), buffer, nullptr);

			uint32 priority = PRIORITY_NORMAL;
			if(MemCmp(buffer, u8"NVIDIA CUDA", 11) == 0)
				priority = PRIORITY_HIGH;

			OpenCL1Backend *openclBackend = new OpenCL1Backend(platformId);
			ADD_BACKEND(openclBackend, priority);
		}
	}
#endif
#undef ADD_BACKEND
}

static void RegisterMIDIBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<MIDIBackend>::GetRootInstance().RegisterBackend(backend, priority);
#ifdef _STDPLUSPLUS_BACKEND_ALSA
	{
		ALSABackend *backend = new ALSABackend;
		ADD_BACKEND(backend, PRIORITY_NORMAL);
	}
#endif
#undef ADD_BACKEND
}

static void RegisterUIBackends()
{
#define ADD_BACKEND(backend, priority) BackendManager<UIBackend>::GetRootInstance().RegisterBackend(backend, priority);
	//UI backends
#ifdef _STDPLUSPLUS_BACKEND_COCOA
	CocoaBackend *cocoaBackend = new CocoaBackend;
	ADD_BACKEND(cocoaBackend, PRIORITY_HIGH);
#endif
#ifdef _STDPLUSPLUS_BACKEND_COMMCTRL
	CommCtrlBackend *commCtrlBackend = new CommCtrlBackend;
	ADD_BACKEND(commCtrlBackend, PRIORITY_NORMAL);
#endif
#ifdef _STDPLUSPLUS_BACKEND_GTK3
	Gtk3Backend *gtk3Backend = new Gtk3Backend;
	ADD_BACKEND(gtk3Backend, PRIORITY_NORMAL);
#endif
#ifdef _STDXX_BACKEND_XCB_XLIB
	ADD_BACKEND(new XcbXlibBackend, PRIORITY_LOW);
#endif
#undef ADD_BACKEND
}

//Global functions
void RegisterBackends()
{
	RegisterAudioBackends();
	RegisterComputeBackends();
	RegisterMIDIBackends();
	RegisterUIBackends();
}