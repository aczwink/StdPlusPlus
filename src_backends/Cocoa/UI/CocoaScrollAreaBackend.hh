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
//Local
#include <Std++/_Backends/UI/ContentAreaWidgetBackend.hpp>
#import <Std++/UI/Containers/ScrollArea.hpp>
#include "CocoaWidgetBackend.hh"

namespace _stdxx_
{
	class CocoaScrollAreaBackend : public ContentAreaWidgetBackend, public CocoaWidgetBackend
	{
	public:
		//Constructor
		inline CocoaScrollAreaBackend(StdXX::UIBackend& uiBackend, StdXX::UI::ScrollArea& scrollArea)
				: ContentAreaWidgetBackend(uiBackend), WidgetContainerBackend(uiBackend), CocoaWidgetBackend(uiBackend), WidgetBackend(uiBackend),
				  scrollArea(scrollArea)
		{
			this->cocoaScrollView = [NSScrollView new];
			[this->cocoaScrollView setDrawsBackground:NO];
		}

		//Methods
		void AddChild(StdXX::UI::Widget *widget) override;
		WidgetContainerBackend *CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget) override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		StdXX::Math::SizeD GetSizeHint() const override;
		NSView *GetView() override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;



		//NOT IMPLEMENTED
		void Repaint() override;

		void SetEditable(bool enable) const override;
		//END OF NOT IMPLEMENTED

	private:
		//Members
		StdXX::UI::ScrollArea& scrollArea;
		NSScrollView* cocoaScrollView;
	};
}