#!/bin/bash
make clean && \
make && \
PROJECT=pidgin-protocol-status-icons && \
DIR=${PROJECT}-$(cat VERSION)-linux-x86 && \
mkdir -p ${DIR}/plugins && \
mkdir -p ${DIR}/pixmaps/pidgin/${PROJECT} && \
mkdir -p ${DIR}/locale/de/LC_MESSAGES && \
cp ChangeLog ${DIR}/ChangeLog && \
cp src/.libs/${PROJECT}.so ${DIR}/plugins && \
cp share/pixmaps/*.png ${DIR}/pixmaps/pidgin/${PROJECT} && \

cd share/pixmaps/default && \
for file in $(find . -iname '*.png'); do
	dir=../../../${DIR}/pixmaps/pidgin/${PROJECT}/default/$(dirname $file) && \
	mkdir -p $dir && \
	cp $file $dir
done && \
cd ../../../ && \
cp share/pixmaps/default/theme.xml ${DIR}/pixmaps/pidgin/${PROJECT}/default/ && \

cp po/de.gmo ${DIR}/locale/de/LC_MESSAGES/${PROJECT}.mo && \
cp install_linux_package.sh ${DIR}/install.sh && \

tar czvf ${DIR}.tar.gz ${DIR} && \
rm -rf ${DIR}
