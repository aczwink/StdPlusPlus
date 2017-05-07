set(SOURCE_FILES
        ${SOURCE_FILES}

        #.
        ${CMAKE_CURRENT_SOURCE_DIR}/__InitAndShutdown.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp

	#Filesystem
	${CMAKE_CURRENT_SOURCE_DIR}/Filesystem/Path.cpp

        #Math
	${CMAKE_CURRENT_SOURCE_DIR}/Math/Size.cpp

        #Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering/DeviceContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGL.cpp

        #Streams
	${CMAKE_CURRENT_SOURCE_DIR}/Streams/FileInputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Streams/CFileOutputStream.cpp

        #UI
	${CMAKE_CURRENT_SOURCE_DIR}/UI/_RedirectGtkContainer.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/_RedirectGtkContainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Widget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/WidgetContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/EventQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk.h
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Gtk.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/GtkEventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/CommonDialogs/OpenFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Containers/GroupBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/CheckBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Label.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/PushButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/RadioButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/Slider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Controls/SpinBox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Menu/CMenuBar.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Render\ Targets/CDefaultTerminalRenderTarget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/CTreeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UI/Views/RenderTargetWidget.cpp

        PARENT_SCOPE)