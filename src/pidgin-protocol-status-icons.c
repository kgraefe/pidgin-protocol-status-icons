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

#ifndef PURPLE_PLUGINS
#define PURPLE_PLUGINS
#endif

#include "internal.h"

#include <plugin.h>
#include <version.h>
#include <debug.h>
#include <core.h>

#include <gtkplugin.h>
#include <gtkblist.h>

#define PLUGIN_PREFS_PREFIX "/plugins/gtk/protocol-status-icons"


static PurplePlugin *plugin;
static PidginBuddyList *gtkblist = NULL;

/* TODO: became serious, boy! */
static GdkPixbuf *hello_kitty = NULL;

/** Icons **/
static int row_changed_handler_id = -1;

static void unload_icons() {
	if(hello_kitty) g_object_unref(hello_kitty);
	hello_kitty = NULL;
}

static gboolean load_icons() {
	const gchar *filename = "hello_kitty.png";
	gchar *file = NULL;
	
	file = g_build_filename(purple_user_dir(), "pixmaps", "pidgin", "pidgin-protocol-status-icons", filename, NULL);
	if(!g_file_test(file, G_FILE_TEST_EXISTS)) {
		g_free(file);
		file = g_build_filename(DATADIR, "pixmaps", "pidgin", "pidgin-protocol-status-icons", filename, NULL);
	}
	if(!g_file_test(file, G_FILE_TEST_EXISTS)) {
		g_free(file);
		return FALSE;
	}
	
	hello_kitty = gdk_pixbuf_new_from_file(file, NULL);
	
	g_free(file);
	
	return TRUE;
}

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

/** Plugin-Gerödel **/
static gboolean core_quitting = FALSE;

static void core_quitting_cb() {
	core_quitting = TRUE;
}

static gboolean plugin_load(PurplePlugin *_plugin) {
	plugin = _plugin;
	
	if(!load_icons()) {
		purple_debug_error(PLUGIN_STATIC_NAME, _("Could not load icons!\n"));
		return FALSE;
	}
	
	purple_signal_connect(purple_get_core(), "quitting", plugin, core_quitting_cb, NULL);
	
	purple_signal_connect(pidgin_blist_get_handle(), "gtkblist-created", plugin, PURPLE_CALLBACK(gtkblist_created_cb), NULL);
	if(pidgin_blist_get_default_gtk_blist()) gtkblist_created_cb(purple_get_blist());
	
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	if(g_signal_handler_is_connected(gtkblist->treemodel, row_changed_handler_id))
		g_signal_handler_disconnect(gtkblist->treemodel, row_changed_handler_id);
	
	unload_icons();
	
	if(!core_quitting) pidgin_blist_refresh(purple_get_blist());
	
	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,				/**< type           */
	PIDGIN_PLUGIN_TYPE,					/**< ui_requirement */
	0,									/**< flags          */
	NULL,								/**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,			/**< priority       */

	PLUGIN_ID,							/**< id             */
	NULL,			
										/**< name           */
	PLUGIN_VERSION,						/**< version        */
	NULL,								/**  summary        */
				
	NULL,								/**  description    */
	PLUGIN_AUTHOR,						/**< author         */
	PLUGIN_WEBSITE,						/**< homepage       */

	plugin_load,						/**< load           */
	plugin_unload,						/**< unload         */
	NULL,								/**< destroy        */

	NULL,								/**< ui_info        */
	NULL,								/**< extra_info     */
	NULL,								/**< prefs_info     */
	NULL,								/**< actions        */
	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin) {
	const char *str = "Protocol Status Icons";
	gchar *plugins_locale_dir;

#ifdef ENABLE_NLS
	plugins_locale_dir = g_build_filename(purple_user_dir(), "locale", NULL);

	bindtextdomain(GETTEXT_PACKAGE, plugins_locale_dir);
	if(str == _(str)) {
		bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	}
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");

	g_free(plugins_locale_dir);
#endif /* ENABLE_NLS */

	info.name        = _("Protocol Status Icons");
	info.summary     = _("Replaces the status icons in the buddy list.");
	info.description = _("Replaces the status icons in the buddy list.");
}

PURPLE_INIT_PLUGIN(PLUGIN_STATIC_NAME, init_plugin, info)
