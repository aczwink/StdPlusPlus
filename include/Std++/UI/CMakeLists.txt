add_subdirectory(Events)
add_subdirectory(Style)
add_subdirectory(Views)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Containers
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/CompositeWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/ContentAreaWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/GroupBox.hpp

	#Controllers
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/ControllerIndex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/ListController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/SelectionController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/TableController.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/TreeController.hpp

	#Controls
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/CheckBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/Label.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/LineEdit.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/ProgressBar.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/PushButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/RadioButton.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/SearchBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/Slider.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/SpinBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/TextEdit.hpp

	#Displays
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/PathRenderTargetWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/RenderTargetWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/VideoWidget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/WidgetFrameBufferSetup.hpp

	#Layouts
	${CMAKE_CURRENT_SOURCE_DIR}/Layouts/GridLayout.hpp

	#Menu
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/Menu.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/MenuBar.hpp

	#Transfer
	${CMAKE_CURRENT_SOURCE_DIR}/Transfer/Clipboard.hpp

	#Windows
	${CMAKE_CURRENT_SOURCE_DIR}/Windows/Dialog.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Windows/MainAppWindow.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Windows/Window.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Action.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/CommonDialogs.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mouse.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Painter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Property.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Signal.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SizingPolicy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIEventSource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Widget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/WidgetContainer.hpp

	PARENT_SCOPE)