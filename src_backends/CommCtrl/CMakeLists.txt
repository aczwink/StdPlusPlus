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
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlCheckBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlCheckBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlContainerBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlContainerBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlGroupBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlGroupBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlLabelBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlLabelBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBarBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlMenuBarBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlPushButtonBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlPushButtonBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlRenderTargetWidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlRenderTargetWidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlScrollAreaBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlScrollAreaBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlSliderBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlSliderBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlSpinBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlSpinBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Win32Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Win32Window.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Imports.h
		)

    target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_COMMCTRL)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} "Comctl32.lib" "UxTheme.lib")

	#OpenGL3Core extension
	if(${BACKEND_OPENGL})
		set(SOURCE_FILES
			${SOURCE_FILES}

			#Rendering
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CommCtrlOpenGL3CoreDeviceContext.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/CommCtrlOpenGL3CoreDeviceContext.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/WGL.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/WGL.h

			#.
			${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlOpenGL3CoreBackend.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlOpenGL3CoreBackend.cpp
			)
	endif ()

    #set in parent scope
    set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()