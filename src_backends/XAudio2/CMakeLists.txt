option(BACKEND_XAUDIO2 "XAudio2 Audio-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	if (WIN32)
        set(BACKEND_XAUDIO2 ON)
    endif ()
endif ()




if(${BACKEND_XAUDIO2})
	set(SOURCE_FILES
		${SOURCE_FILES}

		#Audio
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/XAudio2DeviceContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/XAudio2DeviceContext.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/XAudio2Backend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/XAudio2Device.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/XAudio2Device.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/XAudio2DeviceEnumeratorState.hpp

		PARENT_SCOPE)

	target_compile_definitions(Std++ PRIVATE _STDXX_BACKEND_XAUDIO2)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} "Xaudio2.lib" PARENT_SCOPE)
endif ()