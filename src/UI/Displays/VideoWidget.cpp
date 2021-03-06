/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/UI/Displays/VideoWidget.hpp>
//Local
#include <Std++/Rendering/DeviceContext.hpp>
#include <Std++/Math/Rational.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;
using namespace StdXX::UI;

//Destructor
VideoWidget::~VideoWidget()
{
	delete this->texture;
}

//Public methods
void VideoWidget::UpdatePicture(Packet *videoPacket, Math::Size<uint16> frameSize)
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
void VideoWidget::OnPaint(PaintEvent& event)
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
	Math::Rational<uint16> aspectRatio;
	aspectRatio.numerator = this->frameSize.width;
	aspectRatio.denominator = this->frameSize.height;
	aspectRatio = aspectRatio.Reduce();

	Math::RectD frameRect;
	frameRect.size.width = this->GetSize().width;
	if(aspectRatio.numerator == 0)
		aspectRatio.numerator = 1; //avoid 0 division
	frameRect.size.height = (uint16) (frameRect.size.width / aspectRatio.ToFloat());
	if(frameRect.size.height > this->GetSize().height)
	{
		frameRect.size.height = this->GetSize().height;
		frameRect.size.width = (uint16) (frameRect.size.height * aspectRatio.ToFloat());
	}

	frameRect.origin.x = (this->GetSize().width - frameRect.size.width) / 2;
	frameRect.origin.y = (this->GetSize().height - frameRect.size.height) / 2;

	//render
	this->deviceContext->ClearColorBuffer(Color(0, 0, 0, 1));
	
	this->renderer->Rectangle(frameRect);
	this->renderer->SetFillTexture(this->texture);
	this->renderer->Fill();

	this->renderer->Sync();
	this->deviceContext->SwapBuffers();

	event.Accept();
}

void VideoWidget::OnRealized()
{
	PathRenderTargetWidget::OnRealized();

	this->texture = this->deviceContext->CreateTexture2D();
}