option(BACKEND_GTK3 "GTK3 UI-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(PkgConfig)
	if (PkgConfig_FOUND)
		pkg_check_modules(GTK3 QUIET gtk+-3.0)
		if (GTK3_FOUND)
			set(BACKEND_GTK3 ON)
		endif ()
	endif ()
endif ()




if(${BACKEND_GTK3})
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Math
		${CMAKE_CURRENT_SOURCE_DIR}/Math/Size.cpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/GTKBackend.hpp

		PARENT_SCOPE)

	target_compile_definitions(ACStdLib PRIVATE _ACSTDLIB_BACKEND_GTK3)
	add_definitions(${GTK3_CFLAGS_OTHER})
	target_include_directories(ACStdLib PRIVATE ${GTK3_INCLUDE_DIRS})
	set(ACSTDLIB_PRIVATE_LIBRARIES ${ACSTDLIB_PRIVATE_LIBRARIES} ${GTK3_LIBRARIES} PARENT_SCOPE)

	#GLX is needed for getting opengl extensions
	pkg_check_modules(PC_GLX QUIET glx)
	find_library(GLX_LIBRARY
		GL
		HINTS ${PC_GLX_LIBDIR} ${PC_GLX_LIBRARY_DIRS}
	)
endif ()