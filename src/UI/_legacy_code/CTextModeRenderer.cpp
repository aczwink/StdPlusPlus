//Class header
#include "../../headers/UI/CTextModeRenderer.h"
//Local
#include "Render Targets/ATextRenderTarget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
CTextModeRenderer::CTextModeRenderer(AWidget &refWidget) : refWidget(refWidget)
{
}

//Public methods
void CTextModeRenderer::WriteCharAt(uint16 x, uint16 y, uint32 codePoint)
{
	CPoint pt;
	
	//clipping
	if(x >= this->refWidget.GetSize().width || y >= this->refWidget.GetSize().height)
		return;

	pt = this->refWidget.TransformToWindow(CPoint(x, y));

	//render
	ATextRenderTarget &refRenderTarget = (ATextRenderTarget &)GetRenderTarget(ERenderMode::Text);

	refRenderTarget.OutputCodepointAt(pt, codePoint);
}