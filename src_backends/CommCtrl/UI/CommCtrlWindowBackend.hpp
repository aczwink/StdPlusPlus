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
#include <Windows.h>
//Local
#include <Std++/_Backends/WindowBackend.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
    class CommCtrlWindowBackend : public WindowBackend
    {
    public:
        //Constructor
        CommCtrlWindowBackend(UIBackend *uiBackend, _stdpp::WindowBackendType type, Widget *widget);

        //Destructor
        ~CommCtrlWindowBackend();

        //Public methods
        void ClearView() const override;
        WindowBackend *CreateChildBackend(WindowBackendType type, StdPlusPlus::UI::Widget *widget) const override;
        Size GetSize() const override;
        Size GetSizeHint() const override;
        UIBackend *GetUIBackend() override;
        void Repaint() override;
        void Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const override;
        Path SelectExistingDirectory(const StdPlusPlus::String &title, const StdPlusPlus::Function<bool(StdPlusPlus::Path &)> callback) const override;
        void SetBounds(const StdPlusPlus::Rect &area) override;
        void SetEnabled(bool enable) const override;
        void SetHint(const StdPlusPlus::String &text) const override;
        void SetText(const StdPlusPlus::String &text) override;
        void Show(bool visible) override;
        void ShowInformationBox(const StdPlusPlus::String &title, const StdPlusPlus::String &message) const override;
        void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const override;

    private:
        //Members
        HWND hWnd;
        Widget *widget;
    };
}