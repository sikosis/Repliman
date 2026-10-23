// xListItem.cpp: implementation of the xListItem class

#ifndef __xLISTITEM_H__
#include "xListItem.h"
#endif

#include <math.h>

xListItem::xListItem(const char* name, BBitmap *icon, int32 i, int32 location)
	: BListItem(),
	  fName(name != NULL ? name : "Unnamed replicant"),
	  fIcon(icon != NULL ? new BBitmap(icon) : NULL),
	  fID(i),
	  fLocation(location)
{
}
//---------------------------------------------------------------------------------------------------------------------------------//


xListItem::~xListItem()
{
	delete fIcon;
}
//---------------------------------------------------------------------------------------------------------------------------------//


int32
xListItem::GetID() const
{
	return fID;
}
//---------------------------------------------------------------------------------------------------------------------------------//


int32
xListItem::GetLocation() const
{
	return fLocation;
}
//---------------------------------------------------------------------------------------------------------------------------------//


const char*
xListItem::GetName() const
{
	return fName.String();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void
xListItem::DrawItem(BView *owner, BRect frame, bool)
{
	owner->PushState();

	if (IsSelected()) {
		owner->SetLowUIColor(B_LIST_SELECTED_BACKGROUND_COLOR);
		owner->SetHighUIColor(B_LIST_SELECTED_ITEM_TEXT_COLOR);
	} else {
		owner->SetLowUIColor(B_LIST_BACKGROUND_COLOR);
		owner->SetHighUIColor(B_LIST_ITEM_TEXT_COLOR);
	}
			
	owner->FillRect(frame, B_SOLID_LOW);

	if (fIcon != NULL) {
		owner->SetDrawingMode(B_OP_ALPHA);
		owner->DrawBitmap(fIcon, BPoint(frame.left + 4,frame.top + bitmap_pos));
		owner->SetDrawingMode(B_OP_COPY);
	}

	owner->MovePenTo(frame.left + 28, frame.bottom - text_pos);


	owner->DrawString(fName.String());
	owner->PopState();
}
//---------------------------------------------------------------------------------------------------------------------------------//

void
xListItem::Update(BView *, const BFont *font)
{
	font_height height;
	
//	BListItem::Update(owner, font);
	
	font->GetHeight(&height);
	actual_height = ceil(height.ascent + height.descent);
	
	// set the new height for the menu item
	if (actual_height < 20)
		SetHeight(20);
	else
		SetHeight(actual_height);
	SetWidth(font->StringWidth(fName.String()) + 36.0f);
		
	// calculate the positions of the bitmap and the text.
	text_pos = ceil(height.descent) + 2;
	bitmap_pos = (Height() - 16) / 2;
}
//---------------------------------------------------------------------------------------------------------------------------------//
