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
#include <Std++/_Backends/UI/CheckBoxBackend.hpp>
#import "CocoaWidgetBackend.hh"

//Forward delcarations
namespace _stdxx_
{
	class CocoaCheckBoxBackend;
}

//Objective-C class
@interface CocoaCheckBox : NSButton
-(id)initWithBackend:(_stdxx_::CocoaCheckBoxBackend *)backend;
- (void)OnClick:(id)OnClick;
@end

namespace _stdxx_
{
	class CocoaCheckBoxBackend : public CheckBoxBackend, public CocoaWidgetBackend
	{
	public:
		//Constructor
		CocoaCheckBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::CheckBox *checkBox);

		//Destructor
		~CocoaCheckBoxBackend();

		//Methods
		void Clicked();
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		bool IsChecked() const override;
		void SetEnabled(bool enable) override;
		void SetText(const StdXX::String &text) override;






		//old stuff
		void Repaint() override;
		void SetEditable(bool enable) const override;
		//end old stuff

	private:
		StdXX::UI::CheckBox *checkBox;
		CocoaCheckBox *cocoaCheckBox;
	};
}