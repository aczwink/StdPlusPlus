/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../../Multimedia/Packet.hpp"
#include "../../Multitasking/Mutex.hpp"
#include "../../Rendering/Texture2D.hpp"
#include "PathRenderTargetWidget.hpp"

namespace StdXX
{
	namespace UI
	{
		class STDPLUSPLUS_API VideoWidget : public PathRenderTargetWidget
		{
		public:
			//Constructor
			inline VideoWidget()
				: texture(nullptr), nextFrame(nullptr)
			{
			}

			//Destructor
			~VideoWidget();

			//Methods
			void UpdatePicture(Multimedia::Packet *videoPacket, Math::Size<uint16> frameSize);

		protected:
			//Event handlers
			void OnRealized() override;

		private:
			//Members
			Rendering::Texture2D *texture;
			Multimedia::Packet *nextFrame;
			Math::Size<uint16> frameSize;
			Mutex frameLock;

			//Eventhandlers
			void OnPaint(UI::Event& event) override;
		};
	}
}