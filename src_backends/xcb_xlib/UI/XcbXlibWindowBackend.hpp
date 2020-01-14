/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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

namespace _stdxx_
{
	//Forward declarations
	class XcbWindow;
	class XConnection;

	class XcbXlibWindowBackend : public WindowBackend
	{
	public:
		//Constructor
		XcbXlibWindowBackend(StdXX::UIBackend& uiBackend, XConnection& xConnection, StdXX::UI::Window& window);

		//Destructor
		~XcbXlibWindowBackend();

		//Methods
		void AddChild(StdXX::UI::Widget *widget) override;
		WidgetContainerBackend *CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget) override;
		StdXX::UI::Widget &GetWidget() override;
		const StdXX::UI::Widget &GetWidget() const override;
		void Maximize() override;
		void SetTitle(const StdXX::String &title) override;
		void Show(bool visible) override;

		//NOT IMPLEMENTED
		StdXX::Math::RectD GetContentAreaBounds() const override;
		StdXX::Math::SizeD GetSizeHint() const override;
		void IgnoreEvent() override;
		void Repaint() override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;
		void SetEditable(bool enable) const override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String &text) override;
		StdXX::Path SelectExistingDirectory(const StdXX::String &title,
		                                    const StdXX::Function<bool(StdXX::Path &)> callback) const override;
		StdXX::Path SelectExistingFile(const StdXX::String &title,
		                               const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>> &filters,
		                               const StdXX::Path &initialPath) const override;
		void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) override;
		void ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const override;
		void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const override;
		//END OF NOT IMPLEMENTED

	private:
		//Members
		XConnection& xConnection;
		StdXX::UI::Window& window;
		XcbWindow* xcbWindow;
	};
}