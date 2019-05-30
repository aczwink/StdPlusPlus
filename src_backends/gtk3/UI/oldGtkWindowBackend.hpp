/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include <Std++/UI/Widget.hpp>
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
		StdPlusPlus::Size GetSize() const;
		StdPlusPlus::Size GetSizeHint() const;
		int32 GetValue() const override;
		bool IsChecked() const override;
		void Maximize() override;
		void Paint() override;
		void ResetView() const override;
		void Repaint() override;
		void Select(ControllerIndex &controllerIndex) const;
		Path SelectExistingDirectory(const String &title, const Function<bool(StdPlusPlus::Path &)> callback) const;
		void SetBounds(const StdPlusPlus::Rect &area);
		void SetEditable(bool enable) const override;
		void SetMaximum(uint32 max) override;
		void SetMinimum(uint32 min) override;
		void SetPosition(uint32 pos) const override;
		void SetRange(int32 min, int32 max) override;
		void SetText(const StdPlusPlus::String &text);
		void SetValue(int32 value) override;
		void ShowInformationBox(const String &title, const String &message) const;
		void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const;

		//Inline
		void SetMenuBar(StdPlusPlus::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;


	private:
		//Members
		GtkWidget *childAreaWidget; //its child area
		GtkWidget *headerBar; //for windows
	};
}