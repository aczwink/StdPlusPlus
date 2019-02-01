add_subdirectory(x86_64)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Decoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Program.cpp

	PARENT_SCOPE)