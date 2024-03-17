/*
 * Copyright (c) 2019-2024 Amir Czwink (amir130@hotmail.de)
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
#include "../Gtk3WidgetBackend.hpp"

namespace _stdxx_
{
    class Gtk3WindowBackend : public virtual WindowBackend, public virtual Gtk3WidgetBackend
    {
    public:
        //Constructor
        Gtk3WindowBackend(StdXX::UIBackend& backend, StdXX::UI::Window& window);

        //Methods
        void AddChild(StdXX::UI::Widget *widget) override;
        WidgetContainerBackend *CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget) override;
        StdXX::Math::RectD GetContentAreaBounds() const override;
        const StdXX::UI::Widget &GetWidget() const override;
	    void Maximize() override;
		StdXX::FileSystem::Path SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(StdXX::FileSystem::Path &)> callback) const override;
		StdXX::FileSystem::Path SelectExistingFile(const StdXX::String &title, const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>> &filters, const StdXX::FileSystem::Path &initialPath) const override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
        void SetTitle(const StdXX::String &title) override;
	    void Show(bool visible) override;

        //NOT IMPLEMENTED
        void Repaint() override;
        void SetBounds(const StdXX::Math::RectD &bounds) override;
        void ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const override;
        void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;
		//NOT IMPLEMENTED

    private:
        //Members
    	bool wasVisibleBefore;
        StdXX::UI::Window& window;

        //Properties
        inline GtkWindow* GetGtkWindow() const
        {
            return GTK_WINDOW(this->GetGtkWidget());
        }
    };
}