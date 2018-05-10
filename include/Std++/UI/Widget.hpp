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

namespace StdPlusPlus
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

        class STDPLUSPLUS_API Widget
        {
            friend class UIEventSource;
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
			Point TranslateToAncestorCoords(const Point &point, const WidgetContainer *ancestor) const;

            //Overrideable
            virtual Size GetSizeHint() const;

            //Inline
			inline const Rect &GetBounds() const
			{
				return this->bounds;
			}

			/**
			 * Returns a rectangle that describes the area of this widget locally (i.e. without considering parents).
			 * In its own coordinate system, this widget always has the origin (0, 0).
			 * @return
			 */
			inline Rect GetLocalBounds() const
			{
				return Rect(Point(), this->GetSize());
			}

            inline const WidgetContainer *GetParent() const
            {
                return this->parent;
            }

			inline Size GetSize() const
			{
				return this->bounds.size;
			}

            inline const Window *GetWindow() const
            {
                if(this->owner)
                    return this->owner;

                //this is a window itself
                return (Window *)this;
            }

			inline void Repaint()
			{
				this->backend->Repaint();
			}

			inline void SetBounds(const Rect &area)
			{
				this->OnResizing(area);
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

			inline Point TranslateToOwnerCoords(const Point &point) const
			{
				return this->TranslateToAncestorCoords(point, reinterpret_cast<const WidgetContainer *>(this->owner));
			}

			inline Point TranslateToParentCoords(const Point &point) const
			{
				return this->bounds.origin + point;
			}

		protected:
			//Members
			_stdpp::WindowBackend *backend;

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
			Window *owner;
			/**
			 * Defines a rectangular area of this widget within the children rectangle of its parent.
			 * The bounds are always relative to the parents child area coordinates.
			 */
			Rect bounds;

			//Eventhandlers
			virtual void OnMouseButtonPressed(MouseButton button, const Point &pos);
			virtual void OnMouseButtonReleased(MouseButton button, const Point &pos);
			virtual void OnMouseMoved(const Point &pos);
			virtual void OnMouseWheelTurned(int16 delta);
			virtual void OnPaint();
			virtual void OnResized();
			virtual void OnResizing(const Rect &newBounds);
        };
    }
}