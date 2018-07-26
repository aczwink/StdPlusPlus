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
#include <Std++/_Backends/UI/WidgetBackend.hpp>
#include "Definitions.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
    class CommCtrlWindowBackend : public WindowBackend
    {
    public:
        //Constructor
        CommCtrlWindowBackend(UIBackend *uiBackend, WindowBackendType type, Widget *widget, HWND hParent = nullptr);

        //Destructor
        ~CommCtrlWindowBackend();

        //Public methods
        WindowBackend *CreateChildBackend(WindowBackendType type, StdPlusPlus::UI::Widget *widget) const override;
		Rect GetChildrenRect() const override;
		uint32 GetPosition() const override;
		void GetRange(int32 &min, int32 &max) override;
        Size GetSize() const override;
        Size GetSizeHint() const override;
		int32 GetValue() const override;
		void IgnoreEvent() override;
		bool IsChecked() const override;
		void Maximize() override;
        void Paint() override;
        void Repaint() override;
		void ResetView() const override;
        void Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const override;
        Path SelectExistingDirectory(const StdPlusPlus::String &title, const StdPlusPlus::Function<bool(StdPlusPlus::Path &)> callback) const override;
        void SetBounds(const StdPlusPlus::Rect &area) override;
		void SetEditable(bool enable) const override;
        void SetEnabled(bool enable) const override;
        void SetHint(const StdPlusPlus::String &text) const override;
		void SetMaximum(uint32 max) override;
		void SetMenuBar(StdPlusPlus::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void SetMinimum(uint32 min) override;
		void SetPosition(uint32 pos) const override;
		void SetRange(int32 min, int32 max) override;
        void SetText(const StdPlusPlus::String &text) override;
		void SetValue(int32 value) override;
        void Show(bool visible) override;
        void ShowInformationBox(const StdPlusPlus::String &title, const StdPlusPlus::String &message) const override;
        void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const override;

		//Inline
		inline HWND GetHandle()
		{
			return this->hWnd;
		}

    private:
        //Members
        HWND hWnd;
		HWND hWndReal; //the Up-Down-Control (i.e. this->hWnd is the buddy) for SpinBoxes

		//Methods
		String GetText() const;
		Size GetTextExtents() const;

		//Inline
		inline HFONT GetFont() const
		{
			return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		}

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const
		{
			return SendMessageW(this->hWnd, Msg, wParam, lParam);
		}
	};
}