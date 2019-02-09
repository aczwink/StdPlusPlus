option(EXTENSION_LIBLZMA "liblzma (XZ Utils) compression-backend." OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(liblzma)
	if (LIBLZMA_FOUND)
		set(EXTENSION_LIBLZMA ON)
	endif ()
endif ()



if(${EXTENSION_LIBLZMA})
	find_package(liblzma REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/LZMACompressor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/LZMACompressor.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_LIBLZMA)
	target_include_directories(Std++ PRIVATE ${LIBLZMA_INCLUDE_DIRS})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${LIBLZMA_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()