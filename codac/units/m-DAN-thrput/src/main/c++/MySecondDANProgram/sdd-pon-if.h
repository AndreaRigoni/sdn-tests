#ifndef SDD_PON_IF_H
#define SDD_PON_IF_H

#include <cadef.h> /* Channel Access API definition, etc. */


#define SDD_PVMAXLENGTH 40
#define SDD_STRING_MAX_LENGTH 100
#define SDD_NB_OF_PONVAR 0 //generated by SDD

struct SDD_PONVar {

  char name[SDD_PVMAXLENGTH];
  
  //if read-> monitor=yes otherwise false
  bool isMonitored;
  
  chtype type;

  //the code can cast
  char initialvalue[SDD_STRING_MAX_LENGTH];

};

//list of symbols
typedef enum SDD_PONVarEnum {

		
} SDD_PONVarEnum;

//for each enum (bi/bo/mbbi/mbbo) name is symbolname suffix with _enum




#endif /* SDD_PON_IF_H */