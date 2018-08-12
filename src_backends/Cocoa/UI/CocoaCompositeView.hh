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
#import <Std++/UI/Containers/CompositeWidget.hpp>
#import "CocoaView.hh"

namespace _stdxx_
{
	class CocoaCompositeView : public StdXX::UI::CompositeWidget, public WidgetContainerBackend, public CocoaView
	{
	public:
		//Constructor
		inline CocoaCompositeView(StdXX::UIBackend *uiBackend, NSView *view) : WidgetContainerBackend(uiBackend), CocoaView(uiBackend), WidgetBackend(uiBackend), view(view)
		{
			this->widgetContainerBackend = this;
			this->backend = this;
		}

		//Destructor
		~CocoaCompositeView();

		//Methods
		void AddChild(StdXX::UI::Widget *widget) override;
		StdXX::Math::SizeD GetSizeHint() const override;
		Widget &GetWidget() override;
		void SetBounds(const StdXX::Math::RectD &area) override;

		//Inline
		inline NSView *GetView()
		{
			return this->view;
		}





		//OLD STUFF:
		StdXX::Math::SizeD GetSize() const override;
		void Paint() override;

		void Repaint() override;

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

		void RemoveChild(Widget *child) override;
		//END OF OLD STUFF

	private:
		//Members
		NSView *view;
	};
}