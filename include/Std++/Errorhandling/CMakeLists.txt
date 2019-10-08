add_subdirectory(Errors)
add_subdirectory(Exceptions)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BaseException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Error.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Exception.hpp

	PARENT_SCOPE)