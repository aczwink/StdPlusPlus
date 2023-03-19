#pragma once
//Local
#include "../IRenderTarget.h"

namespace ACStdLib
{
	namespace TUI
	{
		class ACSTDLIB_API CConsoleRenderTarget : public IRenderTarget
		{
		private:
			//Members
			void *pFrontBuffer;
			void *pBackBuffer;
			CColor backgroundColor;
			CColor foregroundColor;
		public:
			//Constructor
			CConsoleRenderTarget();

			//Methods
			void Clear();
			uint16 GetNumberOfColumns() const;
			uint16 GetNumberOfRows() const;
			void OutputCodePoint(uint16 x, uint16 y, uint32 codePoint);
			bool PeekEvent(CTuple<EEventType, void *> &refEvent);
			void Present();
			void ResizeView();
			void SetBackgroundColor(const CColor &refColor);
			void SetCursorPos(uint16 x, uint16 y);
			void SetForegroundColor(const CColor &refColor);
			void ShowCursor(bool show);
		};
	}
}