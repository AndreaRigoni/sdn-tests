#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <errno.h>
#include <string.h>


#include <dan/dan_Admin.h>
#include <dan/dan_Source.h>
#include <dan/dan_DataCore.h>
#include <dan/dan_Conf.h>

extern "C" {
#include "dan_DataCore_st.h"
#include "dan_Source_st.h"
#include "dan_Subscriber_st.h"
#include "dan_Admin_st.h"
}



#define print_size(str) << "-> " << sizeof(str) << "\n"

//    char progName[DAN_MAX_STR];
//    char configurationFile[DAN_MAX_STR];
//    char xmlContent[DAN_MAX_XMLCONTENT];
//	int64_t xmlSize;
//	int32_t nSources;
//	int32_t nSubscribers;
//    st_dan_Source_Info sources_info[DAN_MAX_SOURCES];
//    st_dan_Subscriber_Info subscribers_info[DAN_MAX_STREAMERS];
//    dan_Configuration shm_danConf;


int32_t local_dan_admin_createLibraryCore(const char *xmlContent,  const char *confFileName)
{
    st_dan_DataCore ddc;
    int32_t fd;
    int32_t res=0;
    char danCoreName[1000];
    char programName[1000];

    ddc.datacoreInfo=NULL;

    umask(0);

    res=dan_conf_getProgramName(xmlContent,programName);
    if(res<0) {
        LOGMSG(LL_ERR,"No 'ICprogram' tag in XML configuration\n");
        return -1;
    }
    dan_int_buildCoreShmName(programName, danCoreName);

    fd=shm_open(danCoreName,O_CREAT|O_RDWR,S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        LOGMSG(LL_ERR,"Error creating shared memory zone for DAN API\n");
        res=-1;
        goto END;
    }

    std::cout
            print_size(char[DAN_MAX_STR])
            print_size(char[DAN_MAX_STR])
            print_size(char[DAN_MAX_XMLCONTENT])
            print_size(st_dan_Source_Info[DAN_MAX_SOURCES])
            print_size(st_dan_Subscriber_Info[DAN_MAX_STREAMERS])
            print_size(dan_Configuration)
            ;

    std::cout << "allocating size: " << DAN_TOTALSIZE+64 << "\n";

    if(ftruncate(fd,DAN_TOTALSIZE+64)<0) {
        LOGMSG(LL_ERR,"Error resizing DataCore memory zone to %ld bytes\n",DAN_TOTALSIZE+64);
        res=-1;
        goto END;
    }
//    ddc.datacoreInfo = (st_dan_DataCore_Info *)mmap(0, DAN_TOTALSIZE, (PROT_READ | PROT_WRITE), MAP_SHARED || MAP_LOCKED, fd, 0);

//    if (ddc.datacoreInfo == NULL) {
//        LOGMSG(LL_ERR,"Error on DataCore mmap \n");
//        res=-1;
//        goto END;
//    }
//    ddc.datacoreInfo->nSources=0;
//    ddc.datacoreInfo->nSubscribers=0;
//    ddc.datacoreInfo->xmlSize=0;
//    strcpy(ddc.datacoreInfo->configurationFile,confFileName);


//    if (dan_conf_loadXMLConfiguration(&(ddc.datacoreInfo->shm_danConf),xmlContent) < 0) {
//        LOGMSG(LL_ERR,"Error loading XML configuration\n");
//        res=-1;
//        goto END;
//    }

//    if (confFileName != NULL) {
//        strcpy(ddc.datacoreInfo->configurationFile,confFileName);
//    }

//    if (dan_admin_initFromConfiguration((dan_DataCore)&ddc,&(ddc.datacoreInfo->shm_danConf)) < 0) {
//        LOGMSG(LL_ERR,"Error initializing DAN API with XML configuration\n");
//        dan_conf_freeXMLConfiguration(&(ddc.datacoreInfo->shm_danConf));
//        res=-1;
//        goto END;
//    }

END:

    if (ddc.datacoreInfo != NULL) {
        munmap(ddc.datacoreInfo,DAN_TOTALSIZE);
        ddc.datacoreInfo=NULL;
    }
    if (fd > -1) { close(fd);
        if (res < 0) {
            shm_unlink(danCoreName);
        }
    }

    return res;
}






int _main(int argc, char *argv[])
{
    int32_t fd=-1;
    int32_t size = 1024;
    int res = 0;

    fd=shm_open("test", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        int errsv = errno;
        std::cerr << "error opening shm error: " << strerror(errsv) << " \n";
        res=1;
        goto ERR;
    }

    res = ftruncate(fd,64);
    if (res < 0) {
        int errsv = errno;
        std::cerr << "ftruncate error: " << strerror(errsv) << " \n";
    }
    shm_unlink("test");
ERR:
    return res;
}



const char XML_content[] =
"<danApi >"
"    <version >1.0.0</version>"
"    <ICprogram name=\"DummyShm\" >"
"        <source name=\"DummyShm\" >"
"            <itemDanType >danFloat</itemDanType>"
"            <maxTransferRate >10</maxTransferRate>"
//"            <advancedSource >"
//"                <streamType >DATA_D0</streamType>"
//"                <dataModel >DM_BLOCK2D_VAR</dataModel>"
//"                <checkPolicy >CHECK_AND_OVERWRITE</checkPolicy>"
//"                <queueSize >10</queueSize>"
//"                <profMonitoring >1</profMonitoring>"
//"                <timeFormat >UTC</timeFormat>"
//"            </advancedSource>"
"            <deviceInfo >"
"                <deviceName >DSOURCE1</deviceName>"
"                <deviceVersion >v1.3</deviceVersion>"
"                <operationalMode >0</operationalMode>"
"                <manufactureCode >N1134342</manufactureCode>"
"            </deviceInfo>"
"            <channelsInfo >"
"                <channel number=\"0\" variable=\"XX:DUMMY-SIGNAL\""
"                   label=\"XX:DUMMY-SIGNAL\" status=\"ENABLE\" >"
"                    <calibration sourceUnit=\"V\" calibratedUnit=\"\""
"                   calibrationType=\"raw\" ></calibration>"
"                </channel>"
"            </channelsInfo>"
"        </source>"
"        <danStreamer name=\"Test_STREAMER\" >"
"            <link >DummyShm</link>"
"            <internal >"
"                <danInterface ></danInterface>"
"                <servers >"
"                    <policy >IF_FAIL_NEXT</policy>"
"                    <server >${DAN_ARCHIVE_MASTER}</server>"
"                    <server >${DAN_ARCHIVE_SLAVE}</server>"
"                </servers>"
"            </internal>"
"        </danStreamer>"
"    </ICprogram>"
"</danApi>"
;

int main(int argc, char *argv[])
{
    int res = local_dan_admin_createLibraryCore(XML_content, "dummy_conf_file.xml");
    return (res>=0);
}



