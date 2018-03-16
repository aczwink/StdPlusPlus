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
			virtual Rect GetChildrenRect() const;
            virtual ERenderMode GetChildrenRenderMode() const;
            virtual Size GetSizeHint() const;

            //Inline
            inline const LinkedList<Widget *> &GetChildren() const
            {
                return this->children;
            }

            inline ERenderMode GetRenderMode() const
            {
                return this->renderMode;
            }

        protected:
            //Members
            ILayout *layout;
            ERenderMode renderMode;

            //Eventhandlers
            virtual void OnPaint();

        private:
            //Members
            LinkedList<Widget *> children;

            //Eventhandlers
            virtual void OnResized();

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