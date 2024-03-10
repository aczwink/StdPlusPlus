/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Containers/TabContainer.hpp>
//Local
#include <Std++/_Backends/UI/TabContainerBackend.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Destructor
TabContainer::~TabContainer()
{
    while(!this->tabs.IsEmpty())
        delete this->tabs.GetFront().content;
}

//Public methods
void TabContainer::AddTab(const String &label, Widget *content)
{
    this->tabs.InsertTail({ label, content });
    this->TakeWidgetOwnership(content);
}

Widget *TabContainer::GetChild(uint32 index)
{
    return this->tabs.Get(index).content;
}

const Widget *TabContainer::GetChild(uint32 index) const
{
    return this->tabs.Get(index).content;
}

uint32 TabContainer::GetNumberOfChildren() const
{
    return this->tabs.GetNumberOfElements();
}

void TabContainer::RemoveChild(Widget* child)
{
    for(auto it = this->tabs.begin(); it != this->tabs.end(); ++it)
    {
        if((*it).content == child)
        {
            it.Remove();
            this->FreeWidgetOwnership(child);
            return;
        }
    }

    NOT_IMPLEMENTED_ERROR; //TODO: implement case: child not found
}

//Protected methods
void TabContainer::OnRealized()
{
    WidgetContainer::OnRealized();

    uint32 index = 0;
    for(const auto& tab : this->tabs)
    {
        this->tabContainerBackend->SetLabel(index++, tab.label);
    }
}

//Private methods
void TabContainer::RealizeSelf()
{
    _stdxx_::TabContainerBackend* tabContainerBackend = this->_GetUIBackend()->CreateTabContainerBackend(*this);
    this->_SetBackend(tabContainerBackend);
}