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
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include "Win32Window.hpp"
#include "../Imports.h"
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

namespace _stdxx_
{
    class CommCtrlWindowBackend : public WindowBackend, public Win32Window
    {
    public:
        //Constructor
        CommCtrlWindowBackend(UIBackend *uiBackend, Window *window);

        //Public methods
		void AddChild(StdXX::UI::Widget * widget) override;
		StdXX::UI::CompositeWidget * CreateContentArea() override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		uint32 GetPosition() const override;
        Math::SizeD GetSize() const override;
		Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void IgnoreEvent() override;
		void Maximize() override;
        void Paint() override;
        void Repaint() override;
		void ResetView() const override;
        void Select(StdXX::UI::ControllerIndex &controllerIndex) const override;
        Path SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(StdXX::Path &)> callback) const override;
        void SetBounds(const StdXX::Math::RectD &area) override;
		void SetEditable(bool enable) const override;
        void SetEnabled(bool enable) const override;
        void SetHint(const StdXX::String &text) const override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetTitle(const StdXX::String & title) override;
        void Show(bool visible) override;
        void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;
        void UpdateSelection(StdXX::UI::SelectionController &selectionController) const override;

    private:
        //Members
		Window *window;
		bool showFirstTime;
		bool maximizeWindow;
		//HWND hWndReal; //the Up-Down-Control (i.e. this->hWnd is the buddy) for SpinBoxes

		//Methods
		String GetText() const;
		Math::SizeD GetTextExtents() const;
	};
}