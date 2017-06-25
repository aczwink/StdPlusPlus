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
//Class header
#include <ACStdLib/UI/Displays/VideoWidget.hpp>
//Local
#include <ACStdLib/Rendering/DeviceContext.hpp>
#include <ACStdLib/Math/Fraction.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
using namespace ACStdLib::UI;

//Constructor
VideoWidget::VideoWidget(WidgetContainer *parent) : PathRenderTargetWidget(parent)
{
	this->texture = this->deviceContext->CreateTexture2D();
	this->nextFrame = nullptr;
}

//Destructor
VideoWidget::~VideoWidget()
{
	delete this->texture;
}

//Public methods
void VideoWidget::UpdatePicture(Packet *videoPacket, Size frameSize)
{
	AutoLock autoLock(this->frameLock);
	if(this->nextFrame)
	{
		//we already have a frame that wasn't displayed yet
		delete videoPacket; //throw frame away, to not delay playback
	}
	else
	{
		this->nextFrame = videoPacket;
		this->frameSize = frameSize;
		this->Repaint();
	}
}

//Eventhandlers
void VideoWidget::OnPaint()
{
	//upload new frame if necessary
	this->frameLock.Lock();
	if(this->nextFrame)
	{
		this->deviceContext->SetUploadAlignment(1); //allow proper sizing of non-power-of-two images
		this->texture->AllocateRGB(this->frameSize, this->nextFrame->GetData());
		this->texture->SetMaximumMipMapLevel(0);

		delete this->nextFrame;
		this->nextFrame = nullptr;
	}
	this->frameLock.Unlock();

	//compute size and pos of image
	Fraction aspectRatio;
	aspectRatio.numerator = this->frameSize.width;
	aspectRatio.denominator = this->frameSize.height;
	aspectRatio = aspectRatio.Reduce();

	Rect frameRect;
	frameRect.size.width = this->GetSize().width;
	if(aspectRatio.numerator == 0)
		aspectRatio.numerator = 1; //avoid 0 division
	frameRect.size.height = (uint16) (frameRect.size.width / aspectRatio);
	if(frameRect.size.height > this->GetSize().height)
	{
		frameRect.size.height = this->GetSize().height;
		frameRect.size.width = (uint16) (frameRect.size.height * aspectRatio);
	}

	frameRect.origin.x = (this->GetSize().width - frameRect.size.width) / 2;
	frameRect.origin.y = (this->GetSize().height - frameRect.size.height) / 2;

	//render
	this->deviceContext->ClearColorBuffer(Color(0, 0, 0, 1));
	this->deviceContext->EnableDepthTest(false); //#TODO: gtk bug that GtkGlArea always does glEnable(GL_DEPTH_TEST);

	this->renderer.BeginPath();
	this->renderer.Rectangle(frameRect.origin.x, frameRect.origin.y, frameRect.size.width, frameRect.size.height);
	this->renderer.SetFillTexture(this->texture);
	this->renderer.Fill();

	this->renderer.Sync();
	this->deviceContext->SwapBuffers();
}