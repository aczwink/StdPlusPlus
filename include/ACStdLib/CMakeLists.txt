set(SOURCE_FILES
	${SOURCE_FILES}
	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.h
	${CMAKE_CURRENT_SOURCE_DIR}/Color.h
	${CMAKE_CURRENT_SOURCE_DIR}/Tuple.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Mathematics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory.h

	#Compute
	${CMAKE_CURRENT_SOURCE_DIR}/Compute/Device.hpp

	#Containers
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedList.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListConstIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/LinkedList/LinkedListNode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/AFixedCharLengthString.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/C8BitString.h
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/String.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/CUTF32String.h
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/StringUtil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/CConstUTF8StringIterator.h
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-8/UTF8String.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CUTF16String.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Containers/Strings/UTF-16/CConstUTF16StringIterator.h

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/PathIterator.hpp

	#Math
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Size.h
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Geometry/Rect.h

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.h

        #Streams
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/Readers/CTextReader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CBufferInputStream.h
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CStdOut.h

        #Time
        ${CMAKE_CURRENT_SOURCE_DIR}/Time/CCalendar.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Time/CTime.h

        #UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Widget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/MainAppWindow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CSizingPolicy.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/WidgetContainer.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Window.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CheckBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/PushButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RenderTargetWidget.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Layouts/CGridLayout.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.h

        PARENT_SCOPE)