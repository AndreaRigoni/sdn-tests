############################################################################
## Autosave monitor post setup
############################################################################

cd "${TOP}/iocBoot/$(IOC)"
create_monitor_set("iocEC-DAN-PCF0SYSM.req",30,"P=$(AUTOSAVE_SYSM_PV_PREFIX)")

#- End-of-file marker - do not delete or add lines below!
