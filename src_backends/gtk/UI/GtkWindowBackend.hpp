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
#include <gtk/gtk.h>
//Local
#include <Std++/_Backends/WindowBackend.hpp>
#include <Std++/UI/Widget.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
	class GtkWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		GtkWindowBackend(_stdpp::WindowBackendType type, Widget *widget);

		//Destructor
		~GtkWindowBackend();

		//Methods
		void ClearView() const;
		WindowBackend *CreateChildBackend(_stdpp::WindowBackendType type, StdPlusPlus::UI::Widget *widget) const;
		StdPlusPlus::Size GetSize() const;
		StdPlusPlus::Size GetSizeHint() const;
		void Select(ControllerIndex &controllerIndex) const;
		Path SelectExistingDirectory(const String &title, const Function<bool(StdPlusPlus::Path &)> callback) const;
		void SetBounds(const StdPlusPlus::Rect &area);
		void SetEnabled(bool enable) const;
		void SetHint(const StdPlusPlus::String &text) const;
		void SetText(const StdPlusPlus::String &text);
		void Show(bool visible);
		void ShowInformationBox(const String &title, const String &message) const;
		void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const;

	private:
		//Members
		_stdpp::WindowBackendType type;
		StdPlusPlus::UI::Widget *widget;
		GtkWidget *gtkWidget; //the widget, window whatever itself
		GtkWidget *childAreaWidget; //its child area
		GtkWidget *headerBar; //for windows

		//Methods
		void AddNodes(GtkTreeStore *store, GtkTreeIter *nodeIter, const StdPlusPlus::UI::ControllerIndex &parent, const StdPlusPlus::UI::TreeController &controller) const;
	};
}