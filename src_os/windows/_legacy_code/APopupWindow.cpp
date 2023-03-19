/*
//Class Header
#include "../../../../headers/GUI/WindowClasses/APopupWindow.h"
//Global
#include <Windows.h>
#include <gl/GL.h>
//Local
#include "../../../../headers/Containers/CFiniteSet/CFiniteSet.h"
#include "../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../../headers/GUI/Themes.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::GUI;

//Constructor
APopupWindow::APopupWindow() : AWindowContainer(*this)
{	
	this->pOSDeviceContext = nullptr;
}

//Eventhandlers
void APopupWindow::OnResized()
{
}

//Constructor
/*APopupWindow::APopupWindow()
{
	g_popupList.InsertTail(this);
}

//Callbacks
void APopupWindow::OnResized()
{
	CRect rect;
	
	rect = this->GetRect();
	// Sets the size of the OpenGL viewport
	glViewport(0, 0, rect.width, rect.height);

	// Select the projection stack and apply 
	// an orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, rect.width, rect.height, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//Private methods
void APopupWindow::InitOpenGL()
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd = {0};

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	pixelFormat = ChoosePixelFormat((HDC)this->pOSDeviceContext, &pfd);
	SetPixelFormat((HDC)this->pOSDeviceContext, pixelFormat, &pfd);
	this->pOpenGLContext = wglCreateContext((HDC)this->pOSDeviceContext);
	wglMakeCurrent((HDC)this->pOSDeviceContext, (HGLRC)this->pOpenGLContext);

	//enable 2d texturing
	glEnable(GL_TEXTURE_2D);
	//choose a smooth shading model
	glShadeModel(GL_SMOOTH);
	
	//enable alpha test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
}

void APopupWindow::InfoMessageBox(const CUTF8String &refTitle, const CUTF8String &refMsg)
{
	CUTF16String titleUTF16(refTitle);
	CUTF16String msgUTF16(refMsg);
	
	MessageBoxW((HWND)this->pOSHandle, (LPCWSTR)msgUTF16.GetC_Str(), (LPCWSTR)titleUTF16.GetC_Str(), MB_ICONINFORMATION);
}
*/

//Public methods
CRect APopupWindow::GetRect() const
{
	RECT rc;
	CRect rect;

	GetClientRect((HWND)this->pOSHandle, &rc);

	rect.x() = 0;
	rect.y() = 0;
	rect.width() = (int16)rc.right; //rc.left is == 0
	rect.height() = (int16)rc.bottom; //rc.top is == 0

	return rect;
}
*/
