// xListItem.h: definition of an icon and text BListItem

#ifndef __xLISTITEM_H__
#define __xLISTITEM_H__

#include <ListItem.h>
#include <Bitmap.h>
#include <String.h>
#include <View.h>

class xListItem : public BListItem {
public:
					xListItem(const char* name, BBitmap *icon, int32 i, int32 location);
					~xListItem();

			int32	GetID() const;
			int32	GetLocation() const;
	const char*		GetName() const;
protected:
	virtual void	DrawItem(BView *owner, BRect frame, bool complete = false);
	virtual void	Update(BView *owner, const BFont *font);
private:
	BString			fName;
	BBitmap			*fIcon;
	int32			fID;
	int32			fLocation;
	float 			actual_height;
	int				text_pos;
	int				bitmap_pos;
};
#endif
// ----------------------------------------------------------- xListItem.h --
