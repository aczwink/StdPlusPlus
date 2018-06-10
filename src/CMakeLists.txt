add_subdirectory(CommonFileFormats)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Containers)
add_subdirectory(Devices)
add_subdirectory(Math)
add_subdirectory(Multimedia/Codecs)
add_subdirectory(Multimedia/Formats)
add_subdirectory(Streams)
add_subdirectory(UI)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Devices
	${CMAKE_CURRENT_SOURCE_DIR}/Devices/DeviceEnumerator.cpp

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/StandardEventQueue.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/DirectoryWalker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.cpp

	#Multimedia
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGB/R8G8B8Image.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/YCbCr/YCbCr420Image.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGBImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGBAImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/YCbCrImage.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/__Register.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AbstractAudioBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioFrame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/ClusterIndex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Codec.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Decoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Demuxer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Format.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/FourCC.h
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer_DecoderThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer_DemuxerThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Packet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/PixelFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Stream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/SubtitleStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoFrame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoStream.cpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/StaticThreadPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

	#Network
	${CMAKE_CURRENT_SOURCE_DIR}/Network/Datagram.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/DatagramSocket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/IPv4Address.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/Shared.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/Shared.hpp

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/DevicePathRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/FlatVectorPath.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/HostComputedPathRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/VectorPath.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLCubeMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLFrameBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLIndexBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLInputState.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShaderProgram.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLTexture2D.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLVertexBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Calendar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Time.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimerEventSource.cpp

	#XML
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Document.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Element.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/TextNode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/CXMLParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/CXMLParser.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigurationFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	PARENT_SCOPE)