/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/WidgetBackend.hpp>
#include <Std++/UI/Events/MouseEvents.hpp>
#include <Std++/UI/Events/PaintEvent.hpp>
#include <Std++/UI/Style/StyleContext.hpp>
#include <Std++/UI/Style/StyleSheet.hpp>
#include <Std++/UI/Style/StyleProperties.hpp>
#include <Std++/Function.hpp>
#include "../Definitions.h"
#include "../Math/Geometry/Rect.hpp"
#include "../Math/Size.hpp"
#include "Events/Event.hpp"
#include "Mouse.hpp"
#include "SizingPolicy.hpp"
#include "Signal.hpp"

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
            inline Widget() : parent(nullptr), visible(true), enabled(true), backend(nullptr), isRealized(false), isStyleQueryUpToDate(false)
			{
			}

            //Destructor
            virtual ~Widget();

			//Properties
			inline const UI::StyleContext& StyleContext() const
			{
				return this->styleContext;
			}

            //Methods
			const WidgetContainer* _GetParentWithBackend() const;
			Window *GetWindow();
			const Window *GetWindow() const;
			void SetBounds(const Math::RectD &newBounds);
			Math::PointD TranslateToAncestorCoords(const Math::PointD &point, const WidgetContainer *ancestor) const;

            //Overrideable
			virtual void Event(Event& event);
            virtual Math::SizeD GetSizeHint() const;

            //Inline
			inline _stdxx_::WidgetBackend *_GetBackend()
			{
				return this->backend;
			}

			inline const _stdxx_::WidgetBackend *_GetBackend() const
			{
				return this->backend;
			}

			inline bool _IsRealized() const
			{
				return this->isRealized;
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

			inline WidgetContainer *GetParent()
			{
				return this->parent;
			}

            inline const WidgetContainer *GetParent() const
            {
                return this->parent;
            }

			inline Math::SizeD GetSize() const
			{
				return this->bounds.size;
			}

			inline void Move(const Math::PointD &newOrigin)
			{
				if (this->bounds.origin != newOrigin)
				{
					this->bounds.origin = newOrigin;
					this->backend->SetBounds(this->bounds);
					this->OnMoved();
				}
			}

			inline void Repaint()
			{
				this->backend->Repaint();
			}

			inline void Resize(const Math::SizeD &newSize)
			{
				if (this->bounds.size != newSize)
				{
					this->bounds.size = newSize;
					this->backend->SetBounds(this->bounds);
					this->OnResized();
				}
			}

			inline void SetEnabled(bool enable = true)
			{
				this->enabled = enable;
				if(this->backend)
					this->backend->SetEnabled(enable);
			}

			inline void SetHint(const String &text)
			{
				this->hint = text;
				if(this->backend)
					this->backend->SetHint(text);
			}

			inline void Show(bool visible = true)
			{
				this->visible = visible;

				if(visible && !this->backend)
					this->EnsureRealized();

				if(this->backend)
					this->backend->Show(visible);
			}

			inline Math::PointD TranslateToParentCoords(const Math::PointD &point) const
			{
				return this->bounds.origin + point;
			}

		protected:
			//Members
			UI::StyleContext styleContext;

			//Properties
			inline const UI::StyleProperties& StyleProperties()
			{
				if (!this->isStyleQueryUpToDate)
				{
					this->styleQuery = StyleSheet::Global().Query(*this);
					this->isStyleQueryUpToDate = true;
				}
				return this->styleQuery;
			}

			//Event handlers
			virtual void OnRealized();

			//Methods
			UIBackend * _GetUIBackend();

			//Inline
			inline void EnsureRealized()
			{
				ASSERT(this->CanRealize(), u8"REPORT THIS PLEASE!");
				this->Realize();
			}

			inline void _SetBackend(_stdxx_::WidgetBackend* widgetBackend)
			{
				this->backend = widgetBackend;
			}

		private:
			//Members
			bool isStyleQueryUpToDate;
			UI::StyleProperties styleQuery;
			WidgetContainer *parent;
			/**
			 * Defines a rectangular area of this widget within the rectangle of its parent.
			 * The bounds are always relative to the parents coordinate system.
			 */
			Math::RectD bounds;
			bool visible;
			bool enabled;
			_stdxx_::WidgetBackend *backend;
			bool isRealized;
			String hint;

			//Methods
			/**
			 * Pre-condition: CanRealize()
			 */
			void Realize();

			//Overrideable
			virtual bool CanRealize() const;
			virtual void RealizeSelf();

			//Eventhandlers
			virtual void OnMouseButtonPressed(MouseClickEvent &event);
			virtual void OnMouseButtonReleased(MouseClickEvent &event);
			virtual void OnMouseMoved(MouseEvent& event);
			virtual void OnMouseWheelRolled(MouseWheelEvent& event);
			virtual void OnMoved();
			virtual void OnPaint(PaintEvent& event);
			virtual void OnResized();
        };
    }
}