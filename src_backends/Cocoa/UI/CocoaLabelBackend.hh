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
//Local
#include <Std++/_Backends/UI/LabelBackend.hpp>
#import "CocoaView.hh"

namespace _stdxx_
{
	class CocoaLabelBackend : public LabelBackend, public CocoaView
	{
	public:
		//Constructor
		CocoaLabelBackend(StdXX::UIBackend *uiBackend, StdXX::UI::Label *label);

		//Destructor
		~CocoaLabelBackend();

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget &GetWidget() override;
		NSView *GetView() override;
		//using CocoaView::SetBounds;
		void SetText(const StdXX::String &text) override;











		//OLD STUFF
		WidgetBackend *CreateChildBackend(StdXX::UI::Widget *widget) const override;

		StdXX::Math::SizeD GetSize() const override;
		void Paint() override;

		void Repaint() override;

		void Select(StdXX::UI::ControllerIndex &controllerIndex) const override;

		StdXX::Path SelectExistingDirectory(const StdXX::String &title,
											const StdXX::Function<bool(StdXX::Path &)> callback) const override;
		void SetEditable(bool enable) const override;

		void SetEnabled(bool enable) const override;

		void SetHint(const StdXX::String &text) const override;
		void Show(bool visible) override;

		void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;

		void UpdateSelection(StdXX::UI::SelectionController &selectionController) const override;
		uint32 GetPosition() const override;

		void GetRange(int32 &min, int32 &max) override;
		void ResetView() const override;

		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		//END OLD STUFF

	private:
		//Members
		StdXX::UI::Label *label;
		NSTextField *textField;
	};
}