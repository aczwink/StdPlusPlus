add_subdirectory(Codecs)
add_subdirectory(Formats)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__Register.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AudioSampleFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BlockIndex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ComputePixmapResampler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodingFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CodingFormatIdMap.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FormatRegistry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ID3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ID3.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DecoderContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Encoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PacketWrapper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParserContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RGBPixmapView.cpp

	PARENT_SCOPE)