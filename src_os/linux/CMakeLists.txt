set(SOURCE_FILES
	${SOURCE_FILES}

	#Eventhandling
	${CMAKE_CURRENT_SOURCE_DIR}/Eventhandling/EventQueue.cpp

	#Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileOutputStream.cpp

	PARENT_SCOPE)