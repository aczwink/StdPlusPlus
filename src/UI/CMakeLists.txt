set(SOURCE_FILES
	${SOURCE_FILES}

	#Containers
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/GroupBox.cpp

	#Controllers
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/ControllerIndex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/ListController.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controllers/TableController.cpp

	#Controls
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/CheckBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/Label.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/LineEdit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/PushButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/RadioButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/SearchBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/Slider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/SpinBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Controls/TextEdit.cpp

	#Displays
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/PathRenderTargetWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/RenderTargetWidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Displays/VideoWidget.cpp

	#Layouts
	${CMAKE_CURRENT_SOURCE_DIR}/Layouts/GridLayout.cpp

	#Menu
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/CMenu.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Menu/CMenuBar.cpp

	#Render Targets
	${CMAKE_CURRENT_SOURCE_DIR}/Render\ Targets/ATextRenderTarget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Render\ Targets/CDesktopRenderTarget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Render\ Targets/IRenderTarget.cpp

	#Views
	${CMAKE_CURRENT_SOURCE_DIR}/Views/ComboBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/ListView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/TableView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/TreeView.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Views/View.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/MainAppWindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SizingPolicy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Widget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WidgetContainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp

	PARENT_SCOPE)