add_subdirectory(OpenCL)
add_subdirectory(OpenGL)

add_subdirectory(gtk)

set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/RegisterBackends.cpp

	PARENT_SCOPE)