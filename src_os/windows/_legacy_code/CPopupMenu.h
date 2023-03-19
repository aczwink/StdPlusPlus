//Global
#include <Windows.h>
//Local
#include "../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../headers/GUI/IPopupMenu.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::GUI;

/*class CPopupMenu : public IPopupMenu
{
public:
	//Members
	HMENU hMenu;
	CUTF16String name;
	uint8 id; //we only have a 16bit value to identify any menu item, we take 8 bit to identify the menu and another 8 bit to determine the item in that menu
	uint8 itemIdxCounter;
	CArray<CFunction<void()>> eventHandlers;
	
	//Constructor
	inline CPopupMenu(const CUTF16String &refName)
	{
		static uint8 idCounter = 0;
		
		this->hMenu = CreatePopupMenu();
		this->name = refName;
		this->id = idCounter++;
		this->itemIdxCounter = 0;
	}

	//Destructor
	inline ~CPopupMenu()
	{
		if(this->hMenu)
		{
			DestroyMenu(this->hMenu);
		}
	}

	//Inline
	inline void AppendItem(const CUTF8String &refName, const CFunction<void()> &refEventHandler)
	{
		const CUTF16String &refNameUTF16 = refName;
		
		AppendMenuW(this->hMenu, MF_STRING, MAKE16(this->itemIdxCounter++, this->id), (LPCWSTR)refNameUTF16.GetC_Str());
		this->eventHandlers.Push(refEventHandler);
	}

	inline void AppendSeperator()
	{
		AppendMenuW(this->hMenu, MF_SEPARATOR, NULL, NULL);
	}
	
	inline void ClearHandles()
	{
		this->hMenu = nullptr;
	}

	inline void DispatchEvent(uint8 itemIdx)
	{
		this->eventHandlers[itemIdx]();
	}

	inline uint8 GetId() const
	{
		return this->id;
	}
};*/