// xListItem.h: definition of an icon and text BListItem

#ifndef __xLISTHEADERITEM_H__
#define __xLISTHEADERITEM_H__

#include <ListItem.h>
#include <View.h>
#include <Bitmap.h>
#include <String.h>

class xHeaderListItem : public BListItem {
public:
					xHeaderListItem(const char* name);

protected:
	virtual void	DrawItem(BView *owner, BRect frame, bool complete = false);
	virtual void	Update(BView *owner, const BFont *font);
private:
	BString			fName;
	float 			actual_height;
	int				text_pos;
};
#endif
// ------------------------------------------------------- xHeaderListItem.h --
