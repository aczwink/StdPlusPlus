set(SOURCE_FILES
	${SOURCE_FILES}

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/OSFileSystem.cpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/ConditionVariable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Multitasking.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Mutex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

	#Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.cpp

	#Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileOutputStream.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	PARENT_SCOPE)