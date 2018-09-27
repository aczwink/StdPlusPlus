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
#include <Std++/_Backends/UI/SliderBackend.hpp>
#include <Std++/UI/Controls/Slider.hpp>
#include "CommCtrlWidgetBackend.hpp"
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI Documentation: https://docs.microsoft.com/de-de/windows/desktop/Controls/trackbar-control-reference
	*/
	class CommCtrlSliderBackend : public SliderBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlSliderBackend(StdXX::UIBackend *uiBackend, StdXX::UI::Slider *slider)
			: SliderBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, TRACKBAR_CLASSW, TBS_HORZ | TBS_TRANSPARENTBKGND),
			slider(slider)
		{
		}

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void OnMessage(WinMessageEvent& event) override;
		void SetPosition(uint32 pos) override;
		void SetRange(uint32 min, uint32 max) override;

		//not implemeneted
		void Repaint() override;
		void Select(StdXX::UI::ControllerIndex & controllerIndex) const override;
		void SetEditable(bool enable) const override;
		void SetHint(const StdXX::String & text) const override;
		void UpdateSelection(StdXX::UI::SelectionController & selectionController) const override;
		void ResetView() const override;

	private:
		//Members
		StdXX::UI::Slider *slider;

		//Inline
		inline uint32 GetPosition() const
		{
			return this->SendMessage(TBM_GETPOS, 0, 0);
		}
	};
}