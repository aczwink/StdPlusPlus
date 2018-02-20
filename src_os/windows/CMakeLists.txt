set(SOURCE_FILES
        ${SOURCE_FILES}

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

		#Filesystem
		${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/PathIterator.cpp

		#Math
		${CMAKE_CURRENT_SOURCE_DIR}/Math/CSize.cpp
		
		#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/ConditionVariable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Multitasking.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Mutex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

		#Rendering
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CDeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp

		#Streams
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileInputStream.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileOutputStream.cpp

		#Time
		${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Time/CTime.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.cpp

        #UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Label.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/LineEdit.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Slider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SpinBox.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/RenderTargetWidget.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDefaultTerminalRenderTarget.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/C3DView.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TableView.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/AWidgetContainer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CEventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CWindow.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CWinMessageEventQueue.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CCheckBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/ComboBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CPushButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RadioButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp

		#.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp

        PARENT_SCOPE)