/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Corresponding header
#include <ACStdLib/__InitAndShutdown.h>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
#include <ACStdLib/Containers/Map/Map.hpp>
#include <ACStdLib/Containers/Strings/ByteString.hpp>
#include <ACStdLib/Multimedia/CodecId.hpp>
#include <ACStdLib/UI/EventQueue.hpp>
#include <ACStdLib/_Backends/BackendManager.hpp>
#include <ACStdLib/Filesystem/FileSystemFormat.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Prototypes
void InitACStdLib_Platform();
void MultimediaRegisterCodecsAndFormats();
void ShutdownACStdLib_Platform();

//Local functions
static void FreeAudioVideo()
{
	//free static variables so that they don't get reported as memory leaks
	extern Map<ByteString, CodecId> g_matroskaCodecStringMap;
	g_matroskaCodecStringMap.Release();

	extern Map<uint16, CodecId> g_ms_audio_twoCC_map;
	g_ms_audio_twoCC_map.Release();

	extern Map<uint32, CodecId> g_ms_video_fourCC_map;
	g_ms_video_fourCC_map.Release();

#ifdef _AC_LIB_USEAVCODEC
	extern ACStdLib::Map<ACStdLib::Multimedia::CodecId, uint32> g_libavcodec_codec_map;
	g_libavcodec_codec_map.Release();
#endif
}

//Global functions
void InitACStdLib()
{
	MultimediaRegisterCodecsAndFormats();

	InitACStdLib_Platform();
}

void ShutdownACStdLib()
{
	ShutdownACStdLib_Platform();

	FreeAudioVideo();

	//release file system formats
	extern DynamicArray<const FileSystemFormat *> g_fsFormats;
	for(const FileSystemFormat *format : g_fsFormats)
		delete(format);
	g_fsFormats.Release();

	//shut down global event queue
	extern UI::EventQueue *g_globalEventQueue;
	if(g_globalEventQueue)
		delete g_globalEventQueue;

	//release backends
	BackendManager::GetInstance().ReleaseAll();

    //look for memory leaks
    ASSERT(!DebugDumpMemoryLeaks(), u8"You have memory leaks. Check ACStdLib MemLeaks.txt");
}