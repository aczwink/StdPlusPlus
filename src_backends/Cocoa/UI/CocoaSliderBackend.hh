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
#import <Std++/_Backends/UI/SliderBackend.hpp>
#import "CocoaView.hh"

//Forward delcarations
namespace _stdxx_
{
	class CocoaSliderBackend;
}

//Objective-C class
@interface CocoaSlider : NSSlider
-(id)initWithBackend:(_stdxx_::CocoaSliderBackend *)backend;
- (void)OnValueChanged:(NSSlider *)sender;
@end

namespace _stdxx_
{
	class CocoaSliderBackend : public SliderBackend, public CocoaView
	{
	public:
		//Constructor
		CocoaSliderBackend(StdXX::UIBackend *uiBackend, StdXX::UI::Slider *slider);

		//Destructor
		~CocoaSliderBackend();

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() override;
		StdXX::UI::Widget &GetWidget() override;
		void SetPosition(uint32 pos) override;
		void SetRange(uint32 min, uint32 max) override;
		void ValueChanged();







		//OLD STUFF
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

		uint32 GetPosition() const override;
		void ResetView() const override;

		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		//END OLD STUFF

	private:
		//Members
		StdXX::UI::Slider *slider;
		CocoaSlider *cocoaSlider;
	};
}