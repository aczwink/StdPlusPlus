add_subdirectory(Instructions)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Architecture.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Instruction.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Program.hpp

	PARENT_SCOPE)