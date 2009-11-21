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

#include <glib.h>

#include <plugin.h>
#include <gtkplugin.h>
#include <gtkblist.h>
#include <debug.h>

#include "icons.h"
#include "gtkstatus-icon-theme.h"
#include "theme-manager.h"

GData *icons = NULL;
PidginIconTheme *theme = NULL;

static void free_icon(GQuark key_id, gpointer data, gpointer null) {
	g_object_unref(data);
}

void unload_icons(void) {
	g_datalist_foreach(&icons, free_icon, NULL);
	icons = NULL;
}

static void clear_icons(const char *name, PurplePrefType type, gconstpointer val, gpointer data) {
	g_datalist_foreach(&icons, free_icon, NULL);
	theme = PIDGIN_ICON_THEME(purple_theme_manager_find_theme(purple_prefs_get_string(PIDGIN_PREFS_ROOT "/status/icon-theme"), "status-icon"));
	g_datalist_init(&icons);
}

gboolean init_icons(PurplePlugin *plugin) {
	theme = PIDGIN_ICON_THEME(purple_theme_manager_find_theme(purple_prefs_get_string(PIDGIN_PREFS_ROOT "/status/icon-theme"), "status-icon"));
	
	g_datalist_init(&icons);
	
	purple_prefs_connect_callback(plugin, PIDGIN_PREFS_ROOT "/status/icon-theme", clear_icons, NULL);
	
	return TRUE;
}

GdkPixbuf *get_icon(const gchar *prpl, gint status) {
	GdkPixbuf *icon = NULL;
	
	gchar *id = NULL;
	const gchar *file = NULL;
	const gchar *dir = NULL;
	gchar *filename = NULL;
	
	purple_debug_info(PLUGIN_STATIC_NAME, "status: %i\n", status);
	
	switch(status) {
		case PURPLE_STATUS_UNAVAILABLE:
			id = g_strdup_printf("%s-unavailable", prpl);
			break;
		case PURPLE_STATUS_AWAY:
			id = g_strdup_printf("%s-away", prpl);
			break;
		case PURPLE_STATUS_EXTENDED_AWAY:
			id = g_strdup_printf("%s-extended-away", prpl);
			break;
		case PURPLE_STATUS_OFFLINE:
			id = g_strdup_printf("%s-offline", prpl);
			break;
		case PURPLE_STATUS_INVISIBLE:
			id = g_strdup_printf("%s-invisible", prpl);
			break;
		case PURPLE_STATUS_AVAILABLE:
			id = g_strdup_printf("%s-available", prpl);
			break;
		default:
			return NULL;
	}
	
	icon = (GdkPixbuf *)g_datalist_get_data(&icons, id);
	if(!icon) {
		/*** Icon laden ***/
		file = pidgin_icon_theme_get_icon(PIDGIN_ICON_THEME(theme), id);
		if(!file) {
			g_free(id);
			return NULL;
		}
		
		dir = purple_prefs_get_path(PIDGIN_PREFS_ROOT "/status/icon-theme-dir");
		if(!dir) {
			g_free(id);
			return NULL;
		}
		
		filename = g_build_filename(dir, "16", file, NULL);
		if(!filename || !g_file_test(filename, G_FILE_TEST_EXISTS)) {
			g_free(id);
			return NULL;
		}
		
		icon = gdk_pixbuf_new_from_file(filename, NULL);
		
		g_datalist_set_data(&icons, id, (gpointer)icon);
	}
	
	return icon;
}
