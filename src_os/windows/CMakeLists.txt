set(SOURCE_FILES
	${SOURCE_FILES}

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventWaiter.cpp
	
	#FileSystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/OSFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsDirectory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsRoot.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/WindowsRoot.hpp

		#Internationalization
		${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/Locale.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/TimeZone.cpp
		
		#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/ConditionVariable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Multitasking.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Mutex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

		#Rendering
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CDeviceContext.cpp

		#Streams
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileInputStream.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileOutputStream.cpp

		#Time
		${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Time/DateTime.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.cpp

        #UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDefaultTerminalRenderTarget.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/CWindow.cpp

		#.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

        PARENT_SCOPE)