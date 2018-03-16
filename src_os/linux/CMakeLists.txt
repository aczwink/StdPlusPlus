set(SOURCE_FILES
        ${SOURCE_FILES}

        #.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/DirectoryIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.cpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/ConditionVariable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Multitasking.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Mutex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

        #Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp

        #Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileOutputStream.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Timer.cpp

	#UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDefaultTerminalRenderTarget.cpp

	PARENT_SCOPE)