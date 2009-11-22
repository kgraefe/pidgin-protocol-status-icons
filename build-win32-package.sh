#!/bin/bash
make -f Makefile.mingw clean && \
make -f Makefile.mingw && \

PROJECT=pidgin-protocol-status-icons && \
WIN32DIR=${PROJECT}-$(cat VERSION)-win32 && \

mkdir -p ${WIN32DIR}/pidgin/plugins && \
mkdir -p ${WIN32DIR}/pidgin/locale/de/LC_MESSAGES && \
mkdir -p ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/16 && \

sed 's/$/\r/' ChangeLog >${WIN32DIR}/ChangeLog.txt && \
sed 's/$/\r/' README.win32 >${WIN32DIR}/ReadMe.txt && \
cp src/${PROJECT}.dll ${WIN32DIR}/pidgin/plugins && \
cp default-theme/protocol-status-theme/purple/status-icon/theme.xml ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/ && \
cp default-theme/protocol-status-theme/purple/status-icon/16/*.png ${WIN32DIR}/pidgin/themes/protocol-status-theme/purple/status-icon/16/ && \
i586-mingw32msvc-strip --strip-unneeded ${WIN32DIR}/pidgin/plugins/${PROJECT}.dll && \
cp po/de.gmo ${WIN32DIR}/pidgin/locale/de/LC_MESSAGES/${PROJECT}.mo && \

rm -f ${WIN32DIR}.zip && \
cd ${WIN32DIR} && \
zip -r ../${WIN32DIR}.zip * && \
cd .. && \
rm -rf ${WIN32DIR}
