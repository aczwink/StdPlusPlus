//Class header
#include "../../../../headers/UI/Controls/CTextEdit.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx

//Constructor
CTextEdit::CTextEdit(AWidgetContainer *pParent) : AWidget(pParent)
{
	this->pOSHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

//Public methods
CString CTextEdit::GetText() const
{
	uint32 nChars;
	wchar_t *pBuffer;
	CString result;
	
	nChars = GetWindowTextLengthW((HWND)this->pOSHandle);
	nChars++;
	pBuffer = (wchar_t *)MemAlloc(nChars * sizeof(*pBuffer));
	pBuffer[nChars - 1] = 0;
	GetWindowTextW((HWND)this->pOSHandle, pBuffer, nChars);
	result = CUTF16String((uint16 *)pBuffer);
	MemFree(pBuffer);

	return result;
}

void CTextEdit::SetEditable(bool editable)
{
	SendMessageA((HWND)this->pOSHandle, EM_SETREADONLY, !editable, 0);
}

void CTextEdit::SetText(const CString &refText)
{
	SetWindowTextW((HWND)this->pOSHandle, (LPCWSTR)refText.GetUTF16().GetC_Str());
}
