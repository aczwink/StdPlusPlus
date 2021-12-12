/*
 * Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Window.hpp>
#include <Std++/FileSystem/Path.hpp>
#include "CommCtrlContainerBackend.hpp"
#include "Win32Window.hpp"
#include "../Imports.h"
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

namespace _stdxx_
{
    class CommCtrlWindowBackend : public WindowBackend, public CommCtrlContainerBackend, public Win32Window
    {
    public:
        //Constructor
		inline CommCtrlWindowBackend(UIBackend& uiBackend, Window *window)
			: WindowBackend(uiBackend), WidgetContainerBackend(uiBackend), CommCtrlContainerBackend(uiBackend, window),
			CommCtrlWidgetBackend(uiBackend), Win32Window(*this, STDPLUSPLUS_WIN_WNDCLASS, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN),
			WidgetBackend(uiBackend),
			window(window), showFirstTime(true), maximizeWindow(false)
		{
		}

        //Public methods
		void AddChild(StdXX::UI::Widget * widget) override;
		_stdxx_::WidgetContainerBackend * CreateContentAreaBackend(StdXX::UI::CompositeWidget & widget) override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void Maximize() override;
		void OnMessage(WinMessageEvent& event) override;
        void Repaint() override;
        FileSystem::Path SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(FileSystem::Path &)> callback) const override;
		virtual StdXX::FileSystem::Path SelectExistingFile(const StdXX::String& title, const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>>& filters, const StdXX::FileSystem::Path& initialPath) const override;
        void SetBounds(const StdXX::Math::RectD &area) override;
		void SetEditable(bool enable) const override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetTitle(const StdXX::String & title) override;
        void Show(bool visible) override;
		void ShowErrorBox(const StdXX::String & title, const StdXX::String & message) const override;
        void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;

    private:
        //Members
		Window *window;
		bool showFirstTime;
		bool maximizeWindow;

		//HWND hWndReal; //the Up-Down-Control (i.e. this->hWnd is the buddy) for SpinBoxes
	};
}