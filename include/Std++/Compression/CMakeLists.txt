set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/CompressionAlgorithm.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Compressor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Decompressor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DictionaryDecompressor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/HuffmanDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IndexedSlidingDictionary.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SlidingDictionary.hpp

	PARENT_SCOPE)