/* Copyright (C) 2009-2019 Konrad Gräfe <konradgraefe@aol.com>
 *
 * This software may be modified and distributed under the terms
 * of the GPLv2 license. See the COPYING file for details.
 */

#ifndef PIDGIN_PROTOCOL_STATUS_ICONS_H
#define PIDGIN_PROTOCOL_STATUS_ICONS_H

#include "internal.h"

#include <gtkplugin.h>

void unload_icons(void);
gboolean init_icons(PurplePlugin *plugin);

GdkPixbuf *get_icon(const gchar *prpl, gint status);

#endif /* PIDGIN_PROTOCOL_STATUS_ICONS_H */
