option(BACKEND_ALSA "ALSA MIDI-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
    find_package(ALSA)
    if (ALSA_FOUND)
        set(BACKEND_ALSA ON)
    endif ()
endif ()




if(${BACKEND_ALSA})
    find_package(ALSA REQUIRED)

    set(SOURCE_FILES
        ${SOURCE_FILES}

        #.
        ${CMAKE_CURRENT_SOURCE_DIR}/ALSAOutputSequencer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ALSAOutputSequencer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ALSABackend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ALSABackend.hpp

            PARENT_SCOPE)

    target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_ALSA)
    target_include_directories(Std++ PRIVATE ${ALSA_INCLUDE_DIRS})
    set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${ALSA_LIBRARIES} PARENT_SCOPE)
endif ()