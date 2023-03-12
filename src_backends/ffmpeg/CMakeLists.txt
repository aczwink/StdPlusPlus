option(EXTENSION_LIBAVCODEC "libavcodec (ffmpeg) codec-extension." OFF)
option(EXTENSION_LIBAVFORMAT "libavformat (ffmpeg) format-extension." OFF)


#VERY IMPORTANT!
#!license issues!
option(EXTENSION_LIBAVCODEC_PATENTED "Enable patented codecs. YOU SHOULD INFORM YOURSELF ABOUT THE IMPLICATIONS BEFORE ENABLING THIS." ON)
#VERY IMPORTANT!


include(FindFFmpeg.cmake)
if(${BACKENDS_AUTO_AVAILABLE})
	if (LIBAV_FOUND)
		set(EXTENSION_LIBAVCODEC ON)
		set(EXTENSION_LIBAVFORMAT ON)
	endif ()
endif ()

set(EXTENSION_LIBAVCODEC OFF) #TODO: libavcodec currently crashes with some error. needs to be fixed
if(${EXTENSION_LIBAVCODEC})
	if(NOT LIBAV_FOUND)
		message(FATAL_ERROR "libavcodec could not be found.")
	endif()

	set(SOURCE_FILES
		${SOURCE_FILES}

		#codec
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Decoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Decoder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_DecoderContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_DecoderContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Encoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Encoder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_EncoderContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_EncoderContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Parser.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_Parser.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_ParserContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/codec/libavcodec_ParserContext.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Extension.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Extension.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVCODEC)
	if(EXTENSION_LIBAVCODEC_PATENTED)
		target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED)
	endif()
endif()


if(${EXTENSION_LIBAVFORMAT})
	if(NOT LIBAV_FOUND)
		message(FATAL_ERROR "libavformat could not be found.")
	endif()

	set(SOURCE_FILES
		${SOURCE_FILES}

		#format
		${CMAKE_CURRENT_SOURCE_DIR}/libav_Packet.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libav_Packet.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Demuxer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Demuxer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Format.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Format.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Muxer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavformat_Muxer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavio_InputStreamWrapper.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavio_InputStreamWrapper.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavio_OutputStreamWrapper.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/format/libavio_OutputStreamWrapper.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg_Extension.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg_Extension.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavformat_Extension.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavformat_Extension.hpp
	)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVFORMAT)
endif ()

if(${EXTENSION_LIBAVFORMAT} OR ${EXTENSION_LIBAVCODEC})
	target_include_directories(Std++ PRIVATE ${LIBAV_INCLUDE_DIR})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${LIBAV_LIBRARIES})
endif()

#set in parent scope
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)