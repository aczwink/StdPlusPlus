/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Global
#include <gtk/gtk.h>
//Local
#include <ACStdLib/_Backends/WindowBackend.hpp>
#include <ACStdLib/UI/Widget.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

namespace _ACStdLib_internal
{
	class GtkWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		GtkWindowBackend(_ACStdLib_internal::WindowBackendType type, Widget *widget);

		//Destructor
		~GtkWindowBackend();

		//Methods
		void ClearView() const;
		WindowBackend *CreateChildBackend(_ACStdLib_internal::WindowBackendType type, ACStdLib::UI::Widget *widget) const;
		ACStdLib::Size GetSize() const;
		ACStdLib::Size GetSizeHint() const;
		void Select(ControllerIndex &controllerIndex) const;
		Path SelectExistingDirectory(const String &title, const Function<bool(ACStdLib::Path &)> callback) const;
		void SetBounds(const ACStdLib::Rect &area);
		void SetEnabled(bool enable) const;
		void SetHint(const ACStdLib::String &text) const;
		void SetText(const ACStdLib::String &text);
		void Show(bool visible);
		void ShowInformationBox(const String &title, const String &message) const;
		void UpdateSelection(ACStdLib::UI::SelectionController &selectionController) const;

	private:
		//Members
		_ACStdLib_internal::WindowBackendType type;
		ACStdLib::UI::Widget *widget;
		GtkWidget *gtkWidget; //the widget, window whatever itself
		GtkWidget *childAreaWidget; //its child area
		GtkWidget *headerBar; //for windows

		//Methods
		void AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, const ACStdLib::UI::ControllerIndex &parent, const ACStdLib::UI::TreeController &controller) const;
	};
}