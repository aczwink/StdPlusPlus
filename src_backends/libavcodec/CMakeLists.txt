option(EXTENSION_LIBAVCODEC "libavcodec (ffmpeg) codec-backend." OFF)

#VERY IMPORTANT!
#!license issues!
option(EXTENSION_LIBAVCODEC_PATENTED "Enable patented codecs. YOU SHOULD INFORM YOURSELF ABOUT THE IMPLICATIONS BEFORE ENABLING THIS." OFF)
#VERY IMPORTANT!

include(FindFFmpeg.cmake)
if(${BACKENDS_AUTO_AVAILABLE})
	if (LIBAV_FOUND)
		set(EXTENSION_LIBAVCODEC ON)
	endif ()
endif ()

if(${EXTENSION_LIBAVCODEC})
	if(NOT LIBAV_FOUND)
		message(FATAL_ERROR "libavcodec could not be found.")
	endif()

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Multimedia
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Decoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Decoder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_DecoderContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_DecoderContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Encoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Encoder.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_EncoderContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_EncoderContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Parser.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Parser.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_ParserContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_ParserContext.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Backend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Backend.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVCODEC)
	if(EXTENSION_LIBAVCODEC_PATENTED)
		target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED)
	endif()

	target_include_directories(Std++ PRIVATE ${LIBAV_INCLUDE_DIR})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${LIBAV_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()