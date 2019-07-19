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

#include "icons.h"
#include "gtkblist-modifier.h"


static PurplePlugin *plugin;

static gboolean core_quitting = FALSE;

static void core_quitting_cb() {
	core_quitting = TRUE;
}

static gboolean plugin_load(PurplePlugin *_plugin) {
	plugin = _plugin;
	
	init_icons(plugin);
	
	purple_signal_connect(purple_get_core(), "quitting", plugin, core_quitting_cb, NULL);
	
	gtkblist_modifier_init(plugin);
	
	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	gtkblist_modifier_uninit();
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
	NULL,								/**< name           */
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
