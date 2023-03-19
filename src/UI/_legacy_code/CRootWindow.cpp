//Class Header
#include "../../headers/TUI/CRootWindow.h"
//Local
#include "../../headers/TUI/Themes.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::TUI;

//Constructor
CRootWindow::CRootWindow(IRenderTarget *pRenderTarget)
{
	this->pRenderTarget = pRenderTarget;
	
	this->pRenderTarget->ShowCursor(false);

	this->eventQueue.InsertTail(CTuple<EEventType, void *>(EVENT_TYPE_RESIZED, nullptr));
}

//Eventhandlers
void CRootWindow::OnDraw()
{
	const STheme &refTheme = GetActiveTheme();

	this->pRenderTarget->SetBackgroundColor(refTheme.backgroundColor);
	this->pRenderTarget->SetForegroundColor(refTheme.foregroundColor);
	
	this->pRenderTarget->Clear();

	AWindowContainer::OnDraw();
	
	this->pRenderTarget->Present();
}

void CRootWindow::OnResized()
{
	AWindowContainer::OnResized();

	this->pRenderTarget->ResizeView();
	this->Redraw();
}

//Public methods
void CRootWindow::ProcessEvent()
{
	CTuple<EEventType, void *> event;
	
	while(true)
	{
		//get pending events from render target
		while(this->pRenderTarget->PeekEvent(event))
			this->eventQueue.InsertTail(event);

		//process the event
		if(!this->eventQueue.IsEmpty())
		{
			event = this->eventQueue.PopFront();
			
			switch(event.Get<0>())
			{
				/*
			case EVENT_TYPE_KEYDOWN:
			case EVENT_TYPE_KEYUP:
				{
					GUI::SKeyEvent *pKeyEvent;
					
					pKeyEvent = (GUI::SKeyEvent *)event.Get<1>();
					
					MemFree(event.Get<1>());
				}
				break;
				*/
			case EVENT_TYPE_RESIZED:
				{
					CRect rect;
					
					rect.width() = this->pRenderTarget->GetNumberOfColumns();
					rect.height() = this->pRenderTarget->GetNumberOfRows();
					
					this->SetPosition(rect);
				}
				break;
			default:
				ASSERT(0);
			}
			
			return;
		}

		//TODO would be a good idea to sleep here a bit
	}
}

void CRootWindow::ProcessEvents()
{
	while(true)
	{
		this->ProcessEvent();
	}
}

void CRootWindow::Redraw()
{
	this->OnDraw();
}