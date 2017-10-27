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
#include "ACStdLib/Containers/Strings/String.hpp"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "ACStdLib/Math/Geometry/Rect.hpp"
#include "ACStdLib/Math/Size.hpp"
#include "Mouse.hpp"
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
        public:
            //Members
            SizingPolicy sizingPolicy;

            //Constructor
            Widget(WidgetContainer *parent);

            //Destructor
            virtual ~Widget();

            //Methods
            void Repaint();
            void SetEnabled(bool enable = true);
            void SetRect(const Rect &area);
            void Show(bool visible = true);
            Point TransformToWindow(const Point &refPoint) const;

            //Overrideable
            virtual Size GetSizeHint() const;

            //Inline
			inline const Rect &GetBounds() const
			{
				return this->bounds;
			}

            inline const WidgetContainer *GetParent() const
            {
                return this->parent;
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

		protected:
			//Members
			void *systemHandle;

			//Methods
			ERenderMode GetRenderMode() const;

			//Functions
			static void IgnoreEvent();

		private:
			//Members
			WidgetContainer *parent;
			Window *pOwner;
			Rect bounds;

			//Eventhandlers
			virtual void OnMouseButtonPressed(MouseButton button, const Point &pos);
			virtual void OnMouseButtonReleased(MouseButton button, const Point &pos);
			virtual void OnMouseMoved(const Point &pos);
			virtual void OnMouseWheelTurned(int16 delta);
			virtual void OnPaint();
			virtual void OnResized();

			//Methods
			Size System_GetSize() const;
			void System_SetRect(const Rect &area);
        };
    }
}