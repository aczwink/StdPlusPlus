set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/AddressInUseException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileAlreadyExistsException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileNotFoundException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IllegalDataException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IllegalEncodedCharException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PermissionDeniedException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VerificationFailedException.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/XMLParseException.hpp

	PARENT_SCOPE)