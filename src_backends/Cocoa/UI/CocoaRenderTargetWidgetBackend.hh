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
#include <Std++/_Backends/UI/WidgetBackend.hpp>
#import "CocoaView.hh"

//Forward declarations
namespace _stdxx_
{
	class CocoaRenderTargetWidgetBackend;
}

//Objective-C class
@interface OpenGLView : NSOpenGLView
{
};
-(id)initWithBackend:(_stdxx_::CocoaRenderTargetWidgetBackend *)backend;
@end


namespace _stdxx_
{
	class CocoaRenderTargetWidgetBackend : public CocoaView
	{
	public:
		//Constructor
		CocoaRenderTargetWidgetBackend(StdXX::UIBackend *uiBackend, StdXX::UI::Widget *widget);

		//Destructor
		~CocoaRenderTargetWidgetBackend();

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() override;
		StdXX::UI::Widget &GetWidget() override;
		void Repaint() override;

		//Inline
		inline OpenGLView *GetOpenGLView()
		{
			return this->openGLView;
		}






		//OLD STUFF:
		WidgetBackend *CreateChildBackend(StdXX::UI::Widget *widget) const override;

		StdXX::Math::SizeD GetSize() const override;
		void Paint() override;

		void Select(StdXX::UI::ControllerIndex &controllerIndex) const override;
		void SetEditable(bool enable) const override;

		void SetEnabled(bool enable) const override;

		void SetHint(const StdXX::String &text) const override;
		void Show(bool visible) override;

		void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;

		void UpdateSelection(StdXX::UI::SelectionController &selectionController) const override;

		void IgnoreEvent() override;

		uint32 GetPosition() const override;

		void GetRange(int32 &min, int32 &max) override;
		void ResetView() const override;

		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		//END OF OLD STUFF

	private:
		//Members
		StdXX::UI::Widget *widget;
		OpenGLView *openGLView;
	};
}