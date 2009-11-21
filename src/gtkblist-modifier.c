/*
* Pidgin Protocol Status Icons
* Copyright (C) 2009 Konrad Gräfe
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1301, USA.
*/

#include "config.h"
#include "internal.h"

#include <gtkblist.h>

#include "gtkblist-modifier.h"
#include "icons.h"

typedef enum {
	PIDGIN_BLIST_NODE_HAS_PENDING_MESSAGE    =  1 << 0,  /* Whether there's pending message in a conversation */
} PidginBlistNodeFlags;

typedef struct _pidgin_blist_node {
	GtkTreeRowReference *row;
	gboolean contact_expanded;
	gboolean recent_signonoff;
	gint recent_signonoff_timer;
	struct {
		PurpleConversation *conv;
		time_t last_message;          /* timestamp for last displayed message */
		PidginBlistNodeFlags flags;
	} conv;
} PidginBlistNode;

static int row_changed_handler_id = -1;
static PidginBuddyList *gtkblist = NULL;

static void row_changed_cb(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
	static gboolean inuse = FALSE;
	PurpleBlistNode *node;
	PurpleBuddy *buddy;
	PidginBlistNode *gtkbuddynode;
	PurpleAccount *account;
	PurplePresence *presence;
	gint status;
	const gchar *protocol_id;
	GdkPixbuf *new_icon;

	if(inuse) return;

	gtk_tree_model_get(model, iter, NODE_COLUMN, &node, -1);

	if(PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		buddy = (PurpleBuddy *)node;
	} else if(PURPLE_BLIST_NODE_IS_CONTACT(node)) {
		buddy = purple_contact_get_priority_buddy((PurpleContact *)node);
	} else {
		return;
	}
	
	if(!buddy) return;
	gtkbuddynode = ((PurpleBlistNode *)buddy)->ui_data;

	/** get protocol id **/
	account = purple_buddy_get_account(buddy);
	if(!account) return;
	
	protocol_id = purple_account_get_protocol_id(account);
	if(!protocol_id) return;
	
	/** get status **/
	presence = purple_buddy_get_presence(buddy);
	if(!presence) return;
	
	if(gtkbuddynode && gtkbuddynode->recent_signonoff) {
		return; /* don't touch */
	} else if(purple_presence_is_status_primitive_active(presence, PURPLE_STATUS_UNAVAILABLE)) {
		status = PURPLE_STATUS_UNAVAILABLE;
	} else if (purple_presence_is_status_primitive_active(presence, PURPLE_STATUS_AWAY)) {
		status = PURPLE_STATUS_AWAY;
	} else if (purple_presence_is_status_primitive_active(presence, PURPLE_STATUS_EXTENDED_AWAY)) {
		status = PURPLE_STATUS_EXTENDED_AWAY;
	} else if (purple_presence_is_status_primitive_active(presence, PURPLE_STATUS_OFFLINE)) {
		status = PURPLE_STATUS_OFFLINE;
	} else if (purple_presence_is_status_primitive_active(presence, PURPLE_STATUS_INVISIBLE)) {
		status = PURPLE_STATUS_INVISIBLE;
	} else {
		status = PURPLE_STATUS_AVAILABLE;
	}
	
	new_icon = get_icon(protocol_id, status);
	if(!new_icon) return;
	
	inuse = TRUE;
	
	gtk_tree_store_set(GTK_TREE_STORE(model), iter, STATUS_ICON_COLUMN, new_icon, STATUS_ICON_VISIBLE_COLUMN, TRUE, -1);

	inuse = FALSE;
}

static void gtkblist_created_cb(PurpleBuddyList *blist) {
	gtkblist = PIDGIN_BLIST(blist);
	
	row_changed_handler_id = g_signal_connect(gtkblist->treemodel, "row_changed", G_CALLBACK(row_changed_cb), NULL);

	pidgin_blist_refresh(blist);
}

void gtkblist_modifier_init(PurplePlugin *plugin) {
	purple_signal_connect(pidgin_blist_get_handle(), "gtkblist-created", plugin, PURPLE_CALLBACK(gtkblist_created_cb), NULL);
	if(pidgin_blist_get_default_gtk_blist()) gtkblist_created_cb(purple_get_blist());
}

void gtkblist_modifier_uninit(void) {
	if(g_signal_handler_is_connected(gtkblist->treemodel, row_changed_handler_id))
		g_signal_handler_disconnect(gtkblist->treemodel, row_changed_handler_id);
}

