#ifndef SDD_IOMODULE_H
#define SDD_IOMODULE_H

//include all linux libraries for IO module listed in the programs


//max is 15 for component name
#define SDD_CPMAXLENGTH 50
#define SDD_SERIALNBMAXLENGTH 60
#define SDD_FDPREFIXMAXLENGTH 100

struct SDD_IOModule {
	
	//name of the module
 	char name[SDD_CPMAXLENGTH];
  
  	//type of the module
	char modtype[SDD_CPMAXLENGTH];

	//module index
	int fd;
	
	//file descriptor name
	char filedescrip_prefix[SDD_FDPREFIXMAXLENGTH];
	
	//serial number
	char serial_number [SDD_SERIALNBMAXLENGTH];

};

//list of symbol for IOModule : IO module type and fd
typedef enum SDD_IOModuleEnum {

	} SDD_IOModuleEnum;


#endif /* SDD_IOMODULE_H */