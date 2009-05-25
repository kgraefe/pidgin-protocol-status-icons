#!/bin/bash
make -f Makefile.mingw clean && \
make -f Makefile.mingw && \
PROJECT=pidgin-protocol-status-icons && \
WIN32DIR=${PROJECT}-$(cat VERSION)-win32 && \
mkdir -p ${WIN32DIR}/pidgin/plugins && \
mkdir -p ${WIN32DIR}/pidgin/pixmaps/pidgin/${PROJECT} && \
mkdir -p ${WIN32DIR}/pidgin/locale/de/LC_MESSAGES && \
sed 's/$/\r/' ChangeLog >${WIN32DIR}/ChangeLog.txt && \
cp src/${PROJECT}.dll ${WIN32DIR}/pidgin/plugins && \
cp share/pixmaps/*.png ${WIN32DIR}/pidgin/pixmaps/pidgin/${PROJECT} && \

cd share/pixmaps/default && \
for file in $(find . -iname '*.png'); do
	dir=../../../${WIN32DIR}/pidgin/pixmaps/pidgin/${PROJECT}/default/$(dirname $file) && \
	mkdir -p $dir && \
	cp $file $dir
done && \
cd ../../../ && \
cp share/pixmaps/default/theme.xml ${WIN32DIR}/pidgin/pixmaps/pidgin/${PROJECT}/default/ && \

i586-mingw32msvc-strip --strip-unneeded ${WIN32DIR}/pidgin/plugins/${PROJECT}.dll && \
cp po/de.gmo ${WIN32DIR}/pidgin/locale/de/LC_MESSAGES/${PROJECT}.mo && \
rm -f ${WIN32DIR}.zip && \
cd ${WIN32DIR} && \
zip -r ../${WIN32DIR}.zip * && \
cd .. && \
rm -rf ${WIN32DIR}
