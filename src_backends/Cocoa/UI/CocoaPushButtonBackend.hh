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
#include <Std++/_Backends/UI/PushButtonBackend.hpp>
#import "CocoaWidgetBackend.hh"

//Forward delcarations
namespace _stdxx_
{
	class CocoaPushButtonBackend;
}

//Objective-C class
@interface CocoaPushButton : NSButton
-(id)initWithBackend:(_stdxx_::CocoaPushButtonBackend *)backend;
- (void)OnClick:(id)OnClick;
@end

namespace _stdxx_
{
	class CocoaPushButtonBackend : public PushButtonBackend, public CocoaWidgetBackend
	{
	public:
		//Constructor
		CocoaPushButtonBackend(StdXX::UIBackend& uiBackend, StdXX::UI::PushButton *pushButton);

		//Destructor
		~CocoaPushButtonBackend();

		//Methods
		void Clicked();
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void SetEnabled(bool enable) override;
		void SetText(const StdXX::String &text) override;













		//OLD STUFF
		void Repaint() override;
		void SetEditable(bool enable) const override;
		//END OLD STUFF

	private:
		//Members
		StdXX::UI::PushButton *pushButton;
		CocoaPushButton *cocoaButton;
	};
}