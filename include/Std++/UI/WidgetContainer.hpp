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
#include "../Containers/LinkedList/LinkedList.hpp"
#include "Widget.hpp"

namespace StdPlusPlus
{
    namespace UI
    {
        //Move declarations
        class ILayout;

        class STDPLUSPLUS_API WidgetContainer : public Widget
        {
            friend class Widget;
            friend class EventQueue;
            friend class Window;
        public:
            //Constructor
            WidgetContainer(WidgetContainer *pContainer);

            //Destructor
            virtual ~WidgetContainer();

            //Methods
            void SetLayout(ILayout *pLayout);

            //Overrideable
            /**
             * The rectangle in which the children widgets are placed.
             * It is relative to this widgets area (i.e. Rect(Point(0, 0), this->GetSize()).
             * It must lay inside this widgets area or can be exactly this widgets area.
             * @return
             */
			virtual Rect GetChildrenRect() const;
            virtual Size GetSizeHint() const;

            //Inline
            inline const LinkedList<Widget *> &GetChildren() const
            {
                return this->children;
            }

			inline Point TranslateChildToWidgetCoords(const Point &point) const
			{
				return this->GetChildrenRect().origin + point;
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