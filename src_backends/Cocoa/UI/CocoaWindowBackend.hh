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
#include <Cocoa/Cocoa.h>
//Local
#include <Std++/_Backends/WindowBackend.hpp>
#include "WindowDelegate.hh"

namespace _stdpp
{
	class CocoaWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		CocoaWindowBackend(StdPlusPlus::UIBackend *uiBackend, WindowBackendType type, StdPlusPlus::UI::Widget *widget);

		//Destructor
		~CocoaWindowBackend();

		//Methods
		WindowBackend *CreateChildBackend(_stdpp::WindowBackendType type, StdPlusPlus::UI::Widget *widget) const override;
		StdPlusPlus::Rect GetChildrenRect() const override;
		uint32 GetPosition() const override;
		void GetRange(int32 &min, int32 &max) override;
		StdPlusPlus::Size GetSize() const override;
		StdPlusPlus::Size GetSizeHint() const override;
		int32 GetValue() const override;
		NSView *GetView() const;
		bool IsChecked() const override;
		void IgnoreEvent() override;
		void Maximize() override;
		void Paint() override;
		void Repaint() override;
		void ResetView() const override;
		void Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const override;
		StdPlusPlus::Path SelectExistingDirectory(const StdPlusPlus::String &title, const StdPlusPlus::Function<bool(StdPlusPlus::Path &)> callback) const override;
		void SetBounds(const StdPlusPlus::Rect &area) override;
		void SetEditable(bool enable) const override;
		void SetEnabled(bool enable) const override;
		void SetHint(const StdPlusPlus::String &text) const override;
		void SetMaximum(uint32 max) override;
		void SetMenuBar(StdPlusPlus::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetMinimum(uint32 min) override;
		void SetPosition(uint32 pos) const override;
		void SetRange(int32 min, int32 max) override;
		void SetText(const StdPlusPlus::String &text) override;
		void SetValue(int32 value) override;
		void Show(bool visible) override;
		void ShowInformationBox(const StdPlusPlus::String &title, const StdPlusPlus::String &message) const override;
		void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const override;

	private:
		//Members
		union
		{
			NSButton *button;
			NSBox *groupBox;
			NSOpenGLView *openGLView;
			NSSlider *slider;
			NSTextField *textField;
			NSWindow *window;
		};
		WindowDelegate *windowDelegate;
		NSWindowController *windowController;

		//Methods
		StdPlusPlus::Size ComputeTextSize(NSString *string, NSFont *font) const;
		NSView *GetChildrenAreaView() const;
	};
}