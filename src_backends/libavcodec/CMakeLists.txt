option(BACKEND_LIBAVCODEC "libavcodec (ffmpeg) codec-backend. YOU SHOULD INFORM YOURSELF ABOUT LICENSE ISSUES BEFORE ENABLING THIS!" ON)

#VERY IMPORTANT!
#because of the license issues, this backend can never be switched on automatically!!!
#VERY IMPORTANT!

if(${BACKEND_LIBAVCODEC})
	include(FindFFmpeg.cmake)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Multimedia
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Encoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Multimedia/libavcodec_Encoder.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Backend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/libavcodec_Backend.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_BACKEND_LIBAVCODEC)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${FFMPEG_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()