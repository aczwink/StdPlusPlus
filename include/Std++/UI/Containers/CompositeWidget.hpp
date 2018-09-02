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
#pragma once
//Local
#include "Std++/Containers/LinkedList/LinkedList.hpp"
#include "Std++/UI/WidgetContainer.hpp"

namespace StdXX
{
    namespace UI
    {
        //Forward declarations
        class ILayout;

        class STDPLUSPLUS_API CompositeWidget : public WidgetContainer
        {
            friend class Widget;
            friend class EventQueue;
            friend class Window;
        public:
            //Constructor
            CompositeWidget();

            //Destructor
            virtual ~CompositeWidget();

            //Methods
			Widget *GetChild(uint32 index);
			uint32 GetNumberOfChildren() const;
            void SetLayout(ILayout *pLayout);

            //Overrideable
            virtual Math::SizeD GetSizeHint() const;

            //Inline
			inline void AddChild(Widget *widget)
			{
				this->TakeWidgetOwnership(widget);
				this->children.InsertTail(widget);
			}

            inline const LinkedList<Widget *> &GetChildren() const
            {
                return this->children;
            }

        protected:
            //Members
            ILayout *layout;

            //Eventhandlers
            virtual void OnPaint();
			virtual void OnResized();

        private:
            //Members
            LinkedList<Widget *> children;

            //Inline
            inline void RemoveChild(Widget *child)
            {
                int32 index = this->children.Find(child);
                if(index != -1)
                    this->children.Remove(index);
            }
        };
    }
}