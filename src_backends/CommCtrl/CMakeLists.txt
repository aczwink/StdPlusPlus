option(BACKEND_COMMCTRL "Windows Common Controls Library UI-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
    if (WIN32)
        set(BACKEND_COMMCTRL ON)
    endif ()
endif ()



if(${BACKEND_COMMCTRL})
    set(SOURCE_FILES
		${SOURCE_FILES}

		#UI
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBarBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBarBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlRenderTargetWidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlRenderTargetWidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Definitions.h
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Win32Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Win32Window.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlBackend.hpp
		)

    target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_COMMCTRL)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} "Comctl32.lib")

	#OpenGL3Core extension
	if(${BACKEND_OPENGL})
		set(SOURCE_FILES
			${SOURCE_FILES}

			#.
			${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlOpenGL3CoreBackend.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlOpenGL3CoreBackend.cpp
			)
	endif ()

    #set in parent scope
    set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()