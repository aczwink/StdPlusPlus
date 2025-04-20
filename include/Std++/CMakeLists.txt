add_subdirectory(_Backends)
add_subdirectory(Audio)
add_subdirectory(Buffers)
add_subdirectory(CodeAnalysis)
add_subdirectory(CommandLine)
add_subdirectory(CommonFileFormats)
add_subdirectory(Compression)
add_subdirectory(Containers)
add_subdirectory(Cryptography)
add_subdirectory(Debugging)
add_subdirectory(Devices)
add_subdirectory(Errorhandling)
add_subdirectory(EventHandling)
add_subdirectory(FileSystem)
add_subdirectory(FileFormats)
add_subdirectory(FormalLanguages)
add_subdirectory(Math)
add_subdirectory(Memory)
add_subdirectory(MIDI)
add_subdirectory(Multimedia)
add_subdirectory(Multitasking)
add_subdirectory(Network)
add_subdirectory(Rendering)
add_subdirectory(Serialization)
add_subdirectory(Streams)
add_subdirectory(Time)
add_subdirectory(Type)
add_subdirectory(Typography)
add_subdirectory(UI)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Buffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/CommandQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Kernel.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Program.hpp

	#Internationalization
	${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/Locale.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/TimeZone.hpp

	#format

		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Demuxer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/EnumTypes.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/ContainerFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Frame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaObject.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Muxer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoDecoder.hpp

	#Smart Pointers
	${CMAKE_CURRENT_SOURCE_DIR}/SmartPointers/AutoPointer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SmartPointers/SharedPointer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SmartPointers/UniquePointer.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__Globaldependencies.h
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.h
	${CMAKE_CURRENT_SOURCE_DIR}/Char.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Color.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Constants.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/Deleters.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Float.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Function.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShutdownManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Signed.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tuple.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Type.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Unsigned.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Variant.hpp

	PARENT_SCOPE)