add_subdirectory(Buffers)
add_subdirectory(CodeAnalysis)
add_subdirectory(CommandLine)
add_subdirectory(CommonFileFormats)
add_subdirectory(Compression)
add_subdirectory(Containers)
add_subdirectory(Cryptography)
add_subdirectory(Debugging)
add_subdirectory(Devices)
add_subdirectory(FileFormats)
add_subdirectory(FileSystem)
add_subdirectory(FormalLanguages)
add_subdirectory(Math)
add_subdirectory(Memory)
add_subdirectory(MIDI)
add_subdirectory(Multimedia)
add_subdirectory(Network)
add_subdirectory(Serialization)
add_subdirectory(Streams)
add_subdirectory(Typography)
add_subdirectory(UI)

set(SOURCE_FILES
	${SOURCE_FILES}

	#_Backends
	${CMAKE_CURRENT_SOURCE_DIR}/_Backends/UI/DrawableWidgetBackend.cpp

	#Devices
	${CMAKE_CURRENT_SOURCE_DIR}/Devices/DeviceEnumerator.cpp

	#EventHandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/StandardEventQueue.cpp

	#format

        ${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Demuxer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/ContainerFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer_DecoderThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer_DemuxerThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Packet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/PixelFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Stream.cpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/StaticThreadPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/DevicePathRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/FlatVectorPath.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/HostComputedPathRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/VectorPath.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Date.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/DateTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Time.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimerEventSource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/WeakDate.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MemoryOperators.cpp

	PARENT_SCOPE)