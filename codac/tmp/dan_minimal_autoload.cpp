
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


const char XML_content[] =
"<danApi >"
"    <version >1.0.0</version>"
"    <ICprogram name=\"DummySrc\" >"
"        <source name=\"DummySrc\" >"
"            <itemDanType >danFloat</itemDanType>"
"            <maxTransferRate >100000</maxTransferRate>"
"            <advancedSource >"
"                <streamType >DATA_D0</streamType>"
"                <dataModel >DM_BLOCK2D_VAR</dataModel>"
"                <checkPolicy >CHECK_AND_OVERWRITE</checkPolicy>"
"                <queueSize >1000</queueSize>"
"                <profMonitoring >1</profMonitoring>"
"                <timeFormat >UTC</timeFormat>"
"            </advancedSource>"
"            <deviceInfo >"
"                <deviceName >DSOURCE1</deviceName>"
"                <deviceVersion >v1.3</deviceVersion>"
"                <operationalMode >0</operationalMode>"
"                <manufactureCode >N1134342</manufactureCode>"
"            </deviceInfo>"
"            <channelsInfo >"
"                <channel number=\"0\" variable=\"EC-DAN-ACQ:DUMMY-SIGNAL\""
"                   label=\"EC-DAN-ACQ:DUMMY-SIGNAL\" status=\"ENABLE\" >"
"                    <calibration sourceUnit=\"V\" calibratedUnit=\"\""
"                   calibrationType=\"raw\" ></calibration>"
"                </channel>"
"            </channelsInfo>"
"        </source>"
"        <danStreamer name=\"Test_STREAMER\" >"
"            <link >ALL</link>"
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

    char * filename = getenv("CU_PCF_DANCONF");
    if(filename) {
        int err = dan_admin_api_load_fromFile(filename);
        if(err) {
            std::cerr << "Error reading file \n";
            return 1;
        }
    }
    else {
        int err = dan_admin_api_load_fromXML(XML_content);
        if(err) {
            std::cerr << "Error reading XML \n";
            return 1;
        }
    }
    return 0;
}
