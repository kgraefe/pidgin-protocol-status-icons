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

static int row_changed_handler_id = -1;
static PidginBuddyList *gtkblist = NULL;

static void row_changed_cb(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
	static gboolean inuse = FALSE;
	PurpleBlistNode *node;

	if(inuse) return;

	gtk_tree_model_get(model, iter, NODE_COLUMN, &node, -1);

	if(!PURPLE_BLIST_NODE_IS_BUDDY(node) && !PURPLE_BLIST_NODE_IS_CONTACT(node)) return;

	inuse = TRUE;
	
	gtk_tree_store_set(GTK_TREE_STORE(model), iter, STATUS_ICON_COLUMN, hello_kitty, STATUS_ICON_VISIBLE_COLUMN, (hello_kitty != NULL), -1);

	inuse = FALSE;
}

static void gtkblist_created_cb(PurpleBuddyList *blist) {
	gtkblist = PIDGIN_BLIST(blist);
	
	row_changed_handler_id = g_signal_connect(gtkblist->treemodel, "row_changed", G_CALLBACK(row_changed_cb), NULL);

	pidgin_blist_refresh(blist);
}

void gtkblist_modifier_connect(PurplePlugin *plugin) {
	purple_signal_connect(pidgin_blist_get_handle(), "gtkblist-created", plugin, PURPLE_CALLBACK(gtkblist_created_cb), NULL);
	if(pidgin_blist_get_default_gtk_blist()) gtkblist_created_cb(purple_get_blist());
}

void gtkblist_modifier_disconnect(void) {
	if(g_signal_handler_is_connected(gtkblist->treemodel, row_changed_handler_id))
		g_signal_handler_disconnect(gtkblist->treemodel, row_changed_handler_id);
}
