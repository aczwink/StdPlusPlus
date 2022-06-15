/*
 * Copyright (c) 2017-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory.hpp>
#include <Std++/EventHandling/EventQueue.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/_Backends/ComputeBackend.hpp>
#include <Std++/_Backends/ExtensionManager.hpp>
#include <Std++/UI/Style/StyleSheet.hpp>
#include <Std++/Compression/HuffmanDecoder.hpp>
#include <Std++/FileSystem/FileSystemsManager.hpp>
#include <Std++/Memory/MemoryManager.hpp>
#include <Std++/Memory/MemoryTrackingAllocator.hpp>
#include <Std++/ShutdownManager.hpp>
#include <Std++/_Backends/AudioBackend.hpp>
#include <Std++/_Backends/MIDIBackend.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Prototypes
void InitStdPlusPlus_Platform();
void ShutdownStdPlusPlus_Platform();
namespace _stdxx_
{
	void RegisterFileSystemFormats();
}

#ifdef XPC_BUILDTYPE_DEBUG
UniquePointer<Memory::MemoryTrackingAllocator> memoryTrackingAllocator;
#endif

//Global functions
void InitStdPlusPlus()
{
#ifdef XPC_BUILDTYPE_DEBUG
	memoryTrackingAllocator = new Memory::MemoryTrackingAllocator(Memory::MemoryManager::GlobalAllocator());
	Memory::MemoryManager::GlobalAllocator(*memoryTrackingAllocator);
#endif

	void RegisterBackends();
	RegisterBackends();

	_stdxx_::RegisterFileSystemFormats();

	void RegisterExtensions();
	RegisterExtensions();

	InitStdPlusPlus_Platform();
}

void ShutdownStdPlusPlus()
{
	ShutdownManager::Instance().Shutdown();

	ShutdownStdPlusPlus_Platform();

	FileSystem::FileSystemsManager::Instance().ReleaseAllFormats();

	//free inflater stuff
	extern HuffmanDecoder* g_fixedLiteralLengthDecoder;
	extern HuffmanDecoder* g_fixedDistanceDecoder;

	delete g_fixedDistanceDecoder;
	delete g_fixedLiteralLengthDecoder;

	//release memory of global style sheet
	UI::StyleSheet::Global() = {};

	//release backends
	BackendManager<AudioBackend>::GetRootInstance().ReleaseAll();
	BackendManager<ComputeBackend>::GetRootInstance().ReleaseAll();
	BackendManager<MIDIBackend>::GetRootInstance().ReleaseAll();
	BackendManager<RenderBackend>::GetRootInstance().ReleaseAll();
	BackendManager<UIBackend>::GetRootInstance().ReleaseAll();

	//release extensions
	ExtensionManager::GetRootInstance().ReleaseAll();

#ifdef XPC_BUILDTYPE_DEBUG
	//look for memory leaks
	memoryTrackingAllocator->DumpMemoryLeaks();
	Memory::MemoryManager::GlobalAllocator(Memory::MemoryManager::ProcessSystemAllocator());
	memoryTrackingAllocator = nullptr;
#endif
}