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
#import <Std++/_Backends/UI/SpinBoxBackend.hpp>

//Forward declarations
namespace _stdxx_
{
	class CocoaSpinBoxBackend;
}

//Objective-C classes
@interface IntegerValueFormatter : NSNumberFormatter
{
	int32_t min;
	int32_t max;
};
-(void)setRange:(int32_t)min max:(int32_t)max;
@end

@interface TextFieldDelegate : NSObject <NSTextFieldDelegate>
{
};
-(id)initWithBackend:(_stdxx_::CocoaSpinBoxBackend *)backend;
@end

@interface CocoaStepper : NSStepper
-(id)initWithBackend:(_stdxx_::CocoaSpinBoxBackend *)backend;
- (void)OnValueChanged:(NSStepper *)sender;
@end


namespace _stdxx_
{
	class CocoaSpinBoxBackend : public SpinBoxBackend
	{
	public:
		//Constructor
		CocoaSpinBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::SpinBox *spinBox);

		//Destructor
		~CocoaSpinBoxBackend();

		//Event handlers
		void OnStepperClicked();
		void OnTextChanged();

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void IgnoreEvent() override;
		void SetBounds(const StdXX::Math::RectD &area) override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String &text) override;
		void SetRange(int32 min, int32 max) override;
		void SetValue(int32 value) override;
		void Show(bool visible) override;

		//Inline
		inline NSStepper *GetStepper()
		{
			return this->stepper;
		}

		inline NSTextField *GetTextField()
		{
			return this->textField;
		}







		//OLD STUFF
		void Repaint() override;
		void SetEditable(bool enable) const override;
		//END OF OLD STUFF

	private:
		//Members
		StdXX::UI::SpinBox *spinBox;
		NSTextField *textField;
		TextFieldDelegate* textFieldDelegate;
		CocoaStepper *stepper;
		IntegerValueFormatter *integerValueFormatter;

		//Methods
		int32 GetValue() const;
	};
}