/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/UI/UIEventSource.hpp>

//Global
#undef new
#include <Cocoa/Cocoa.h>

namespace _stdxx_
{
	class CocoaEventSource : public StdXX::UI::UIEventSource
	{
	public:
		//Constructor
		CocoaEventSource();

		//Methods
		void DispatchPendingEvents() override;
		uint64 GetMaxTimeout() const override;
		void VisitWaitObjects(const StdXX::Function<void(WaitObjHandle, bool)> &visitFunc) const override;

		//Class function
		static void EmitCloseEvent(StdXX::UI::Window &window);
		static void EmitMouseMovedEvent(StdXX::UI::Widget &widget, NSPoint point);
		static void EmitMousePressedEvent(StdXX::UI::Widget &widget, StdXX::UI::MouseButton button, NSPoint point);
		static void EmitMouseReleasedEvent(StdXX::UI::Widget &widget, StdXX::UI::MouseButton button, NSPoint point);
		static void EmitMouseWheelEvent(StdXX::UI::Widget &widget, float64 delta);
		static void EmitPaintEvent(StdXX::UI::Widget &widget);
		static void EmitResizedEvent(StdXX::UI::Widget &widget);
		static void EmitResizingEvent(StdXX::UI::Widget &widget, const StdXX::Math::RectD &newBounds);
	};
}