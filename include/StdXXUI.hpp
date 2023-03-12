/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */

//UI
#include <Std++/UI/Action.hpp>
#include <Std++/UI/CommonDialogs.hpp>
#include <Std++/UI/CTerminalEventQueue.h>
#include <Std++/UI/Keyboard.hpp>
#include <Std++/UI/MainAppWindow.hpp>
#include <Std++/UI/Mouse.hpp>
//Containers
#include <Std++/UI/Containers/TabContainer.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include <Std++/UI/Containers/GroupBox.hpp>
#include <Std++/UI/Containers/ScrollArea.hpp>
//Controllers
#include <Std++/UI/Controllers/ACheckListController.h>
#include <Std++/UI/Controllers/ListController.hpp>
#include <Std++/UI/Controllers/TableController.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
//Controls
#include <Std++/UI/Controls/CheckBox.hpp>
#include <Std++/UI/Views/SelectBox.hpp>
#include <Std++/UI/Controls/Label.hpp>
#include <Std++/UI/Controls/LineEdit.hpp>
#include <Std++/UI/Controls/ProgressBar.hpp>
#include <Std++/UI/Controls/PushButton.hpp>
#include <Std++/UI/Controls/RadioButton.hpp>
#include <Std++/UI/Controls/SearchBox.hpp>
#include <Std++/UI/Controls/Slider.hpp>
#include <Std++/UI/Controls/SpinBox.hpp>
#include <Std++/UI/Controls/TextEdit.hpp>
//Displays
#include <Std++/UI/Displays/VideoWidget.hpp>
//Events
#include <Std++/UI/Events/KeyEvent.hpp>
//Layouts
#include <Std++/UI/Layouts/GridLayout.hpp>
//Menu
#include <Std++/UI/Menu/Menu.hpp>
#include <Std++/UI/Menu/MenuBar.hpp>
//Transfer
#include <Std++/UI/Transfer/CFileTransfer.h>
#include <Std++/UI/Transfer/Clipboard.hpp>
//Views
#include <Std++/UI/Displays/PathRenderTargetWidget.hpp>
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include <Std++/UI/Views/CCheckListView.h>
#include <Std++/UI/Views/ListView.hpp>
#include <Std++/UI/Views/TableView.hpp>
#include <Std++/UI/Views/TreeView.hpp>