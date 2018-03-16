set(SOURCE_FILES
	${SOURCE_FILES}

	#Strings
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/FixedCharLengthString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ByteString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF32String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ResizeableString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/StringUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-8/ConstUTF8StringIterator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-8/UTF8String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-16/UTF16String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-16/CConstUTF16StringIterator.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/FIFOBuffer.cpp

	PARENT_SCOPE)