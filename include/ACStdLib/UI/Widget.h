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
#include "ACStdLib/Function.hpp"
#include "ACStdLib/Containers/Strings/String.h"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "ACStdLib/Math/Geometry/Rect.h"
#include "ACStdLib/Math/Size.h"
#include "SizingPolicy.hpp"
#include "ERenderMode.h"

namespace ACStdLib
{
	namespace Rendering
	{
		//Forward declarations
		class DeviceContext;
	}

    namespace UI
    {
        //Forward declarations
        class WidgetContainer;
        class Window;

        class ACSTDLIB_API Widget
        {
            friend class EventQueue;
			friend class Rendering::DeviceContext;
            friend class WidgetContainer;
        private:
            //Members
            WidgetContainer *pParent;
            Window *pOwner;
            Rect bounds;

            //Eventhandlers
            virtual void OnPaint();
            virtual void OnResized();

            //Methods
            Size System_GetSize() const;
			void System_SetRect(const Rect &area);

        protected:
            //Members
            void *systemHandle;

            //Methods
            ERenderMode GetRenderMode() const;

            //Functions
            static void IgnoreEvent();

        public:
            //Members
            SizingPolicy sizingPolicy;

            //Constructor
            Widget(WidgetContainer *pParent);

            //Destructor
            virtual ~Widget() {}

            //Methods
            void Repaint();
            void SetEnabled(bool enable = true);
            void SetRect(const Rect &area);
            void Show(bool visible = true);
            CPoint TransformToWindow(const CPoint &refPoint) const;

            //Overrideable
            virtual Size GetSizeHint() const;

            //Inline
			inline const Rect &GetBounds() const
			{
				return this->bounds;
			}

            inline const WidgetContainer *GetParent() const
            {
                return this->pParent;
            }

			inline Size GetSize() const
			{
				if(this->systemHandle == nullptr)
					return this->bounds.size;

				return this->System_GetSize();
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