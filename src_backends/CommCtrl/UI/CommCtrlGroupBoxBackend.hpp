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
#include <Std++/_Backends/UI/GroupBoxBackend.hpp>
#include <Std++/UI/Containers/GroupBox.hpp>
#include "CommCtrlContainerBackend.hpp"
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	https://msdn.microsoft.com/en-us/library/windows/desktop/bb775943(v=vs.85).aspx
	*/
	class CommCtrlGroupBoxBackend : public GroupBoxBackend, virtual public CommCtrlContainerBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlGroupBoxBackend(StdXX::UIBackend *uiBackend, StdXX::UI::GroupBox *groupBox)
			: GroupBoxBackend(uiBackend), CommCtrlContainerBackend(uiBackend, groupBox), WidgetContainerBackend(uiBackend),
			CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend),
			Win32Window(*this, WC_BUTTONW, WS_CLIPSIBLINGS | BS_GROUPBOX, WS_EX_TRANSPARENT),
			groupBox(groupBox)
		{
		}

		//Overrideable
		virtual void Reparent(Win32Window *newParent);

		//Methods
		_stdxx_::WidgetContainerBackend * CreateContentAreaBackend(StdXX::UI::CompositeWidget & widget) override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		uint32 GetPosition() const override;
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void Repaint() override;
		void ResetView() const override;
		void Select(StdXX::UI::ControllerIndex &controllerIndex) const override;
		void SetBounds(const StdXX::Math::RectD & area) override;
		void SetEditable(bool enable) const override;
		void SetHint(const StdXX::String &text) const override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetTitle(const StdXX::String &title) override;
		void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;
		void UpdateSelection(StdXX::UI::SelectionController &selectionController) const override;

	private:
		//Members
		StdXX::UI::GroupBox *groupBox;
	};
}