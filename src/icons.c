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

#include "icons.h"


#include <plugin.h>
#include <gtkplugin.h>

void unload_icons(void) {
	if(hello_kitty) g_object_unref(hello_kitty);
	hello_kitty = NULL;
}

gboolean load_icons(void) {
	const gchar *filename = "hello_kitty.png";
	gchar *file = NULL;
	hello_kitty = NULL;
	
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
