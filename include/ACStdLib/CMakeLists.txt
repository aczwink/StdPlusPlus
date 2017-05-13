set(SOURCE_FILES
	${SOURCE_FILES}
	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.h
	${CMAKE_CURRENT_SOURCE_DIR}/Color.h
	${CMAKE_CURRENT_SOURCE_DIR}/Debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/Float.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Function.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Integer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.h
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.h
	${CMAKE_CURRENT_SOURCE_DIR}/Tuple.hpp

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Device.hpp

	#Containers
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/ResizeableContainer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/DynamicArray/DynamicArray.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/DynamicArray/DynamicArrayConstIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/DynamicArray/DynamicArrayIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/FiniteSet/FiniteSet.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/FiniteSet/FiniteSetIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/FiniteSet/FiniteSetNode.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedList.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListConstIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListNode.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Map/Map.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Map/MapIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Map/MapNode.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/ByteString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/FixedCharLengthString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/VariableCharLengthString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/String.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/CUTF32String.h
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/ResizeableString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/StringUtil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/CConstUTF8StringIterator.h
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/UTF8String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/UTF16String.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CConstUTF16StringIterator.h

	#ErrorHandling
	${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/FileNotFoundException.hpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/PathIterator.hpp

	#Math
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Point.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Size.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/Matrix4x4.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/Rect.h
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/Vector3.hpp

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/IndexBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/InputLayout.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/InputState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/ShaderProgram.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/VertexBuffer.hpp

        #Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/Readers/TextReader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CBufferInputStream.h
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CStdOut.h

        #Time
        ${CMAKE_CURRENT_SOURCE_DIR}/Time/CCalendar.h
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Time/CTime.h

        #UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Mouse.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Widget.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/MainAppWindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/SizingPolicy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/WidgetContainer.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Window.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CheckBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Label.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/PushButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RadioButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RenderTargetWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Slider.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SpinBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Layouts/GridLayout.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.h

        PARENT_SCOPE)