option(BACKEND_XCB_XLIB "XCB with Xlib UI-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(X11)
	if(X11_FOUND)
		find_package(PkgConfig)
		if (PkgConfig_FOUND)
			pkg_check_modules(PKG_X11_XCB QUIET x11-xcb)
			if(PKG_X11_XCB_FOUND)
				set(BACKEND_XCB_XLIB ON)
			endif()
		endif()
	endif()
endif ()




if(${BACKEND_XCB_XLIB})
	find_package(X11 REQUIRED)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(PKG_X11_XCB REQUIRED x11-xcb)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibEventSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibEventSource.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibRenderTargetWidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibRenderTargetWidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibWindowBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/XcbXlibWindowBackend.hpp

		#XLayer
		${CMAKE_CURRENT_SOURCE_DIR}/XLayer/Imports.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/XLayer/XcbWindow.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/XLayer/XConnection.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/XcbXlibBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/XcbXlibBackend.hpp
		)

	#OpenGL3Core extension
	if(${BACKEND_OPENGL})
		set(SOURCE_FILES
			${SOURCE_FILES}

			#Rendering
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/XcbXlibOpenGLDeviceContext.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/XcbXlibOpenGLDeviceContext.hpp

			#.
			${CMAKE_CURRENT_SOURCE_DIR}/XcbXlibOpenGLBackend.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/XcbXlibOpenGLBackend.hpp
			)
	endif ()

	target_compile_definitions(Std++ PRIVATE _STDXX_BACKEND_XCB_XLIB)
	target_include_directories(Std++ PRIVATE ${X11_INCLUDE_DIR})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${X11_LIBRARIES} ${PKG_X11_XCB_LIBRARIES})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()