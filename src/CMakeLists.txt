add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Codecs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Formats)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Buffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/CommandQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Device.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/DeviceContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Kernel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Program.cpp

        #Containers
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/FixedCharLengthString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/ByteString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF32String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/ResizeableString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/StringUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/ConstUTF8StringIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/UTF8String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/UTF16String.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CConstUTF16StringIterator.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.cpp

	#Math
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/AxisAlignedBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/ObjectIntersection.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Math/Degree.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix2x2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix3x3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix4x4.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Radian.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector4.cpp

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

        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CCubeMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CFrameBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLIndexBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLInputState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CShader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CShaderProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CTexture2D.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLVertexBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.hpp

	#Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/Bitstreams/BitInputStreamBitReversed.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Streams/Textcodecs/ASCIITextCodec.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Streams/Readers/TextReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/BufferedInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/BufferInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/InputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CStdOut.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/StdIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/TextCodec.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Calendar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Time.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.cpp

	#UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/PathRenderTargetWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/RenderTargetWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/VideoWidget.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Widget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/WidgetContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/MainAppWindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/SizingPolicy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/ControllerIndex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/ListController.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/TableController.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CheckBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Label.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/LineEdit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/PushButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RadioButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SearchBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Slider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SpinBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Layouts/GridLayout.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/ATextRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDesktopRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/IRenderTarget.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/ComboBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TableView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TreeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/View.cpp

	#XML
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Document.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Element.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/TextNode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/CXMLParser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/CXMLParser.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigurationFile.cpp

	PARENT_SCOPE)