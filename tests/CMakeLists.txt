add_subdirectory(Serialization)
add_subdirectory(Cryptography)
add_subdirectory(FileSystem)
add_subdirectory(FormalLanguages)
add_subdirectory(Math)
add_subdirectory(Time)

set(TEST_FILES
	${TEST_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/FloatToString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lzma.cpp

	PARENT_SCOPE)