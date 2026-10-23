/*
	Copyright 1999, Be Incorporated.   All Rights Reserved.
	This file may be used under the terms of the Be Sample Code License.
*/

#ifndef __REPLIMANWINDOW_H__
#define __REPLIMANWINDOW_H__

#include <Application.h>
#include <FindDirectory.h>
#include <Messenger.h>
#include <Window.h>
#include <Shelf.h>
#include <ListView.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Button.h>
#include <Dragger.h>
#include <Entry.h>
#include <Box.h>
#include <ScrollView.h>
#include <MenuItem.h>
#include <ListItem.h>
#include <StringView.h>
#include <image.h>
#include <Debug.h>
#include <Mime.h>
#include <Node.h>
#include <NodeInfo.h>
#include <Path.h>
#include <String.h>
#include <Deskbar.h>
#include <ClassInfo.h>

/*------------------------------------------------------------*/

enum {
	M_SET_DESKBAR_TARGET		= 'sdes',
	M_SET_DESKTOP_TARGET		= 'strk',
	CMD_REMOVE_REPLICANT		= 'delr',
	CMD_TOGGLE_REPLICANT		= 'togr',
	CMD_REP_SELECTION_CHANGED	= 'rslc',
	CMD_RESIZE_TO_CONTENT		= 'rsiz',
	CMD_TICK					= 'tick',
	
	M_DESKBAR					= M_SET_DESKBAR_TARGET,
	M_DESKTOP					= M_SET_DESKTOP_TARGET
};
	
/*------------------------------------------------------------*/
class xListItem;

class replimanWindow : public BWindow {
public:
					replimanWindow();
					~replimanWindow();
protected:
	virtual void	MessageReceived(BMessage *msg);
	virtual bool	QuitRequested();

private:
	void			InitGUI();
	void			ResizeToContent();
	void			UpdateButtons();
	void			ShowError(const char *operation, status_t status) const;
	bool			ConfirmRemoval(const xListItem *item) const;

	// These are the interesting functions showing how you can 
	// interact via BMessages with a Shelf/Container view
	BMessenger		MessengerForTarget(type_code w) const;
	BMessenger		ContainerMessengerForTarget(type_code w) const;
	int32			GetReplicantAt(int32 index) const;
	status_t		GetReplicantName(int32 uid, BMessage *result) const;
	status_t		GetReplicantArchive(int32 uid, int32 location,
						BMessage *archive, BPoint *position) const;
	BBitmap	*		GetReplicantIcon(int32 uid, int32 location);
	BBitmap	*		GetIconFromArchive(const BMessage &archive) const;
	status_t		RemoveReplicant(int32 uid, int32 location);
	status_t		RestoreReplicant(const BMessage &archive, int32 location,
						BPoint position) const;
	bool			FindDesktopMessenger(BMessenger *messenger) const;

	status_t		LoadDisabledReplicants();
	status_t		SaveDisabledReplicants(BMessage &store) const;
	void			AppendDisabledReplicants();
	bool			FindDisabledReplicant(int64 storageID, BMessage *record,
						int32 *index) const;
	void			DisableReplicant(const xListItem *item);
	void			EnableReplicant(const xListItem *item);
	void			RemoveSelectedReplicant(const xListItem *item);
	
	bool			IsReplicantLoaded(int32 uid) const;
		
	void			UpdateList(bool make_empty);
	bool			UpdateQuasiList(int32 target, bool make_empty);
	void			EmptyLists();
		
	BListView		*fReplicantList;
	int				fDesktopCount;
	int				fDeskbarCount;
	bool			fHaveDesktopHeader;
	bool			fHaveDeskbarHeader;
	BMessenger		fTarget;
	BMessage		fDisabledReplicants;
	BMessageRunner	*fTickRunner;
	BButton			*fRemoveButton;
	BButton			*fDisableButton;
};
#endif
