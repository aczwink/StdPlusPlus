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
#include "../_Backends/WindowBackend.hpp"
#include "../Function.hpp"
#include "../Containers/Strings/OldString.hpp"
#include "../Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "../Math/Geometry/Rect.hpp"
#include "../Math/Size.hpp"
#include "Mouse.hpp"
#include "SizingPolicy.hpp"
#include "ERenderMode.h"

namespace StdPlusPlus
{
	namespace Rendering
	{
		//Move declarations
		class DeviceContext;
	}

    namespace UI
    {
        //Move declarations
        class WidgetContainer;
        class Window;

        class STDPLUSPLUS_API Widget
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
				if(this->backend)
					return this->backend->GetSize();
				return this->bounds.size;
			}

            inline const Window *GetWindow() const
            {
                if(this->pOwner)
                    return this->pOwner;

                //this is a window itself
                return (Window *)this;
            }

			inline void Repaint()
			{
				this->backend->Repaint();
			}

			inline void SetBounds(const Rect &area)
			{
				this->bounds = area;

				if(this->backend)
					this->backend->SetBounds(area);
				else
					this->OnResized();
			}

			inline void SetEnabled(bool enable = true)
			{
				this->backend->SetEnabled(enable);
			}

			inline void SetHint(const String &text)
			{
				this->backend->SetHint(text);
			}

			inline void Show(bool visible = true)
			{
				this->backend->Show(visible);
			}

		protected:
			//Members
			_stdpp::WindowBackend *backend;

			//Methods
			ERenderMode GetRenderMode() const;

			//Inline
			inline _stdpp::WindowBackend *GetParentBackend() const
			{
				if(this->backend)
					return this->backend;
				return ((Widget *)this->parent)->GetParentBackend();
			}

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
        };
    }
}