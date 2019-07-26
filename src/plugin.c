/* Copyright (C) 2009-2019 Konrad Gräfe <konradgraefe@aol.com>
 *
 * This software may be modified and distributed under the terms
 * of the GPLv2 license. See the COPYING file for details.
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
#include <gtkutils.h>
#include <pidginstock.h>

#define debug(fmt, ...) \
	purple_debug_info(PLUGIN_STATIC_NAME, fmt, ##__VA_ARGS__)

typedef struct _pidgin_blist_node {
	GtkTreeRowReference *row;
	gboolean contact_expanded;
	gboolean recent_signonoff;
	/* ... */
} PidginBlistNode;

static PurplePlugin *plugin;
static PidginBuddyList *gtkblist = NULL;
static gboolean core_quitting = FALSE;
static GData *icons = NULL;

static void core_quitting_cb() {
	core_quitting = TRUE;
}

/* Altered from do_colorshift in gnome-panel */
static void do_alphashift(GdkPixbuf *pixbuf, int shift) {
	gint i, j;
	gint width, height, padding;
	guchar *pixels;
	int val;

	if (!gdk_pixbuf_get_has_alpha(pixbuf))
		return;

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	padding = gdk_pixbuf_get_rowstride(pixbuf) - width * 4;
	pixels = gdk_pixbuf_get_pixels(pixbuf);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			pixels++;
			pixels++;
			pixels++;
			val = *pixels - shift;
			*(pixels++) = CLAMP(val, 0, 255);
		}
		pixels += padding;
	}
}


static GdkPixbuf *get_icon(PurpleBuddy *buddy, PurpleBlistNode *node) {
	GdkPixbuf *icon, *overlay = NULL;
	gchar id[128];
	PurpleAccount *account;
	PurplePresence *presence;
	const gchar *prpl, *status;
	gboolean idle, offline;
	gint x, y, h, w;

	account = purple_buddy_get_account(buddy);
	presence = purple_buddy_get_presence(buddy);
	if(!account || !presence) {
		return NULL;
	}

	prpl = purple_account_get_protocol_id(account);
	if(!prpl) {
		return NULL;
	}
	status = pidgin_stock_id_from_status_primitive(
		purple_status_type_get_primitive(
			purple_status_get_type(
				purple_presence_get_active_status(
					presence
				)
			)
		)
	);
	idle = purple_presence_is_idle(presence);
	offline = purple_presence_is_status_primitive_active(
		presence, PURPLE_STATUS_OFFLINE
	);

	g_snprintf(id, sizeof(id), "%s-%s-%d", prpl, status, idle);
	icon = g_datalist_get_data(&icons, id);
	if(icon) {
		return icon;
	}

	icon = pidgin_create_prpl_icon(account, PIDGIN_PRPL_ICON_MEDIUM);
	if(!icon) {
		return NULL;
	}

	if(offline) {
		/* If the buddy is offline, just greyscale the protocol icon. */
		gdk_pixbuf_saturate_and_pixelate(icon, icon, 0.0, FALSE);
		do_alphashift(icon, 77);
	} else {
		/* Otherwise blend the status icon over the protocol icon. */
		overlay = gtk_widget_render_icon(
			GTK_WIDGET(gtkblist->treeview), status,
			gtk_icon_size_from_name(PIDGIN_ICON_SIZE_TANGO_MICROSCOPIC),
			"GtkTreeView"
		);

		w = gdk_pixbuf_get_width(overlay);
		h = gdk_pixbuf_get_height(overlay);
		x = gdk_pixbuf_get_width(icon) - w;
		y = gdk_pixbuf_get_height(icon) - h;

		gdk_pixbuf_composite(
			overlay, icon,
			x, y, w, h,
			x, y, 1.0, 1.0,
			GDK_INTERP_BILINEAR, 255
		);
		g_object_unref(overlay);

		if(idle) {
			do_alphashift(icon, 77);
		}
	}

	g_datalist_set_data_full(&icons, id, icon, g_object_unref);

	return icon;
}

static void gtkblist_row_changed_cb(
	GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data
) {
	static gboolean inuse = FALSE;

	PurpleBlistNode *node;
	PurpleBuddy *buddy;
	PidginBlistNode *gtkbuddynote;
	GdkPixbuf *icon;

	if(inuse) {
		return;
	}

	gtk_tree_model_get(model, iter, NODE_COLUMN, &node, -1);
	if(PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		buddy = (PurpleBuddy *)node;
	} else if(PURPLE_BLIST_NODE_IS_CONTACT(node)) {
		buddy = purple_contact_get_priority_buddy((PurpleContact *)node);
	} else {
		return;
	}
	if(!buddy) {
		return;
	}

	gtkbuddynote = ((PurpleBlistNode*)buddy)->ui_data;
	if(gtkbuddynote && gtkbuddynote->recent_signonoff) {
		return;
	}

	icon = get_icon(buddy, node);
	if(!icon) {
		return;
	}

	inuse = TRUE;
	gtk_tree_store_set(
		GTK_TREE_STORE(model), iter,
		STATUS_ICON_COLUMN, icon, STATUS_ICON_VISIBLE_COLUMN,
		TRUE, -1
	);
	inuse = FALSE;
}
static void gtkblist_created_cb(PurpleBuddyList *blist) {
	if(gtkblist) {
		g_signal_handlers_disconnect_by_func(gtkblist->treemodel,
			G_CALLBACK(gtkblist_row_changed_cb), NULL
		);
	}

	gtkblist = PIDGIN_BLIST(blist);
	g_signal_connect(gtkblist->treemodel,
		"row_changed", G_CALLBACK(gtkblist_row_changed_cb), NULL
	);

	pidgin_blist_refresh(blist);
}

static gboolean plugin_load(PurplePlugin *_plugin) {
	plugin = _plugin;

	g_datalist_init(&icons);
	
	purple_signal_connect(purple_get_core(),
		"quitting", plugin, core_quitting_cb, NULL
	);

	purple_signal_connect(pidgin_blist_get_handle(),
		"gtkblist-created", plugin,
		PURPLE_CALLBACK(gtkblist_created_cb), NULL
	);
	if(pidgin_blist_get_default_gtk_blist()) {
		gtkblist_created_cb(purple_get_blist());
	}
	
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	purple_signals_disconnect_by_handle(plugin);

	if(gtkblist) {
		g_signal_handlers_disconnect_by_func(gtkblist->treemodel,
			G_CALLBACK(gtkblist_row_changed_cb), NULL
		);
		gtkblist = NULL;
	}

	if(!core_quitting) {
		pidgin_blist_refresh(purple_get_blist());
	}

	g_datalist_clear(&icons);
	
	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,     /* type           */
	PIDGIN_PLUGIN_TYPE,         /* ui_requirement */
	0,                          /* flags          */
	NULL,                       /* dependencies   */
	PURPLE_PRIORITY_DEFAULT,    /* priority       */

	PLUGIN_ID,                  /* id             */
	NULL,                       /* name           */
	PLUGIN_VERSION,             /* version        */
	NULL,                       /* summary        */

	NULL,                       /* description    */
	PLUGIN_AUTHOR,              /* author         */
	PLUGIN_WEBSITE,             /* homepage       */

	plugin_load,                /* load           */
	plugin_unload,              /* unload         */
	NULL,                       /* destroy        */

	NULL,                       /* ui_info        */
	NULL,                       /* extra_info     */
	NULL,                       /* prefs_info     */
	NULL,                       /* actions        */
	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void init_plugin(PurplePlugin *plugin) {
#ifdef ENABLE_NLS
	const char *str = "Protocol Status Icons";
	gchar *plugins_locale_dir;

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
