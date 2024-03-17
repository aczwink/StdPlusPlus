set(SOURCE_FILES
	${SOURCE_FILES}

	#DEFLATE
	${CMAKE_CURRENT_SOURCE_DIR}/DEFLATE/Inflater.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DEFLATE/Inflater.hpp

	#gzip
	${CMAKE_CURRENT_SOURCE_DIR}/gzip/gzipCompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gzip/gzipCompressor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gzip/gzipDecompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gzip/gzipDecompressor.hpp

	#zlib
	${CMAKE_CURRENT_SOURCE_DIR}/ZLIB/ZLIBDecompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ZLIB/ZLIBDecompressor.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Compressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Decompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DictionaryDecompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HuffmanDecoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IndexedSlidingDictionary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RedundancyBasedCompressor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SlidingDictionary.cpp

	PARENT_SCOPE)