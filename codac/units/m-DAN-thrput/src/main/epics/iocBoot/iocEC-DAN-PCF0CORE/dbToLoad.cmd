
#======================================================================
# Loading DBs
#======================================================================
cd $(TOP)/db
dbLoadRecords("PCF0-EC-DAN-ACQ.db")
dbLoadRecords("PCF0-EC-DAN-F000.db")




#======================================================================
# IOC Monitor
#======================================================================
cd $(EPICS_ROOT)/db
dbLoadRecords("iocmon.db","CBS=EC-DAN-, CTRLTYPE=F, IDX=0, IOCTYPE=CORE")


#- End-of-file marker - do not delete or add lines below!
