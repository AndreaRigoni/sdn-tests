#ifndef SDD_DATA_H
#define SDD_DATA_H

#include "sdd-pon-if.h"
#include "sdd-iomodule.h"

static SDD_PONVar pvlist []={

				{"EC-DAN-ACQ:GEN_ENABLE", true, DBR_ENUM,"0"},
				{"EC-DAN-ACQ:GEN_FREQ", true, DBR_LONG,"0"},
				{"EC-DAN-ACQ:GEN_PKTSIZE", true, DBR_LONG,"0"},
				{"EC-DAN-F000:ST1", true, DBR_DOUBLE,"0"},
		};

static SDD_IOModule iomlist []={
	};

#endif /* SDD_DATA_H */