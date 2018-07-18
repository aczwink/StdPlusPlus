option(BACKEND_COCOA "macOS Cocoa UI-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	if (APPLE)
		set(BACKEND_COCOA ON)
	endif ()
endif ()



if(${BACKEND_COCOA})
	find_library(COCOA_LIBRARY Cocoa REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaEventSource.hh
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaEventSource.mm
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaMenuBarBackend.hh
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaMenuBarBackend.mm
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaWindowBackend.hh
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CocoaWindowBackend.mm
		${CMAKE_CURRENT_SOURCE_DIR}/UI/OpenGLView.hh
		${CMAKE_CURRENT_SOURCE_DIR}/UI/OpenGLView.mm
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowDelegate.hh
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowDelegate.mm

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/CocoaBackend.mm
		${CMAKE_CURRENT_SOURCE_DIR}/CocoaBackend.hh
		)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_COCOA)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${COCOA_LIBRARY})

	#OpenGL3Core extension
	if(${BACKEND_OPENGL})
		set(SOURCE_FILES
			${SOURCE_FILES}

			#Rendering
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CocoaOpenGL3CoreDeviceContext.hh
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CocoaOpenGL3CoreDeviceContext.mm

			#.
			${CMAKE_CURRENT_SOURCE_DIR}/CocoaOpenGL3CoreBackend.hh
			${CMAKE_CURRENT_SOURCE_DIR}/CocoaOpenGL3CoreBackend.mm
			)
	endif ()


	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()