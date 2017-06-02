/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "ACStdLib/Containers/LinkedList/LinkedList.hpp"
#include "Widget.hpp"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class ILayout;

        class ACSTDLIB_API WidgetContainer : public Widget
        {
            friend class Widget;
            friend class EventQueue;
            friend class Window;
        private:
            //Members
            LinkedList<Widget *> children;

            //Eventhandlers
            virtual void OnResized();

        protected:
            //Members
            ILayout *layout;
            ERenderMode renderMode;

            //Eventhandlers
            virtual void OnPaint();

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
        };
    }
}