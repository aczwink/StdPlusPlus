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
#include "../Containers/CLinkedList/CLinkedList.h"
#include "AWidget.h"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class ILayout;

        class ACSTDLIB_API AWidgetContainer : public AWidget
        {
            friend class AWidget;
            friend class EventQueue;
            friend class Window;
        private:
            //Members
            CLinkedList<AWidget *> children;
            ILayout *pLayout;

            //Eventhandlers
            virtual void OnResized();

        protected:
            //Members
            ERenderMode renderMode;

            //Eventhandlers
            virtual void OnPaint();

        public:
            //Constructor
            AWidgetContainer(AWidgetContainer *pContainer);

            //Destructor
            virtual ~AWidgetContainer();

            //Methods
            void SetLayout(ILayout *pLayout);

            //Overrideable
            virtual CRect GetChildrenRect() const;
            virtual ERenderMode GetChildrenRenderMode() const;
            virtual CSize GetSizeHint() const;

            //Inline
            inline const CLinkedList<AWidget *> &GetChildren() const
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