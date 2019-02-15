
#include "common.inc"
#include "coro2b.inc"
#include "sh_v1m.inc"

#include "logistic.inc"
#include "counter.inc"
#include "mixer.inc"

#include "valloc.inc"
#include "hashfunc.inc"
#include "hashtable.inc"

#include "holder_c.inc"
#include "holder_m.inc"

//#include "sh_mapping.inc"
#include "MOD\sh_model_h.inc"

struct ModelC : Coroutine {

  enum{ DECODE=1 };
  uint p;
  uint bit;

//  uint f_len;

  struct C_Hasher {
    uint hash;
    Counter C;
    template< class HTbl >
    uint Eval( C_Holder& H_C3, HTbl hC3, uint ctx ) {
      hash = ctx;
      hC3.FindInsert(*this,1);
      H_C3.pC = &hC3.htbl[hC3.match_i].key.C;
      return 0;
    }
  };

  struct CM_Hasher {
    uint hash;
    Counter C;
    Mixer M;
    template< class HTbl >
    uint Eval( C_Holder& H_C3, M_Holder& H_X1, HTbl hC3, uint ctx ) {
      hash = ctx;
      hC3.FindInsert(*this,1);
      H_C3.pC = &hC3.htbl[hC3.match_i].key.C;
      H_X1.pM = &hC3.htbl[hC3.match_i].key.M;
      return 0;
    }
  };

  Hashtable<64,CM_Hasher> hC3; // 64M, order3
  Hashtable<64,CM_Hasher> hC4; // 64M, order4
  Hashtable<64,C_Hasher > hC5; // 64M, order5

  Counter C0[256];
  Counter C1[256*256];
  Counter C2[256*256*256]; // 128M?

//  Mixer X0[256*256*256];
//  Mixer X1[256*256*256];
  Mixer X2[256*256*256];
  Mixer X3[256*256];
  Mixer X4[256*256];
  Mixer X5[256*256];


  uint Init( void ) {
    uint r = 0;

    Init_ST_SQ();
    coro_init();

    r = hC3.Init();
    r|= hC4.Init();
    r|= hC5.Init();

    return r;
  }

  void Quit( void ) {
    hC3.Quit();
    hC4.Quit();
    hC5.Quit();
  }

  void do_process( void ) {

    uint i,j,c,z;
    uint o0_ctx=0,o1_ctx,o2_ctx,o3_ctx,o4_ctx,o5_ctx,o6_ctx,o7_ctx,o8_ctx;

    enum{ ctxlen=16 };
    byte ctx[ctxlen];
    for( i=0; i<ctxlen; i++ ) ctx[i]=0;

    uint orderM1 = 0;

    CM_Hasher tC3; Counter (&C3)[1]=(Counter(&)[1])tC3.C; Mixer (&X1)[1]=(Mixer(&)[1])tC3.M;
    CM_Hasher tC4; Counter (&C4)[1]=(Counter(&)[1])tC4.C; Mixer (&X0)[1]=(Mixer(&)[1])tC4.M;
    C_Hasher tC5;  Counter (&C5)[1]=(Counter(&)[1])tC5.C;

    C_Holder H_C0; H_C0.SetCoefs( C0,DIM(C0), M_f0P0,M_f0P1, M_f0mw, M_f0C, M_f0wr0, M_f0wr1 );
    C_Holder H_C1; H_C1.SetCoefs( C1,DIM(C1), M_f1P0,M_f1P1, M_f1mw, M_f1C, M_f1wr0, M_f1wr1 );
    C_Holder H_C2; H_C2.SetCoefs( C2,DIM(C2), M_f2P0,M_f2P1, M_f2mw, M_f2C, M_f2wr0, M_f2wr1 );
    C_Holder H_C3; H_C3.SetCoefs( C3,DIM(C3), M_f3P0,M_f3P1, M_f3mw, M_f3C, M_f3wr0, M_f3wr1 );
    C_Holder H_C4; H_C4.SetCoefs( C4,DIM(C4), M_f4P0,M_f4P1, M_f4mw, M_f4C, M_f4wr0, M_f4wr1 );
    C_Holder H_C5; H_C5.SetCoefs( C5,DIM(C5), M_f5P0,M_f5P1, M_f5mw, M_f5C, M_f5wr0, M_f5wr1 );

    M_Holder H_X0; H_X0.SetCoefs( X0,DIM(X0), M_X0W0,M_X0WC, M_X0PC, M_X0wr,&H_C5.p0,&H_C4.p0 );
    M_Holder H_X1; H_X1.SetCoefs( X1,DIM(X1), M_X1W0,M_X1WC, M_X1PC, M_X1wr,&H_X0.p3,&H_C3.p0 );
    M_Holder H_X2; H_X2.SetCoefs( X2,DIM(X2), M_X2W0,M_X2WC, M_X2PC, M_X2wr,&H_X1.p3,&H_C2.p0 );
    M_Holder H_X3; H_X3.SetCoefs( X3,DIM(X3), M_X3W0,M_X3WC, M_X3PC, M_X3wr,&H_X2.p3,&H_C1.p0 );
    M_Holder H_X4; H_X4.SetCoefs( X4,DIM(X4), M_X4W0,M_X4WC, M_X4PC, M_X4wr,&orderM1,&H_C0.p0 );
    M_Holder H_X5; H_X5.SetCoefs( X5,DIM(X5), M_X5W0,M_X5WC, M_X5PC, M_X5wr,&H_X4.p3,&H_X3.p3 );
 
//    rc_Init();

    o1_ctx = 0; // o1 context
    o2_ctx = 0; // o2 context
    for( i=0; /*i<f_len*/; i++ ) {

      o3_ctx = hash_o3( ctx[ctxlen-3],ctx[ctxlen-2],ctx[ctxlen-1] );
      o4_ctx = hash_o4( ctx[ctxlen-4],ctx[ctxlen-3],ctx[ctxlen-2],ctx[ctxlen-1] );
//      o5_ctx = hash_o4( ctx[ctxlen-5],ctx[ctxlen-4],ctx[ctxlen-3],ctx[ctxlen-2] ) ^ o3_ctx;
      o5_ctx = hash_o4( ctx[ctxlen-5],byte(o4_ctx),byte(o4_ctx>>8),byte(o4_ctx>>16) );

//      if( DECODE==0 ) c = get();

      #pragma unroll(8) always
      for( j=128,z=1; j>0; j>>=1 ) {

        tC5.Eval( H_C5,      hC5, o5_ctx^(z<<8) ); 
        tC4.Eval( H_C4,H_X0, hC4, o4_ctx^(z<<8) ); 
        tC3.Eval( H_C3,H_X1, hC3, o3_ctx^(z<<8) ); 

        H_C5.Eval( o5_ctx^(z<<8), 0 );
        H_C4.Eval( o4_ctx^(z<<8), 0 );
        H_X0.Eval( (o4_ctx%DIM(X0))^z, 0 ); // mix(C5,C4)

        H_C3.Eval( o3_ctx^(z<<8), 0 );
        H_X1.Eval( (o3_ctx%DIM(X1))^z, 0 ); // mix(X0,C3)

        H_C2.Eval( o2_ctx^(z<<0) );
        H_X2.Eval( o2_ctx^z );              // mix(X1,C2)

        H_C1.Eval( o1_ctx^(z<<0) );
        H_X3.Eval( o1_ctx^z );              // mix(X2,C1)

        H_C0.Eval( o0_ctx^(z<<0) );
        H_X4.Eval( o1_ctx^z );              // mix(oM,C0)

        H_X5.Eval( (o1_ctx&0xC000)+z );     // mix(X4,X3)

        this->p = H_X5.p2;
        //bit = rc_BProcess( H_X5.p2, c&j );
        yield(this,2);

        H_C0.Update( bit );
        H_C1.Update( bit );
        H_C2.Update( bit );
        H_C3.Update( bit );
        H_C4.Update( bit );
        H_C5.Update( bit );

        H_X0.Update( bit );
        H_X1.Update( bit );
        H_X2.Update( bit );
        H_X3.Update( bit );
        H_X4.Update( bit );
        H_X5.Update( bit );

        z += z + bit;
      }

//      if( DECODE==1 ) put( z );

      o2_ctx = (o1_ctx | byte(z))<<8;
      o1_ctx = byte(z)<<8;

      for( j=0; j<ctxlen-1; j++ ) ctx[j]=ctx[j+1];
      ctx[ctxlen-1] = z;
    }

//    rc_Quit();

    yield(this,0);
  }

};

ModelC CM;

extern "C" {

uint __cdecl CM_Init( void ) { return CM.Init(); }

uint __cdecl CM_Update( uint _bit ) { 
  CM.bit = _bit;
  CM.coro_call(&CM); 
  return CM.p;
}

void __cdecl CM_Quit( void ) { return CM.Quit(); }

}
