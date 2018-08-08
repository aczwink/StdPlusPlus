add_subdirectory(_Backends)
add_subdirectory(Audio)
add_subdirectory(CommonFileFormats)
add_subdirectory(Containers)
add_subdirectory(Devices)
add_subdirectory(Errorhandling)
add_subdirectory(Filesystem)
add_subdirectory(Math)
add_subdirectory(Multimedia)
add_subdirectory(Streams)
add_subdirectory(UI)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Buffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/CommandQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Kernel.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Program.hpp

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventSource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventWaiter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/StandardEventQueue.hpp

	#Internationalization
	${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/Locale.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/TimeZone.hpp

	#Multimedia
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmaps/RGB/R8G8B8Image.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmaps/YCbCr/YCbCr420Image.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmaps/RGBAImage.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmaps/RGBPixmap.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmaps/YCbCrImage.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AbstractAudioBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioFrame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Codec.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/ClusterIndex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Demuxer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Encoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/EnumTypes.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Format.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Frame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Pixmap.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaObject.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/MediaPlayer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Muxer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Packet.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/PixelFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Stream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/SubtitleStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoFrame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/VideoStream.hpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/ConditionVariable.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Multitasking.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Mutex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/StaticThreadPool.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.hpp

	#Network
	${CMAKE_CURRENT_SOURCE_DIR}/Network/Datagram.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/DatagramSocket.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/IPv4Address.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Network/NetAddress.hpp

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/DevicePathRenderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/FlatVectorPath.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/HostComputedPathRenderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VectorPath/VectorPath.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/IndexBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/InputLayout.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/InputState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/ShaderProgram.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Texture2D.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VertexBuffer.hpp

	#Smart Pointers
	${CMAKE_CURRENT_SOURCE_DIR}/SmartPointers/AutoPointer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SmartPointers/UniquePointer.hpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Date.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Time.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimerEventSource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/WeakDate.hpp

	#XML
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Document.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Element.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Node.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/TextNode.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__Globaldependencies.h
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.h
	${CMAKE_CURRENT_SOURCE_DIR}/Char.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigurationFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Color.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/Deleters.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Float.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Function.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Integer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Natural.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tuple.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Type.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Variant.hpp

	PARENT_SCOPE)