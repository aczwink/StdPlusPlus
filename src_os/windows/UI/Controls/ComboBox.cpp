/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/UI/Views/SelectBox.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

/*
uint32 ComboBox::GetNumberOfItems() const
{
return (uint32)SendMessage((HWND)this->systemHandle, CB_GETCOUNT, 0, 0);
}

uint16 ComboBox::GetSelectedIndex() const
{
int32 selection;

selection = SendMessage((HWND)this->systemHandle, CB_GETCURSEL, 0, 0);
if(selection == CB_ERR)
return UINT16_MAX;

return (uint16)selection;
}
*/