set(SOURCE_FILES
	${SOURCE_FILES}

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/DirectoryIterator.cpp

	#Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileOutputStream.cpp

	#Time
	${CMAKE_CURRENT_SOURCE_DIR}/Time/TimeMisc.cpp

	PARENT_SCOPE)