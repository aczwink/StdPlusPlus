/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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

/*
#pragma once
//Global
#include <gtk/gtk.h>
//Local
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include <Std++/UI/Widget.hpp>
#include <Std++/UI/Controllers/ControllerIndex.hpp>
#include <Std++/UI/Controllers/SelectionController.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

namespace _stdxx_
{
	class oldGtkWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		oldGtkWindowBackend(UIBackend *uiBackend, Widget *widget, const oldGtkWindowBackend *parentBackend);

		//Destructor
		~oldGtkWindowBackend();

		//Methods
		uint32 GetPosition() const override;
		void GetRange(int32 &min, int32 &max) override;
		StdXX::Math::Size<float32> QuerySize() const;
		StdXX::Math::Size<float32> GetSizeHint() const;
		int32 GetValue() const override;
		bool IsChecked() const override;
		void Maximize() override;
		void Paint() override;
		void ResetView() const override;
		void Repaint() override;
		void Select(StdXX::UI::ControllerIndex &controllerIndex) const;
		void SetBounds(const StdXX::Math::Rect<float32> &area);
		void SetEditable(bool enable) const override;
		void SetMaximum(uint32 max) override;
		void SetMinimum(uint32 min) override;
		void SetPosition(uint32 pos) const override;
		void SetRange(int32 min, int32 max) override;
		void SetText(const StdXX::String &text);
		void SetValue(int32 value) override;
		void ShowInformationBox(const String &title, const String &message) const;
		void UpdateSelection(StdXX::UI::SelectionController &selectionController) const;

		//Inline
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;


	private:
		//Members
		GtkWidget *childAreaWidget; //its child area
		GtkWidget *headerBar; //for windows
	};
}
 */