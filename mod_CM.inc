
#include "CM/modelC.h"

void CM_Model( Mixer& m ) {
  static int flag = 0;

  if( flag==0 ) { 
    flag = CM_Init(); 
    if( flag ) exit(1);
    flag=1;
  } 

  int p = CM_Update(y)/8;

//  m.set(0,1);
  m.add( stretch(4096-p) );
}


