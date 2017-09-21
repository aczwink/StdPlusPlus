/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../../Multimedia/Packet.hpp"
#include "../../Multitasking/Mutex.hpp"
#include "../../Rendering/Texture2D.hpp"
#include "PathRenderTargetWidget.hpp"

namespace ACStdLib
{
	namespace UI
	{
		class ACSTDLIB_API VideoWidget : public PathRenderTargetWidget
		{
		public:
			//Constructor
			VideoWidget(WidgetContainer *parent);

			//Destructor
			~VideoWidget();

			//Methods
			void UpdatePicture(Multimedia::Packet *videoPacket, Size frameSize);

		private:
			//Members
			Rendering::Texture2D *texture;
			Multimedia::Packet *nextFrame;
			Size frameSize;
			Mutex frameLock;

			//Eventhandlers
			void OnPaint();
		};
	}
}