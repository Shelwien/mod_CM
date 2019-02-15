
#include "modelC.h"

template <int ProcMode>
struct Model : Rangecoder_SH1m<ProcMode> {

  enum{ DECODE=ProcMode };
  typedef Rangecoder_SH1m<ProcMode> RC;

  uint f_len;

  uint Init( uint _f_len=0 ) {
    uint r = 0;
    f_len = _f_len;
    r = CM_Init();
    return r;
  }

  void Quit( void ) {
    CM_Quit();
  }

  void do_process( void ) {

    uint i,j,c,z,bit=0;

    if( DECODE==0 ) {
      for( i=0; i<32; i+=8 ) put( f_len>>i );
    } else {
      f_len=0;
      for( i=0; i<32; i+=8 ) f_len |= get()<<i;
    }

    rc_Init();

    for( i=0; i<f_len; i++ ) {

      if( DECODE==0 ) c = get();

      #pragma unroll(8) always
      for( j=128,z=1; j>0; j>>=1 ) {
        bit = rc_BProcess( CM_Update(bit), c&j );
        z += z + bit;
      }

      if( DECODE==1 ) put( z );
    }

    rc_Quit();

    yield(this,0);
  }

};

