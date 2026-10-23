// xHeaderListItem.cpp: implementation of the xHeaderListItem class

#ifndef __xHEADERLISTITEM_H__
#include "xHeaderListItem.h"
#endif

#include <math.h>

xHeaderListItem::xHeaderListItem(const char* name)
	: BListItem(),
	  fName(name != NULL ? name : "")
{
	SetEnabled(false);
}
//---------------------------------------------------------------------------------------------------------------------------------//

void
xHeaderListItem::DrawItem(BView *owner, BRect frame, bool)
{
	owner->PushState();
	owner->SetLowUIColor(B_CONTROL_BACKGROUND_COLOR);
	owner->FillRect(frame, B_SOLID_LOW);

	owner->SetHighUIColor(B_CONTROL_TEXT_COLOR);
	owner->SetFont(be_bold_font);

	owner->MovePenTo(frame.left + 4, frame.bottom - text_pos);
	owner->DrawString(fName.String());
	owner->PopState();
}
//---------------------------------------------------------------------------------------------------------------------------------//

void
xHeaderListItem::Update(BView *, const BFont *)
{
	font_height height;
	
	be_bold_font->GetHeight(&height);
	actual_height = ceil(height.ascent + height.descent + height.leading);
	
//	printf("heights are: ascent: %.2f, descent: %.2f, leading: %.2f\n",
	//		height.ascent, height.descent, height.leading);
	SetHeight(actual_height+2);
	SetWidth(be_bold_font->StringWidth(fName.String()) + 8.0f);
		
	// calculate the positions of the bitmap and the text.
	text_pos = ceil(height.descent);//(Height() - actual_height) / 2;
}
//---------------------------------------------------------------------------------------------------------------------------------//
