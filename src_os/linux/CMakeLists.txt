set(SOURCE_FILES
	${SOURCE_FILES}

	#Cryptography
	${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/SecureRandomNumberGenerator.cpp

	#EventHandling
	${CMAKE_CURRENT_SOURCE_DIR}/EventHandling/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EventHandling/EventSignal.cpp

	#FileSystem
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystem/FileSystemsManager.cpp

	#Internationalization
	${CMAKE_CURRENT_SOURCE_DIR}/Internationalization/TimeZone.cpp

	#Multitasking
	${CMAKE_CURRENT_SOURCE_DIR}/Multitasking/Thread.cpp

	#Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileOutputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileUpdateStream.cpp

	PARENT_SCOPE)