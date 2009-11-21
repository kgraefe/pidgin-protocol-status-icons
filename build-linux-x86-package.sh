#!/bin/bash
make clean && \
make && \
PROJECT=pidgin-protocol-status-icons && \
DIR=${PROJECT}-$(cat VERSION)-linux-x86 && \
mkdir -p ${DIR}/plugins && \
mkdir -p ${DIR}/locale/de/LC_MESSAGES && \
cp ChangeLog ${DIR}/ChangeLog && \
cp src/.libs/${PROJECT}.so ${DIR}/plugins && \

cp po/de.gmo ${DIR}/locale/de/LC_MESSAGES/${PROJECT}.mo && \
cp install_linux_package.sh ${DIR}/install.sh && \

tar czvf ${DIR}.tar.gz ${DIR} && \
rm -rf ${DIR}
