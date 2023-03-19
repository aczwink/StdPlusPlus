//Public methods
void CWindow::EnableDrop()
{
	if(!this->pOSDropTarget)
	{
		this->pOSDropTarget = new CDropTarget(this);
		RegisterDragDrop((HWND)this->pOSHandle, this->pOSDropTarget);
	}
}

void CWindow::ShowErrorBox(const CString &refTitle, const CString &refMessage)
{
	CUTF16String title, message;

	title = refTitle.GetUTF16();
	message = refMessage.GetUTF16();

	MessageBoxW((HWND)this->pOSHandle, (LPCWSTR)message.GetC_Str(), (LPCWSTR)title.GetC_Str(), MB_OK | MB_ICONERROR);
}

void CWindow::ShowInformationBox(const CString &refTitle, const CString &refMessage)
{
	CUTF16String title, message;

	title = refTitle.GetUTF16();
	message = refMessage.GetUTF16();

	MessageBoxW((HWND)this->pOSHandle, (LPCWSTR)message.GetC_Str(), (LPCWSTR)title.GetC_Str(), MB_OK | MB_ICONINFORMATION);
}
