set(SOURCE_FILES
	${SOURCE_FILES}

	#Bitstreams
	${CMAKE_CURRENT_SOURCE_DIR}/Bitstreams/BitInputStreamBitReversed.hpp

	#Readers
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/DataReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Readers/TextReader.hpp

	#Writers
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/DataWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/PrintWriterStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/StdOut.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Writers/TextWriter.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BlockInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BufferOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ChainedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/CheckedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ChecksumFunction.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileUpdateStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/InputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LimitedEndInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LimitedInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/NullOutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OutputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReadOnlyInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SeekableInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Seekable.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StdIn.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StringInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/TextCodec.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UpdateStream.hpp

	PARENT_SCOPE)