
#======================================================================
# SYS Monitor
#======================================================================
cd $(EPICS_ROOT)/db
dbLoadRecords("picmg-sensors.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0")
dbLoadRecords("sysmon-tcnd.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0")
dbLoadRecords("picmg-cfa633.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0")
dbLoadRecords("sysmon.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0")


#======================================================================
# IOC Monitor
#======================================================================
cd $(EPICS_ROOT)/db
dbLoadRecords("iocmon.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0, IOCTYPE=SYSM")


#- End-of-file marker - do not delete or add lines below!
