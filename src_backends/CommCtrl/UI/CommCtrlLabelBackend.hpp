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
#include <Std++/_Backends/UI/LabelBackend.hpp>
#include <Std++/UI/Controls/Label.hpp>
#include "CommCtrlWidgetBackend.hpp"
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI Documentation:
	Label: https://msdn.microsoft.com/en-us/library/windows/desktop/bb760769(v=vs.85).aspx
	*/
	class CommCtrlLabelBackend : public LabelBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlLabelBackend(StdXX::UIBackend *uiBackend, StdXX::UI::Label *label)
			: LabelBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, WC_STATICW),
			label(label)
		{
		}

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void SetText(const StdXX::String & text) override;

		//not implemented:
		void Repaint() override;
		void Select(StdXX::UI::ControllerIndex & controllerIndex) const override;
		void SetEditable(bool enable) const override;
		void SetHint(const StdXX::String & text) const override;
		void Show(bool visible) override;
		void ShowInformationBox(const StdXX::String & title, const StdXX::String & message) const override;
		void UpdateSelection(StdXX::UI::SelectionController & selectionController) const override;
		uint32 GetPosition() const override;
		void ResetView() const override;
		void SetMenuBar(StdXX::UI::MenuBar * menuBar, MenuBarBackend * menuBarBackend) override;

	private:
		//Members
		StdXX::UI::Label *label;
	};
}