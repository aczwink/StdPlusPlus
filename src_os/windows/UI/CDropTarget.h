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
//Global
#include <Windows.h>
//Local
#include "../../../headers/UI/CWindow.h"
#include "../../../headers/UI/Transfer/CFileTransfer.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

#include "../../../headers/Streams/CStdOut.h"

namespace ACStdLib
{
	namespace UI
	{
		class CDropTarget : public IDropTarget
		{
		private:
			//Members
			uint32 referenceCounter;
			CWindow *pWnd;
			ITransfer *pTransfer;

			//Inline
			inline DWORD DropTypeToNative(EDropType dropType)
			{
				switch(dropType)
				{
				case EDropType::Copy:
					return DROPEFFECT_COPY;
				case EDropType::Link:
					return DROPEFFECT_LINK;
				case EDropType::Move:
					return DROPEFFECT_MOVE;
				}

				return DROPEFFECT_NONE;
			}

			inline void FindTransferType(IDataObject *pDataObj)
			{
				FORMATETC formatEtc;
				IEnumFORMATETC *pEnumerator;
				STGMEDIUM medium;

				if(this->pTransfer)
				{
					delete this->pTransfer;
					this->pTransfer = nullptr;
				}

				pDataObj->EnumFormatEtc(DATADIR_GET, &pEnumerator);
				while(pEnumerator->Next(1, &formatEtc, nullptr) == S_OK)
				{
					switch(formatEtc.cfFormat)
					{
					case CF_HDROP:
						pDataObj->GetData(&formatEtc, &medium);
						this->MapFileTransfer((HDROP)medium.hGlobal);
						goto end;
					}
				}
			end:;
				ReleaseStgMedium(&medium);
				pEnumerator->Release();
			}

			inline void MapFileTransfer(HDROP hDrop)
			{
				uint32 nFiles;
				wchar_t buffer[4096];
				CFileTransfer *pFileTransfer;
				CString fileName;

				pFileTransfer = new CFileTransfer;

				nFiles = DragQueryFileW(hDrop, UINT32_MAX, nullptr, 0);
				for(uint32 i = 0; i < nFiles; i++)
				{
					DragQueryFileW(hDrop, i, buffer, sizeof(buffer) / sizeof(buffer[0]));
					fileName = CUTF16String((uint16 *)buffer);

					pFileTransfer->files.InsertTail(fileName);
				}

				this->pTransfer = pFileTransfer;
			}

		public:
			//Constructor
			inline CDropTarget(CWindow *pWnd)
			{
				OleInitialize(nullptr);
				this->referenceCounter = 1;
				this->pWnd = pWnd;
				this->pTransfer = nullptr;
			}

			//Destructor
			inline ~CDropTarget()
			{
				if(this->pTransfer)
				{
					delete this->pTransfer;
					this->pTransfer = nullptr;
				}

				OleUninitialize();
			}

			//Inline
			inline HRESULT QueryInterface(REFIID riid, void **ppvObject)
			{
				if(riid == IID_IUnknown || riid == IID_IDropTarget)
				{
					*ppvObject = this;
					this->AddRef();
					return S_OK;
				}
				*ppvObject = nullptr;
				return E_NOINTERFACE;
			}

			inline ULONG AddRef()
			{
				return InterlockedIncrement(&this->referenceCounter);
			}

			inline ULONG Release()
			{
				uint32 c;

				c = InterlockedDecrement(&this->referenceCounter);
				if(c == 0)
					delete this;

				return c;
			}

			inline HRESULT DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
			{
				EDropType dropType;

				this->FindTransferType(pDataObj);

				dropType = this->pWnd->OnDragEnter(*this->pTransfer);
				*pdwEffect = this->DropTypeToNative(dropType);

				return S_OK;
			}

			inline HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
			{
				EDropType dropType;

				dropType = this->pWnd->OnDragMove();
				*pdwEffect = this->DropTypeToNative(dropType);

				return S_OK;
			}

			inline HRESULT DragLeave()
			{
				this->pWnd->OnDragLeave();

				return S_OK;
			}

			inline HRESULT Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
			{
				this->pWnd->OnDrop(*this->pTransfer);

				return S_OK;
			}
		};
	}
}