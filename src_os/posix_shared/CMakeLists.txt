add_subdirectory(Filesystem)

set(SOURCE_FILES
	${SOURCE_FILES}

	#EventHandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp

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
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileUpdateStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/read.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/StdIn.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/Clock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/DateTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	PARENT_SCOPE)