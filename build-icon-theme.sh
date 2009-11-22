#!/bin/bash

THEME=protocol-status-theme && \
DIR=themes-tmp

mkdir -p ${DIR}/protocol-status-theme/purple/status-icon/16/ && \

cp default-theme/protocol-status-theme/purple/status-icon/theme.xml ${DIR}/protocol-status-theme/purple/status-icon/ && \
cp default-theme/protocol-status-theme/purple/status-icon/16/*.png ${DIR}/protocol-status-theme/purple/status-icon/16/ && \

rm -f ${THEME}.zip && \
cd ${DIR} && \
zip -r ../${THEME}.zip * && \
cd .. && \
rm -rf ${DIR}
