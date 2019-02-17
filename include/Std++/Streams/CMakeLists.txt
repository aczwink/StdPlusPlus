set(SOURCE_FILES
	${SOURCE_FILES}

	#Bitstreams
	${CMAKE_CURRENT_SOURCE_DIR}/Bitstreams/BitInputStreamBitReversed.hpp

	#Readers
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/DataReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/TextReader.hpp

	#Writers
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/DataWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/TextWriter.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/HashingInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/HashingOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdOut.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/InputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LimitedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SeekableInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdIn.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/TextCodec.hpp

	PARENT_SCOPE)