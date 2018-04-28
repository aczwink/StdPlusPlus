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
            ${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/UI/CommCtrlWindowBackend.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/UI/Definitions.h
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowsMessageQueueEventSource.hpp

            #.
            ${CMAKE_CURRENT_SOURCE_DIR}/CommCtrlBackend.hpp
            )

    target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_COMMCTRL)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} "Comctl32.lib")

    #set in parent scope
    set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()