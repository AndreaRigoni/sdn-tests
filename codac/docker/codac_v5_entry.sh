#!/bin/bash

set -x

SERDIR=/etc/rc5.d
SERVICES=" \
		   S01sysstat \
		   S02lvm2-monitor \
		   S11portreserve \
		   S12rsyslog \
		   S13irqbalance \
		   S13rpcbind \
		   S14nfslock \
		   S15mdmonitor \
		   S19rpcgssd \
		   S22messagebus \
		   S25blk-availability \
		   S26acpid \
		   S50mcelogd \
		   S56xinetd \
		   S58ntpd \
		   S60epicsCaRepeater \
		   S60epicsIocLogServer \
		   S60tomcat \
		   S60tomcat-css \
		   S60tomcat-sdd \
		   S64postgresql-9.3 \
		   S82abrtd \
		   S82abrt-ccpp \
		   S90css-services \
		   S95atd \
		   S97rhnsd \
		   S97rhsmcertd \
		   S99firstboot \
		   S99local \
		   "

for s in $SERVICES; do
 $SERDIR/$s start
done

sh ${@:1}
