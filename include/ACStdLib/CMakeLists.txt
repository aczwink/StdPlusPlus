add_subdirectory(Containers)
add_subdirectory(Streams)

set(SOURCE_FILES
	${SOURCE_FILES}

	#_Backends
	${CMAKE_CURRENT_SOURCE_DIR}/_Backends/Backend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/_Backends/BackendManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/_Backends/UIBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/_Backends/WindowBackend.hpp

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Buffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/CommandQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Device.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Kernel.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Program.hpp

	#ErrorHandling
	${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/Exception.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/FileNotFoundException.hpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerDirectory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/ContainerFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Directory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/File.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystemFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystemNode.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/PathIterator.hpp

	#Math
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/AxisAlignedBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/ObjectIntersection.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/Rect.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Math/Degree.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Fraction.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix2x2.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix3x3.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Matrix4x4.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Point.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Radian.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Size.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector2.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector3.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Vector4.hpp

	#Multimedia
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGB/R8G8B8Image.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/YCbCr/YCbCr420Image.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGBAImage.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/RGBImage.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Images/YCbCrImage.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AbstractAudioBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioFrame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/AudioStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Codec.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/CodecId.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/ClusterIndex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Demuxer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Encoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/EnumTypes.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Format.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Frame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/Image.hpp
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
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Calendar.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Time.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.hpp

	#UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/ControllerIndex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/ListController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/SelectionController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/TableController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controllers/TreeController.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CheckBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Label.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/LineEdit.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/PushButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RadioButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SearchBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Slider.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SpinBox.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/PathRenderTargetWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/RenderTargetWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/VideoWidget.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Layouts/GridLayout.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.h

	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/ComboBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TableView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TreeView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/View.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/UI/CommonDialogs.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/EventQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Keyboard.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/MainAppWindow.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Mouse.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/SizingPolicy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Widget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/WidgetContainer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Window.hpp

	#XML
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Document.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Element.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/Node.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XML/TextNode.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.h
	${CMAKE_CURRENT_SOURCE_DIR}/Char.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigurationFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Color.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/Definitions.h
	${CMAKE_CURRENT_SOURCE_DIR}/Deleters.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Float.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Function.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Integer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.h
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.h
	${CMAKE_CURRENT_SOURCE_DIR}/Natural.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Tuple.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Type.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Variant.hpp

	PARENT_SCOPE)