/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "CommCtrlContainerBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
void CommCtrlContainerBackend::AddChild(StdXX::UI::Widget *widget)
{
	if (this->container->GetParent())
	{
		CommCtrlContainerBackend *parentBackend = dynamic_cast<CommCtrlContainerBackend *>(this->container->GetParent()->_GetBackend());
		if(parentBackend)
			parentBackend->AddChild(widget);
	}
}

SizeD CommCtrlContainerBackend::GetSizeHint() const
{
	//no requirements here
	return StdXX::Math::SizeD();
}

Widget &CommCtrlContainerBackend::GetWidget()
{
	return *this->container;
}

const Widget &CommCtrlContainerBackend::GetWidget() const
{
	return *this->container;
}

void CommCtrlContainerBackend::IgnoreEvent()
{
	//do nothing
}

void _stdxx_::CommCtrlContainerBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlContainerBackend::Reparent(Win32Window *newParent)
{
	//we dont have hwnds but our children might have
	for (uint32 i = 0; i < this->container->GetNumberOfChildren(); i++)
	{
		CommCtrlWidgetBackend *commCtrlWidgetBackend = dynamic_cast<CommCtrlWidgetBackend *>(this->container->GetChild(i)->_GetBackend());
		commCtrlWidgetBackend->Reparent(newParent);
	}
}

void _stdxx_::CommCtrlContainerBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlContainerBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlContainerBackend::SetEnabled(bool enable)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlContainerBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlContainerBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlContainerBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}