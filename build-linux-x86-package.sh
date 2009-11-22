#!/bin/bash
make clean && \
make && \

PROJECT=pidgin-protocol-status-icons && \
DIR=${PROJECT}-$(cat VERSION)-linux-x86 && \

mkdir -p ${DIR}/plugins && \
mkdir -p ${DIR}/locale/de/LC_MESSAGES && \
mkdir -p ${DIR}/themes/protocol-status-theme/purple/status-icon/16 && \

cp ChangeLog ${DIR}/ChangeLog && \
cp README.linux_binary ${DIR}/ReadMe && \
cp src/.libs/${PROJECT}.so ${DIR}/plugins && \
cp default-theme/protocol-status-theme/purple/status-icon/theme.xml ${DIR}/themes/protocol-status-theme/purple/status-icon/ && \
cp default-theme/protocol-status-theme/purple/status-icon/16/*.png ${DIR}/themes/protocol-status-theme/purple/status-icon/16/ && \
cp po/de.gmo ${DIR}/locale/de/LC_MESSAGES/${PROJECT}.mo && \
cp install_linux_package.sh ${DIR}/install.sh && \

tar czvf ${DIR}.tar.gz ${DIR} && \
rm -rf ${DIR}
