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
//Global
#undef new
#include <Cocoa/Cocoa.h>
#include <Std++/_Backends/UI/WidgetBackend.hpp>

namespace _stdxx_
{
	class CocoaWidgetBackend : virtual public WidgetBackend
	{
	public:
		//Constructor
		inline CocoaWidgetBackend(StdXX::UIBackend& uiBackend) : WidgetBackend(uiBackend)
		{
		}

		//Abstract
		virtual NSView *GetView() = 0;

		//Methods
		void IgnoreEvent() override;
		virtual void SetBounds(const StdXX::Math::RectD &area) override;
		virtual void SetEnabled(bool enable) override;
		virtual void SetHint(const StdXX::String &text) override;
		virtual void Show(bool visible) override;
	};
}