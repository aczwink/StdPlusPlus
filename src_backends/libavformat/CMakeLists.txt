option(EXTENSION_LIBAVFORMAT "libavformat (ffmpeg) format-extension." OFF)


include(../FindFFmpeg.cmake)
if(${BACKENDS_AUTO_AVAILABLE})
	if (LIBAV_FOUND)
		set(EXTENSION_LIBAVFORMAT ON)
	endif ()
endif ()



if(${EXTENSION_LIBAVFORMAT})
	if(NOT LIBAV_FOUND)
		message(FATAL_ERROR "libavformat could not be found.")
	endif()

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Multimedia
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavformat_Demuxer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavformat_Demuxer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavformat_Format.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavformat_Format.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavio_StreamWrapper.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavio_StreamWrapper.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/libavformat_Backend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavformat_Backend.hpp
	)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBAVFORMAT)

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()