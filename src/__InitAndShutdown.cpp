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
//Corresponding header
#include <Std++/__InitAndShutdown.h>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Eventhandling/EventQueue.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/FileSystem/Format.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/_Backends/ComputeBackend.hpp>
#include <Std++/_Backends/ExtensionManager.hpp>
#include <Std++/UI/Style/StyleSheet.hpp>
#include <Std++/Compression/HuffmanDecoder.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Prototypes
void InitStdPlusPlus_Platform();
void MultimediaRegisterCodecsAndFormats();
void ShutdownStdPlusPlus_Platform();
namespace _stdxx_
{
	void RegisterFileSystemFormats();
}

//Global functions
void InitStdPlusPlus()
{
	void RegisterAudioBackends();
	RegisterAudioBackends();
	void RegisterComputeBackends();
	RegisterComputeBackends();
	void RegisterUIBackends();
	RegisterUIBackends();

	_stdxx_::RegisterFileSystemFormats();

	MultimediaRegisterCodecsAndFormats();

	void RegisterExtensions();
	RegisterExtensions();

	InitStdPlusPlus_Platform();
}

void ShutdownStdPlusPlus()
{
	ShutdownStdPlusPlus_Platform();

	//release file system formats
	extern DynamicArray<const FileSystem::Format *> g_fsFormats;
	for(const FileSystem::Format *format : g_fsFormats)
		delete(format);
	g_fsFormats.Release();

	//free inflater stuff
	extern HuffmanDecoder* g_fixedLiteralLengthDecoder;
	extern HuffmanDecoder* g_fixedDistanceDecoder;

	delete g_fixedDistanceDecoder;
	delete g_fixedLiteralLengthDecoder;

	//release memory of global style sheet
	UI::StyleSheet::Global() = {};

	//release backends
	BackendManager<ComputeBackend>::GetRootInstance().ReleaseAll();
	BackendManager<RenderBackend>::GetRootInstance().ReleaseAll();
	BackendManager<UIBackend>::GetRootInstance().ReleaseAll();

	//release extensions
	ExtensionManager::GetRootInstance().ReleaseAll();

#ifdef XPC_BUILDTYPE_DEBUG
    //look for memory leaks
	DebugDumpMemoryLeaks();
#endif
}