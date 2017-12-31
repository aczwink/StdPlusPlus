set(SOURCE_FILES
	${SOURCE_FILES}

	#Bitstreams
	${CMAKE_CURRENT_SOURCE_DIR}/Bitstreams/BitInputStreamBitReversed.hpp

	#Readers
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/DataReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/TextReader.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/CStdOut.h
	${CMAKE_CURRENT_SOURCE_DIR}/InputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SeekableInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdIn.hpp

	PARENT_SCOPE)