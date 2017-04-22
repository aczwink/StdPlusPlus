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
#include "../CFunction.h"
#include "../Containers/Strings/CString.h"
#include "../Containers/Strings/UTF-8/CUTF8String.h"
#include "../Definitions.h"
#include "../Math/Geometry/CRect.h"
#include "../Math/CSize.h"
#include "CSizingPolicy.h"
#include "ERenderMode.h"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class AWidgetContainer;
        class Window;

        class ACSTDLIB_API AWidget
        {
            friend class AWidgetContainer;
            friend class CEventQueue;
        private:
            //Members
            AWidgetContainer *pParent;
            Window *pOwner;
            CRect bounds;

            //Eventhandlers
            virtual void OnPaint();
            virtual void OnResized();

            //Methods
            CSize GetOSSize() const;

        protected:
            //Members
            void *pOSHandle;

            //Methods
            ERenderMode GetRenderMode() const;

            //Functions
            static void IgnoreEvent();

        public:
            //Members
            CSizingPolicy sizingPolicy;

            //Constructor
            AWidget(AWidgetContainer *pParent);

            //Destructor
            virtual ~AWidget() {}

            //Methods
            CSize GetSize() const;
            void Repaint();
            void SetEnabled(bool enable = true);
            void SetRect(const CRect &refArea);
            void Show(bool visible = true);
            CPoint TransformToWindow(const CPoint &refPoint) const;

            //Overrideable
            virtual CSize GetSizeHint() const;

            //Inline
            inline const AWidgetContainer *GetParent() const
            {
                return this->pParent;
            }

            inline const Window *GetWindow() const
            {
                if(this->pOwner)
                    return this->pOwner;

                //this is a window itself
                return (Window *)this;
            }
        };
    }
}