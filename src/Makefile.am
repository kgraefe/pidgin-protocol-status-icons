EXTRA_DIST = \
	internal.h \
	icons.h \
	gtkblist-modifier.h \
	Makefile.mingw

plugindir = $(libdir)/pidgin
DEFS = -DLOCALEDIR=\"$(localedir)\" -DDATADIR=\"$(datadir)\"

plugin_LTLIBRARIES = pidgin-protocol-status-icons.la
pidgin_protocol_status_icons_la_SOURCES = \
	icons.c \
	gtkblist-modifier.c \
	pidgin-protocol-status-icons.c
	
pidgin_protocol_status_icons_la_LDFLAGS = -module -avoid-version

