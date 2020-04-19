option(BACKEND_FUSE "fuse extension" OFF)




if(${BACKENDS_AUTO_AVAILABLE})
	find_package(PkgConfig)
	if (PkgConfig_FOUND)
		pkg_check_modules(FUSE QUIET fuse3)
		if (FUSE_FOUND)
			set(BACKEND_FUSE ON)
		endif ()
	endif ()
endif ()




if(${BACKEND_FUSE})
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(FUSE REQUIRED fuse3)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/fuse3.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/fuse3.hpp
	)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_FUSE3)
	target_include_directories(Std++ PRIVATE ${FUSE_INCLUDE_DIRS})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${FUSE_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()