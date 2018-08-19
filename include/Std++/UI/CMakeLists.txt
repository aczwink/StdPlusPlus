add_subdirectory(Events)

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

	#Layouts
	${CMAKE_CURRENT_SOURCE_DIR}/Layouts/GridLayout.hpp

	#Menu
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/Menu.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/MenuBar.hpp

	#Views
	${CMAKE_CURRENT_SOURCE_DIR}/Views/ComboBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/ListView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/TableView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/TreeView.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/View.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Action.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/CommonDialogs.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Keyboard.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MainAppWindow.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mouse.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SizingPolicy.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIEventSource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Widget.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/WidgetContainer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Window.hpp

	PARENT_SCOPE)