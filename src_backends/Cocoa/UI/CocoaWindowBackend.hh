/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/WindowBackend.hpp>
#import <Std++/UI/Window.hpp>

//Forward declarations
namespace _stdxx_
{
	class CocoaWindowBackend;
}

//Objective-C class
@interface WindowDelegate : NSObject <NSWindowDelegate>
{
};
-(id)initWithBackend:(_stdxx_::CocoaWindowBackend *)backend;
@end


namespace _stdxx_
{
	class CocoaWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		CocoaWindowBackend(StdXX::UIBackend& uiBackend, StdXX::UI::Window *window);

		//Destructor
		~CocoaWindowBackend();

		//Methods
		void AddChild(StdXX::UI::Widget *widget) override;
		WidgetContainerBackend *CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget) override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void IgnoreEvent() override;
		void Maximize() override;
		StdXX::FileSystem::Path SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(StdXX::FileSystem::Path &)> callback) const override;
		StdXX::FileSystem::Path SelectExistingFile(const StdXX::String &title, const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>>& filters, const StdXX::FileSystem::Path &initialPath) const override;
		void SetBounds(const StdXX::Math::RectD &area) override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String &text) override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetTitle(const StdXX::String &title) override;
		void Show(bool visible) override;

		//Inline
		inline StdXX::UI::Window &GetWindow()
		{
			return *this->window;
		}



		//OLD STUFF
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() const;
		void Repaint() override;
		void SetEditable(bool enable) const override;
		void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;
		void ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const override;
		//END OLD STUFF

	private:
		//Members
		StdXX::UI::Window *window;
		NSWindow *cocoaWindow;
		WindowDelegate *windowDelegate;
		NSWindowController *windowController;

		//Methods
		StdXX::Math::SizeD ComputeTextSize(NSString *string, NSFont *font) const;
		NSString* CreateString(const StdXX::String& string) const;
		StdXX::FileSystem::Path RunOpenPanel(NSOpenPanel* panel) const;
	};
}