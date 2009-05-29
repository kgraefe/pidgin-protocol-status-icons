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

#include "icons.h"
#include "gtkblist-modifier.h"


static PurplePlugin *plugin;

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
	
	gtkblist_modifier_connect(plugin);
	
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	gtkblist_modifier_disconnect();
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
