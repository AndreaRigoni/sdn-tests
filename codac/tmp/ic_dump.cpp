
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <dan/dan_Admin.h>
#include <dan/dan_Source.h>
#include <dan/dan_DataCore.h>


extern "C" {

#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"

}

static int32_t isRunning(const char *progName, char *streamerName)
{
    char comm[2000];
    char finalName[DAN_MAX_STR];
    char instances[1000];
    int32_t ret=0;


    if((progName==NULL) || (!strcmp(progName,""))) {
        strcpy(finalName,"NULL");
    }
    else {
        strcpy(finalName,progName);
    }

    sprintf(comm, "/usr/bin/pgrep -f \".*danStreamer_.*%s %s\"",finalName,streamerName);

    FILE* app = popen(comm, "r");
    if (app)
    {
        if(fgets(instances, sizeof(instances), app) !=NULL ) {
            ret=1;
        }
        pclose(app);
    }
    return ret;
}


void print_streamers(p_dan_DataCore dc) {
    printf("\n\n\n streamers: \n");
    for (int i=0; i<dc->datacoreInfo->nSubscribers;i++) {
        int subscribed = dc->datacoreInfo->subscribers_info[i].subscribed;
        int running = isRunning(dc->datacoreInfo->progName, dc->datacoreInfo->subscribers_info[i].name);
        std::cout << dc->datacoreInfo->subscribers_info[i].name
                  << "\n";
    }
}




////////////////////////////////////////////////////////////////////////////////
//  INIT NEW API  //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// MANUALLY //

int32_t dan_admin_createLibraryCore_noXML()
{
    st_dan_DataCore ddc;
    int32_t fd;
    int32_t res=0;
    char danCoreName[1000];
    char programName[1000];

    ddc.datacoreInfo=NULL;

    umask(0);

    dan_int_buildCoreShmName(programName, danCoreName);
    fd=shm_open(danCoreName,O_CREAT|O_RDWR,S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        LOGMSG(LL_ERR,"Error creating shared memory zone for DAN API\n");
        res=-1;
        goto END;
    }
    if(ftruncate(fd,DAN_TOTALSIZE+64)<0) {
        LOGMSG(LL_ERR,"Error resizing DataCore memory zone to %ld bytes\n",DAN_TOTALSIZE+64);
        res=-1;
        goto END;
    }
    ddc.datacoreInfo = (st_dan_DataCore_Info *)mmap(0, DAN_TOTALSIZE, (PROT_READ | PROT_WRITE), MAP_SHARED || MAP_LOCKED, fd, 0);

    if (ddc.datacoreInfo == NULL) {
        LOGMSG(LL_ERR,"Error on DataCore mmap \n");
        res=-1;
        goto END;
    }
    ddc.datacoreInfo->nSources=0;
    ddc.datacoreInfo->nSubscribers=0;
    ddc.datacoreInfo->xmlSize=0;
    strcpy(ddc.datacoreInfo->configurationFile,confFileName);


    if (dan_conf_loadXMLConfiguration(&(ddc.datacoreInfo->shm_danConf),xmlContent) < 0) {
        LOGMSG(LL_ERR,"Error loading XML configuration\n");
        res=-1;
        goto END;
    }

    if (confFileName != NULL) {
        strcpy(ddc.datacoreInfo->configurationFile,confFileName);
    }

    if (dan_admin_initFromConfiguration((dan_DataCore)&ddc,&(ddc.datacoreInfo->shm_danConf)) < 0) {
        LOGMSG(LL_ERR,"Error initializing DAN API with XML configuration\n");
        dan_conf_freeXMLConfiguration(&(ddc.datacoreInfo->shm_danConf));
        res=-1;
        goto END;
    }

END:

    if (ddc.datacoreInfo != NULL) {
        munmap(ddc.datacoreInfo,DAN_TOTALSIZE);
        ddc.datacoreInfo=NULL;
    }
    if (fd > -1) {
        close(fd);
        if (res < 0) {
            shm_unlink(danCoreName);
        }
    }

    return res;
}




////////////////////////////////////////////////////////////////////////////////
//  MAIN  //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
    int32_t num = 0;
    char ** icp = dan_admin_api_getAllLoadedICPrograms(&num);

    if(num && icp) {
        for(int i=0; i<num; ++i) {
            std::cout << icp[i];
            // if(dan_publisher_isOpenStream())

            dan_DataCore ddc = dan_initLibrary_icprog(icp[i]);
            dan_datacore_printStatus(ddc);
            p_dan_DataCore dc = (p_dan_DataCore)ddc;
            print_streamers(dc);
            dan_closeLibrary(ddc);

            std::cout << std::endl;
            free(icp[i]);
        }
        free(icp);
    } else return 1;






    return 0;
}
