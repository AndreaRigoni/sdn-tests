FROM scratch

ARG CODAC_NAME=codac_v5
ARG ENTRY_POINT=${CODAC_NAME}_entry.sh

ADD ${CODAC_NAME}.tar /

COPY ${ENTRY_POINT} /root/${CODAC_NAME}_entry.sh
ENTRYPOINT /root/${CODAC_NAME}_entry.sh
