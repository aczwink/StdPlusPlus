/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/UI/WidgetContainer.hpp>
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/_Backends/UI/TabContainerBackend.hpp>

namespace StdXX::UI
{
    class TabContainer : public WidgetContainer
    {
        struct Tab
        {
            String label;
            Widget* content;
        };
    public:
        //Destructor
        virtual ~TabContainer();

        //Methods
        void AddTab(const String& label, Widget* content);
        Widget *GetChild(uint32 index) override;
        const Widget *GetChild(uint32 index) const override;
        uint32 GetNumberOfChildren() const override;
        void RemoveChild(Widget *child) override;

    protected:
        //Eventhandlers
        void OnRealized() override;

    private:
        //Members
        LinkedList<Tab> tabs;
        _stdxx_::TabContainerBackend *tabContainerBackend;

        //Methods
        void RealizeSelf() override;

        //Inline
        inline void _SetBackend(_stdxx_::TabContainerBackend *tabContainerBackend)
        {
            WidgetContainer::_SetBackend(tabContainerBackend);
            this->tabContainerBackend = tabContainerBackend;
        }
    };
}