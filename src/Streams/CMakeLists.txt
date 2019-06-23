add_subdirectory(ChecksumFunctions)
add_subdirectory(Textcodecs)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Bitstreams
	${CMAKE_CURRENT_SOURCE_DIR}/Bitstreams/BitInputStreamBitReversed.cpp

	#Readers
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/TextReader.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedOutputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferOutputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ChainedInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CheckedInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/InputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LimitedInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NullOutputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdOut.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TextCodec.cpp

	PARENT_SCOPE)