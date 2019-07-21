EXTRA_DIST = Makefile.mingw

plugindir = $(libdir)/pidgin
DEFS = -DLOCALEDIR=\"$(localedir)\"

plugin_LTLIBRARIES = pidgin-protocol-status-icons.la
pidgin_protocol_status_icons_la_SOURCES = plugin.c
	
pidgin_protocol_status_icons_la_LDFLAGS = -module -avoid-version

