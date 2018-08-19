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

//Objective-C class
@interface IntegerValueFormatter : NSNumberFormatter
{
	int32_t min;
	int32_t max;
};
-(void)setRange:(int32_t)min max:(int32_t)max;
@end


namespace _stdxx_
{
	class CocoaSpinBoxBackend : public SpinBoxBackend
	{
	public:
		//Constructor
		CocoaSpinBoxBackend(StdXX::UIBackend *uiBackend, StdXX::UI::SpinBox *spinBox);

		//Destructor
		~CocoaSpinBoxBackend();

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		int32 GetValue() const override;
		void IgnoreEvent() override;
		void SetBounds(const StdXX::Math::RectD &area) override;
		void SetRange(int32 min, int32 max) override;
		void SetValue(int32 value) override;

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
		//END OF OLD STUFF

	private:
		//Members
		StdXX::UI::SpinBox *spinBox;
		NSTextField *textField;
		NSStepper *stepper;
		IntegerValueFormatter *integerValueFormatter;
	};
}