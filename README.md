# Pidgin Protocol Status Icons

[![License](https://img.shields.io/badge/License-GPLv2-blue.svg?style=flat)](COPYING)
[![Download](https://img.shields.io/badge/Download-Latest_Release-brightgreen.svg?style=flat)][1]

This is a Pidgin plugin that adds the protocol icon to the status icons in the
buddy list.

## Table of contents
- [Installation](#installation)
    - [Installation on Windows](#installation-on-windows)
    - [Installation on Linux](#installation-on-linux)
- [Building from source](#building-from-source)
    - [Building on Windows](#building-on-windows)
    - [Building on Linux](#building-on-linux)
- [Contribution](#contribution)

## Installation
### Installation on Windows
Download the ZIP file from the [latest release][1] and extract the contents of
`pidgin` either to the installation directory of Pidgin (typically `C:\Program
Files\Pidgin`) or to your .purple user directory (typically
`%APPDATA%\Roaming\.purple`).

Extract the default theme into `themes` in your .purple user directory (create
`themes` if not present). After that you can enable the plugin and select the
theme in `Tools -> Preferences -> Themes -> Status Icon Theme`.

### Installation on Linux
If your distribution has the plugin in its repository you can use that.
Otherwise you must build the plugin [from source](#building-on-linux).

Extract the default theme into `themes` in your .purple user directory (create
`themes` if not present). After that you can enable the plugin and select the
theme in `Tools -> Preferences -> Themes -> Status Icon Theme`.

## Building from source
### Building on Windows
In order to build the plugin for Windows an already-compiled source tree of
Pidgin is required. Please see the [Pidgin for Windows Build Instructions][2]
for details. The [pidgin-windev][3] script does all that.

After that you need to create a file named `local.mak` that points to the
Pidgin source tree, e.g.:

    PIDGIN_TREE_TOP=$(PLUGIN_TOP)/../../pidgin-2.12.0

Now you can build the plugin:

    make -f Makefile.mingw

### Building on Linux
To install the plugin on Linux you need to extract a release tarball and
compile it from source:

    sudo apt install pidgin-dev
    ./configure
    make
    sudo make install

By default the plugin will be installed to `/usr/local`.  If you installed
Pidgin through your package manager, it is very likely installed into `/usr`
(i.e. `which pidgin` returns `/usr/bin/pidgin`). Use `./configure
--prefix=/usr` in this case.

When you use the repository directly or one of those auto-generated "Source
code" archives, you need to run `./autogen.sh` before running `./configure`.

## Contribution
We love patches. :heart: Please fork the project, do your changes and make a
pull request.

[1]: https://github.com/kgraefe/pidgin-protocol-status-icons/releases/latest
[2]: https://developer.pidgin.im/wiki/BuildingWinPidgin
[3]: https://github.com/kgraefe/pidgin-windev
