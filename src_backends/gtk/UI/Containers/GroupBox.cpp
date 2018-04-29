/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/UI/Containers/GroupBox.hpp>
//Local
#include <Std++/UI/Layouts/GridLayout.hpp>
#include "../Gtk.h"
#include "../GtkEventSource.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;
//Definitions
#define THIS PRIVATE_DATA(this)

//Public methods
Rect GroupBox::GetChildrenRect() const
{
	Rect rect;

	rect = this->GetBounds();

	//TODO: shit we dont know this correctly...
	rect.y() += 10;
	rect.height() -= 10;

	return rect;
}

void GroupBox::SetText(const OldString &text)
{
	UTF8String textUTF8;

	textUTF8 = text.GetUTF16();
	gtk_frame_set_label(GTK_FRAME(THIS->widget), (const gchar *)textUTF8.GetC_Str());
}