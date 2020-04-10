set(SOURCE_FILES
	${SOURCE_FILES}

	#Strings
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/FixedCharLengthString.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF32String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ResizeableString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/String.cpp

		#.
	${CMAKE_CURRENT_SOURCE_DIR}/FIFOBuffer.cpp

	PARENT_SCOPE)