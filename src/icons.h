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

#ifndef PIDGIN_PROTOCOL_STATUS_ICONS_H
#define PIDGIN_PROTOCOL_STATUS_ICONS_H

#include "internal.h"

#include <gtkplugin.h>

/* TODO: became serious, boy! */
GdkPixbuf *hello_kitty;

void unload_icons(void);
gboolean load_icons(void);

#endif /* PIDGIN_PROTOCOL_STATUS_ICONS_H */
