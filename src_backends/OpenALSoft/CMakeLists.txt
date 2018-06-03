option(BACKEND_OPENALSOFT "OpenAL Soft Audio-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(OpenAL)
	if (OPENAL_FOUND)
		set(BACKEND_OPENALSOFT ON)
	endif ()
endif ()




if(${BACKEND_OPENALSOFT})
	find_package(OpenAL REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Audio
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftDeviceContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftDeviceContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/OpenALSoftSource.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/OpenALSoftBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenALSoftDevice.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenALSoftDeviceEnumeratorState.hpp

		PARENT_SCOPE)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_OPENALSOFT)
	target_include_directories(Std++ PRIVATE ${OPENAL_INCLUDE_DIR})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${OPENAL_LIBRARY} PARENT_SCOPE)
endif ()