set(SOURCE_FILES
        ${SOURCE_FILES}

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

		#Math
		${CMAKE_CURRENT_SOURCE_DIR}/Math/CSize.cpp

		#Rendering
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CDeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp

		#Streams
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileInputStream.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileOutputStream.cpp

		#Time
		${CMAKE_CURRENT_SOURCE_DIR}/Time/CTime.cpp

        #UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidgetContainer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CEventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CWindow.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CWinMessageEventQueue.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CCheckBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CDropDown.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CPushButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDefaultTerminalRenderTarget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/C3DView.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.cpp

        PARENT_SCOPE)