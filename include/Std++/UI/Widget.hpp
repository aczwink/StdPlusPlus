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
#include "Std++/_Backends/UI/WidgetBackend.hpp"
#include "../Function.hpp"
#include "../Containers/Strings/OldString.hpp"
#include "../Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "../Math/Geometry/Rect.hpp"
#include "../Math/Size.hpp"
#include "Mouse.hpp"
#include "SizingPolicy.hpp"

namespace StdXX
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
            inline Widget() : parent(nullptr), backend(nullptr)
			{
			}

            //Destructor
            virtual ~Widget();

            //Methods
			Math::PointD TranslateToAncestorCoords(const Math::PointD &point, const WidgetContainer *ancestor) const;

            //Overrideable
            virtual Math::SizeD GetSizeHint() const;

            //Inline
			inline _stdxx_::WidgetBackend *_GetBackend()
			{
				return this->backend;
			}

			inline const Math::RectD &GetBounds() const
			{
				return this->bounds;
			}

			/**
			 * Returns a rectangle that describes the area of this widget locally (i.e. without considering parents).
			 * In its own coordinate system, this widget always has the origin (0, 0).
			 * @return
			 */
			inline Math::RectD GetLocalBounds() const
			{
				return Math::RectD(Math::PointD(), this->GetSize());
			}

            inline const WidgetContainer *GetParent() const
            {
                return this->parent;
            }

			inline Math::SizeD GetSize() const
			{
				return this->bounds.size;
			}

			inline void Repaint()
			{
				this->backend->Repaint();
			}

			inline void SetBounds(const Math::RectD &newBounds)
			{
				if(this->backend)
					this->backend->SetBounds(newBounds);
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

			inline Math::PointD TranslateToParentCoords(const Math::PointD &point) const
			{
				return this->bounds.origin + point;
			}

		protected:
			//Members
			_stdxx_::WidgetBackend *backend;

			//Inline
			inline void IgnoreEvent()
			{
				this->backend->IgnoreEvent();
			}

		private:
			//Members
			WidgetContainer *parent;
			/**
			 * Defines a rectangular area of this widget within the rectangle of its parent.
			 * The bounds are always relative to the parents coordinate system.
			 */
			Math::RectD bounds;

			//Eventhandlers
			virtual void OnMouseButtonPressed(MouseButton button, const Math::PointD &pos);
			virtual void OnMouseButtonReleased(MouseButton button, const Math::PointD &pos);
			virtual void OnMouseMoved(const Math::PointD &pos);
			virtual void OnMouseWheelTurned(int16 delta);
			virtual void OnPaint();
			virtual void OnResized();
			virtual void OnResizing(const Math::RectD &newBounds);
        };
    }
}