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

		#UI
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/CommonDialogs/OpenFile.cpp

		#UI/Containers
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/Gtk3TabContainerBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/Gtk3TabContainerBackend.hpp

		#UI/Controls
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3CheckBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3CheckBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3ProgressBarBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3ProgressBarBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3TextEditBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Gtk3TextEditBackend.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/LineEdit.cpp

		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/_GtkOpenGLWidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Displays/_GtkOpenGLWidget.h

		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/Gtk3MenuBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/Gtk3MenuBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/Gtk3MenuBarBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/Gtk3MenuBarBackend.hpp

		#Transfer
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Transfer/Gtk3ClipboardBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Transfer/Gtk3ClipboardBackend.hpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/Gtk3ListViewBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/Gtk3ListViewBackend.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/TableView.cpp

		#Windows
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Windows/Gtk3DialogBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Windows/Gtk3DialogBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Windows/Gtk3WindowBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Windows/Gtk3WindowBackend.hpp

		${CMAKE_CURRENT_SOURCE_DIR}/UI/_RedirectGtkContainer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/_RedirectGtkContainer.h
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3DrawingAreaBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3DrawingAreaBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3GroupBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3GroupBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3LabelBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3LabelBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3PushButtonBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3PushButtonBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3RedirectContainer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3RedirectContainer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3RenderTargetWidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3RenderTargetWidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3SelectBoxBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3SelectBoxBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3SliderBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3SliderBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3TreeViewBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3TreeViewBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3WidgetBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk3WidgetBackend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/GtkEventSource.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/UI/GtkEventSource.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/GtkWindowBackend.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/UI/oldGtkWindowBackend.hpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Keyboard.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/Mouse.cpp
		#${CMAKE_CURRENT_SOURCE_DIR}/UI/WindowBackend.cpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/Gtk3Backend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Gtk3Backend.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_GTK3)
	add_definitions(${GTK3_CFLAGS_OTHER})
	target_include_directories(Std++ PRIVATE ${GTK3_INCLUDE_DIRS})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${GTK3_LIBRARIES})

	#OpenGL3Core extension
	if(${BACKEND_OPENGL})
		set(SOURCE_FILES
			${SOURCE_FILES}

			#Rendering
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/GtkOpenGLDeviceContext.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/Rendering/GtkOpenGLDeviceContext.hpp

			#.
			${CMAKE_CURRENT_SOURCE_DIR}/Gtk3OpenGLBackend.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/Gtk3OpenGLBackend.hpp
			)
	endif ()

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()