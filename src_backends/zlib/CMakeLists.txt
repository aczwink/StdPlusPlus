option(EXTENSION_ZLIB "zlib compression-extension." OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(zlib)
	if (ZLIB_FOUND)
		set(EXTENSION_ZLIB ON)
	endif ()
endif ()



if(${EXTENSION_ZLIB})
	find_package(zlib REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/ExtZLIBInflater.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ExtZLIBInflater.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_ZLIB)
	target_include_directories(Std++ PRIVATE ${ZLIB_INCLUDE_DIRS})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${ZLIB_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()