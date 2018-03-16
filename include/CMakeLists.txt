add_subdirectory(Std++)
add_subdirectory(Std++CPUOpt)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Std++.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Std++CPUOpt.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Std++Test.hpp

	PARENT_SCOPE)