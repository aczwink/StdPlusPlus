/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/WidgetBackend.hpp>
#include <Std++/UI/Displays/RenderTargetWidget.hpp>

namespace _stdxx_
{
	//Forward declarations
	class XcbWindow;
	class XConnection;

	class XcbXlibRenderTargetWidgetBackend : public WidgetBackend
	{
	public:
		//Constructor
		XcbXlibRenderTargetWidgetBackend(StdXX::UIBackend& uiBackend, XConnection& xConnection, StdXX::UI::RenderTargetWidget& renderTargetWidget);

		//Methods
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void Show(bool visible) override;

		//NOT IMPLEMENTED
		StdXX::Math::SizeD GetSizeHint() const override;
		void IgnoreEvent() override;
		void Repaint() override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;
		void SetEditable(bool enable) const override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String &text) override;
		//END OF NOT IMPLEMENTED

	private:
		//Members
		XConnection& xConnection;
		StdXX::UI::RenderTargetWidget& renderTargetWidget;
		XcbWindow* xcbWindow;

		//Methods
		int SelectVisual() const;
	};
}