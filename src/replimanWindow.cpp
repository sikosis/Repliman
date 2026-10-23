// replimanWindow.cpp

#ifndef __REPLIMANWINDOW_H__
#include "replimanWindow.h"
#endif

#ifndef __xLISTITEM_H__
#include "xListItem.h"
#endif

#ifndef __xHEADERLISTITEM_H__
#include "xHeaderListItem.h"
#endif

#include <LayoutBuilder.h>
#include <Screen.h>

/*------------------------------------------------------------*/
struct match_info {
				match_info(image_id i, int32 loc)
					: id(i),
					  location(loc),
					  found(false)
				{
				}
				//---------------------------------------------------------------------------------------------------------------------------------//

	image_id	id;
	int32		location;
	bool		found;
};

bool match_id(BListItem *item, void *data)
{
	match_info *mi = (match_info *) (data);

//	printf("match_id: %s\n", class_name(item));
	if (!(is_instance_of(item, xListItem))) {
//		printf("not an xListItem (%s)\n", class_name(item));
		return false;
	}

	xListItem *my = dynamic_cast<xListItem*>(item);
	if (my->GetID() == (mi)->id && my->GetLocation() == (mi)->location) {
		(mi)->found = true;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/

replimanWindow::replimanWindow()
	: BWindow(BRect(100, 50, 500, 350), "Repliman", B_TITLED_WINDOW,
		0),
	  fReplicantList(NULL),
	  fDesktopCount(0),
	  fDeskbarCount(0),
	  fHaveDesktopHeader(false),
	  fHaveDeskbarHeader(false),
	  fTickRunner(NULL),
	  fDeleteButton(NULL)
{
	InitGUI();

	BMessage tickMessage(CMD_TICK);
	fTickRunner = new BMessageRunner(BMessenger(this), &tickMessage, 1000000);
	
	UpdateList(true);
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/
replimanWindow::~replimanWindow()
{
	delete fTickRunner;
	EmptyLists();
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

bool
replimanWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------//

#pragma mark -

void
replimanWindow::InitGUI()
{
	fReplicantList = new BListView("rep_list", B_SINGLE_SELECTION_LIST);
	fReplicantList->SetViewUIColor(B_LIST_BACKGROUND_COLOR);
	fReplicantList->SetSelectionMessage(new BMessage(CMD_REP_SELECTION_CHANGED));

	BScrollView *scrollView = new BScrollView("rep_scroll_view",
		fReplicantList, 0, false, true, B_FANCY_BORDER);

	fDeleteButton = new BButton("delete", "Delete",
		new BMessage(CMD_DELETE_REPLICANT));
	fDeleteButton->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 10.0f)
		.SetInsets(10.0f)
		.Add(scrollView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fDeleteButton)
		.End()
	.End();

	SetSizeLimits(300.0f, B_SIZE_UNLIMITED, 200.0f, B_SIZE_UNLIMITED);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void
replimanWindow::ResizeToContent()
{
	Layout(true);

	float contentWidth = 0.0f;
	float contentHeight = 40.0f;
	int32 itemCount = fReplicantList->CountItems();

	for (int32 index = 0; index < itemCount; index++) {
		BListItem *item = fReplicantList->ItemAt(index);
		if (item != NULL && item->Width() > contentWidth)
			contentWidth = item->Width();
	}

	if (itemCount > 0)
		contentHeight = fReplicantList->ItemFrame(itemCount - 1).bottom + 1.0f;

	float extraWidth = Bounds().Width() - fReplicantList->Bounds().Width();
	float extraHeight = Bounds().Height() - fReplicantList->Bounds().Height();
	float targetWidth = contentWidth + extraWidth + 12.0f;
	float targetHeight = contentHeight + extraHeight + 4.0f;

	if (targetWidth < 300.0f)
		targetWidth = 300.0f;
	if (targetHeight < 200.0f)
		targetHeight = 200.0f;

	BScreen screen(this);
	if (screen.IsValid()) {
		BRect screenFrame = screen.Frame();
		float maximumWidth = screenFrame.Width() * 0.9f;
		float maximumHeight = screenFrame.Height() * 0.9f;

		if (targetWidth > maximumWidth)
			targetWidth = maximumWidth;
		if (targetHeight > maximumHeight)
			targetHeight = maximumHeight;
	}

	ResizeTo(targetWidth, targetHeight);

	if (screen.IsValid()) {
		BRect screenFrame = screen.Frame();
		BRect windowFrame = Frame();
		float horizontalOffset = 0.0f;
		float verticalOffset = 0.0f;

		if (windowFrame.right > screenFrame.right)
			horizontalOffset = screenFrame.right - windowFrame.right;
		if (windowFrame.bottom > screenFrame.bottom)
			verticalOffset = screenFrame.bottom - windowFrame.bottom;
		if (windowFrame.left + horizontalOffset < screenFrame.left)
			horizontalOffset = screenFrame.left - windowFrame.left;
		if (windowFrame.top + verticalOffset < screenFrame.top)
			verticalOffset = screenFrame.top - windowFrame.top;

		if (horizontalOffset != 0.0f || verticalOffset != 0.0f)
			MoveBy(horizontalOffset, verticalOffset);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//
/*------------------------------------------------------------*/

void
replimanWindow::MessageReceived(BMessage *msg)
{		
	switch (msg->what) {
		case CMD_TICK: {
			UpdateList(false);
			break;
		}
		case CMD_DELETE_REPLICANT: {
			int32	sel = fReplicantList->CurrentSelection();
			xListItem *item = dynamic_cast<xListItem*>(fReplicantList->ItemAt(sel));
			if (sel < 0 || item == NULL)
				break;
//			printf("deleting %s with id = %ld\n", item->GetName(), item->GetID());
//			printf("after delete %s\n", strerror(DeleteReplicant(item->GetID(), item->GetLocation())));
			DeleteReplicant(item->GetID(), item->GetLocation());
			break;
		}
		case CMD_REP_SELECTION_CHANGED: {
			bool	enabled;
			enabled = (fReplicantList->CurrentSelection() >= 0);
			fDeleteButton->SetEnabled(enabled);
			break;
		}
		case CMD_RESIZE_TO_CONTENT: {
			ResizeToContent();
			break;
		}
		default:
			BWindow::MessageReceived(msg);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/
#pragma mark -

void
replimanWindow::EmptyLists()
{
	while (!fReplicantList->IsEmpty())
		delete fReplicantList->RemoveItem((int32)0);

	fHaveDesktopHeader = fHaveDeskbarHeader = false;
	fDesktopCount = fDeskbarCount = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------//

void
replimanWindow::UpdateList(bool make_empty)
{
	bool result;
	
	if (make_empty)
		EmptyLists();
		
	result = UpdateQuasiList(M_DESKTOP, make_empty);
	result = UpdateQuasiList(M_DESKBAR, make_empty) || result;
	
	if (result || make_empty) {
		PostMessage(CMD_REP_SELECTION_CHANGED);
		PostMessage(CMD_RESIZE_TO_CONTENT);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//

// 2004/10/10 - I would think that it is better if this
// function handled better the case in which the tracker
// or deskbar is not loaded
bool
replimanWindow::UpdateQuasiList(int32 target, bool make_empty)
{
	bool	listChanged = false;
	int32	index = 0;
	int32	uid;
	
	// first we set the target
	fTarget = MessengerForTarget(target);

	// if the target does not exist then remove the items that were there
	if (!fTarget.IsValid()) {
//		printf("target is not valid\n");
		int32 count, pos;
		
		if (target == M_DESKTOP) {
			count = (fHaveDesktopHeader ? fDesktopCount+1 : 0);
			pos = 0;
			fHaveDesktopHeader = false;
			fDesktopCount = 0;
		} else {
			count = (fHaveDeskbarHeader ? fDeskbarCount+1 : 0);
			pos = (fHaveDesktopHeader ? fDesktopCount+1 : fDesktopCount);
			fHaveDeskbarHeader = false;
			fDeskbarCount = 0;
		}
		
		for (int32 i = 0; i < count; i++)
			delete fReplicantList->RemoveItem(pos);
			
		listChanged = count > 0;

		return listChanged;
	}
	
	if (!make_empty) {
		// walk through the current list of replicants and remove any that are
		// no longer loaded
		xListItem 	*item;
		int32 start, count;
		
		if (target == M_DESKTOP) {
			start = 1;
			count = fDesktopCount;
		} else {
			start = (fHaveDesktopHeader ? fDesktopCount+1 : 0);
			
			if (fHaveDeskbarHeader)
				start++;
				
			count = fDeskbarCount;
		}
		
//		printf("start = %ld, count = %ld\n", start, count);
		int32 offset = 0;
		while (offset < count) {
			item = dynamic_cast<xListItem*>(fReplicantList->ItemAt(start + offset));
			if (item == NULL) {
				offset++;
				continue;
			}

			int32 uid = item->GetID();
			if (IsReplicantLoaded(uid) == false) {
				fReplicantList->RemoveItem(item);
				delete item;
				listChanged = true;
				if (target == M_DESKTOP)
					fDesktopCount--;
				else
					fDeskbarCount--;
				count--;
//				printf("x");
			} else {
				offset++;
			} //else
//				printf(".");
		}
		
		// if there are no items but still the header delete it
		if (target == M_DESKTOP) {
			if (fDesktopCount == 0 && fHaveDesktopHeader) {
				delete fReplicantList->RemoveItem((int32)0);
				fHaveDesktopHeader = false;
				listChanged = true;
			}
		} else {
			if (fDeskbarCount == 0 && fHaveDeskbarHeader) {
				int32 pos = (fHaveDesktopHeader ? fDesktopCount + 1 : 0);
				delete fReplicantList->RemoveItem(pos);
				fHaveDeskbarHeader = false;
				listChanged = true;
			}
		}
	}
	
	// Now get all the replicants from the shelf and make sure that they are in
	// the list
	while ((uid = GetReplicantAt(index++)) >= B_OK) {
		// if this uid is already in the list then skip it
		match_info	mi(uid, target);
		fReplicantList->DoForEach(match_id, &mi);
		if (mi.found) {
			continue;
		}
		
		BMessage	rep_info;
		if (GetReplicantName(uid, &rep_info) != B_OK) {
			continue;
		}
		const char *name;
		if (rep_info.FindString("result", &name) == B_OK) {
			int32 pos;
			BBitmap *icon;
			icon = GetReplicantIcon(uid);

			if (target == M_DESKTOP) {
				pos = (fHaveDesktopHeader ? fDesktopCount+1 : 0);			
			} else {
				pos = (fHaveDesktopHeader ? fDesktopCount+1 : 0);			
				pos += (fHaveDeskbarHeader ? fDeskbarCount+1 : 0);			
			}
			
			fReplicantList->AddItem(new xListItem(name, icon, uid, target), pos);
			delete icon;
			listChanged = true;
			if (target == M_DESKTOP)
				fDesktopCount++;
			else
				fDeskbarCount++;
		}
	}

	// we should add the header if it does not exist
	if (target == M_DESKTOP) {
		if (!fHaveDesktopHeader && fDesktopCount != 0) {
			fReplicantList->AddItem(new xHeaderListItem("Desktop"), 0);
			fHaveDesktopHeader = true;
			listChanged = true;
		}
	} else {
		if (!fHaveDeskbarHeader && fDeskbarCount != 0) {
			int32 pos = (fHaveDesktopHeader ? fDesktopCount + 1 : 0);
			fReplicantList->AddItem(new xHeaderListItem("Deskbar"), pos);
			fHaveDeskbarHeader = true;
			listChanged = true;
		}
	}
	
	return listChanged;
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/
#pragma mark -

status_t
replimanWindow::GetReplicantName(int32 uid, BMessage *reply) const
{
	/*
	 We send a message to the target shelf, asking it for the Name of the 
	 replicant with the given unique id.
	 */
	 
	BMessage	request(B_GET_PROPERTY);
	BMessage	uid_specifier(B_ID_SPECIFIER);	// specifying via ID
	status_t	err;
	status_t	e;
	
	request.AddSpecifier("Name");		// ask for the Name of the replicant
	
	// IDs are specified using code like the following 3 lines:
	uid_specifier.AddInt32("id", uid);
	uid_specifier.AddString("property", "Replicant");
	request.AddSpecifier(&uid_specifier);
	
	if ((err = fTarget.SendMessage(&request, reply)) != B_OK)
		return err;
	
	if (((err = reply->FindInt32("error", &e)) != B_OK) || (e != B_OK))
		return err ? err : e;
	
	return B_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------//

BBitmap *
replimanWindow::GetReplicantIcon(int32 uid)
{
	BMessage	request(B_GET_PROPERTY);		// Get will return the archive msg
	BMessage	uid_specifier(B_ID_SPECIFIER);	// specifying via ID
	BMessage	reply;
	status_t	err;
	status_t	e;
			
	// IDs are specified using code like the following 3 lines:
	uid_specifier.AddInt32("id", uid);
	uid_specifier.AddString("property", "Replicant");
	request.AddSpecifier(&uid_specifier);
	
	if ((err = fTarget.SendMessage(&request, &reply)) != B_OK)
		return NULL;
	
	if (((err = reply.FindInt32("error", &e)) != B_OK) || (e != B_OK))
		return NULL;
	
	// OK, let's get the archive message
	BMessage	data;
	reply.FindMessage("result", &data);

	BBitmap *icon = new BBitmap(BRect(0,0,15,15), B_CMAP8);
	// what is the signature of this class?
	const char *name;
	if (data.FindString("add_on", &name) == B_OK) {
		entry_ref ref;
		BMimeType mimetype(name);
//		printf("add_on = %s and %s\n", name, appInfo.ref.name);
//		BNode node(&appInfo.ref);
//		BNodeInfo nodeinfo(&node);
		status_t status = mimetype.GetAppHint(&ref);
		if (status == B_OK)
			status = BNodeInfo::GetTrackerIcon(&ref, icon, B_MINI_ICON);

		if (status != B_OK) {
//			printf("problem getting icon %s\n", strerror(status));
			// so let us load the zombie icon!
			size_t size;
			const void *resourceData;
	
			BResources *res = be_app->AppResources();
			resourceData = res != NULL
				? res->LoadResource('BBMP', "Zombie", &size) : NULL;
			if (resourceData != NULL && size > 0) {
				BMessage archive('vivi');
				if (archive.Unflatten((const char *)resourceData) == B_OK) {
					BBitmap *zombieIcon = new BBitmap(&archive);
					if (zombieIcon->InitCheck() == B_OK) {
						delete icon;
						icon = zombieIcon;
					} else {
						delete zombieIcon;
					}
				}
			}
		}
	} //else { // we do not know the app_signature or icon...

	return icon;
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

bool
replimanWindow::IsReplicantLoaded(int32 uid) const
{
	/*
	 determine if the specified replicant (the unique ID of the replicant)
	 still exists in the target container/shelf. If we can get the name then the 
	 replicant still exists.
	 */
	BMessage	reply;	
	status_t	err = GetReplicantName(uid, &reply);
	return (err == B_OK);
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

int32
replimanWindow::GetReplicantAt(int32 index) const
{
	/*
	 So here we want to get the Unique ID of the replicant at the given index
	 in the target Shelf.
	 */
	 
	BMessage	request(B_GET_PROPERTY);		// We're getting the ID property
	BMessage	reply;
	status_t	err;
	
	request.AddSpecifier("ID");					// want the ID
	request.AddSpecifier("Replicant", index);	// of the index'th replicant
	
	if ((err = fTarget.SendMessage(&request, &reply)) != B_OK)
		return err;
	
	int32	uid;
	if ((err = reply.FindInt32("result", &uid)) != B_OK) 
		return err;
	
	return uid; 
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

bool
replimanWindow::FindDesktopMessenger(BMessenger *messenger) const
{
	BPath deskpath;
	BEntry deskentry;
	entry_ref deskref;

	find_directory(B_DESKTOP_DIRECTORY, &deskpath, true);
	if (deskentry.SetTo(deskpath.Path(), true) != B_OK)
		return false;

	if (deskentry.GetRef(&deskref) != B_OK)
		return false;

	BMessage	reply;
	BMessage	request(B_GET_PROPERTY);
	BMessenger	desktopmsng;
	BMessenger	trackermsng("application/x-vnd.Be-TRAK", -1);

	request.AddRef("data", &deskref);
	request.AddSpecifier("Folder");

	if (trackermsng.SendMessage(&request, &reply) != B_OK)
		return false;

	if (reply.FindMessenger("window", messenger) != B_OK)
		return false;

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

BMessenger
replimanWindow::MessengerForTarget(type_code w) const
{
	/*
	 This function determines the BMessenger to the various Shelf objects
	 that this app can talk with.
	 */
	BMessage	request(B_GET_PROPERTY);
	BMessenger	to;
	BMessenger	result;

	request.AddSpecifier("Messenger");
	request.AddSpecifier("Shelf");

	switch (w) {
		case M_SET_DESKBAR_TARGET: {
			// In the Deskbar the Shelf is in the View "Status" in Window "Deskbar"
			request.AddSpecifier("View", "Status");
			request.AddSpecifier("Window", "Deskbar");
			to = BMessenger("application/x-vnd.Be-TSKB", -1);
			break;
		}
		case M_SET_DESKTOP_TARGET: {
			// The Desktop is owned by Tracker and the Shelf is in the
			// View "PoseView" in Window "Desktop"
			request.AddSpecifier("View", "PoseView");
			if (FindDesktopMessenger(&to) == false)
			{
				// Window 0 works also fine,
				// but it is undocumented feature that may be changed in the future
				request.AddSpecifier("Window", int32(0));
				to = BMessenger("application/x-vnd.Be-TRAK", -1);
			}
			break;
		}
	}
	
	BMessage	reply;
	
	if (to.SendMessage(&request, &reply) == B_OK) {
		reply.FindMessenger("result", &result);
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------------------------------//

/*------------------------------------------------------------*/

status_t
replimanWindow::DeleteReplicant(int32 uid, int32 location)
{
	status_t status;
	
//	printf("deleting replicant\n");
	// delete the given replicant from the deskbar
	if (location == M_DESKBAR) {
		BDeskbar desk;
		status = desk.RemoveItem(uid);
	} else {
		BMessage	request(B_DELETE_PROPERTY);		// Delete
		BMessage	uid_specifier(B_ID_SPECIFIER);	// specifying via ID
		BMessage	reply;
		status_t	err;
		status_t	deleteStatus;

		fTarget = MessengerForTarget(M_SET_DESKTOP_TARGET);

		// specify the id of the replicant
		uid_specifier.AddInt32("id", uid);
		uid_specifier.AddString("property", "Replicant");
		request.AddSpecifier(&uid_specifier);
	
		if ((err = fTarget.SendMessage(&request, &reply)) != B_OK)
			return err;
	
		if ((err = reply.FindInt32("error", &deleteStatus)) != B_OK)
			return err;

		status = deleteStatus;
	
	}

	return status;
}
//---------------------------------------------------------------------------------------------------------------------------------//
