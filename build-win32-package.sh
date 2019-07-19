#!/bin/bash
set -x

PROJECT=pidgin-protocol-status-icons
WIN32DIR=${PROJECT}-$(cat VERSION)-win32

test -f README.win32 || exit

make -f Makefile.mingw clean || exit
make -f Makefile.mingw || exit
mkdir -p ${WIN32DIR}/pidgin/plugins || exit
sed 's/$/\r/' CHANGES.md >${WIN32DIR}/CHANGES.md || exit
sed 's/$/\r/' README.win32 >${WIN32DIR}/ReadMe.txt || exit
cp src/${PROJECT}.dll ${WIN32DIR}/pidgin/plugins || exit
#i586-mingw32msvc-strip --strip-unneeded ${WIN32DIR}/pidgin/plugins/${PROJECT}.dll || exit
for f in po/*.po; do
	if [ -f $f ]; then
		lang=$(basename $f .po)
		mkdir -p ${WIN32DIR}/pidgin/locale/${lang}/LC_MESSAGES || exit
		cp po/${lang}.gmo ${WIN32DIR}/pidgin/locale/${lang}/LC_MESSAGES/${PROJECT}.mo || exit
	fi
done
mkdir -p ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/16 && \
cp default-theme/protocol-status-theme/purple/status-icon/theme.xml ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/ && \
cp default-theme/protocol-status-theme/purple/status-icon/16/*.png ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/16/ && \
rm -f ${WIN32DIR}.zip || exit
cd ${WIN32DIR} || exit
zip -r ../${WIN32DIR}.zip * || exit
cd .. || exit
rm -rf ${WIN32DIR}
