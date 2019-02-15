//////////////////////////// ppmdModel //////////////////////////

#define NOINLINE __declspec(noinline)
template <class T> T Min( T x, T y ) { return (x<y) ? x : y; }
template <class T> T Max( T x, T y ) { return (x>y) ? x : y; }

//__declspec(align(4096)) static ppmd_Model ppmd_12_256_1;

void ppmdModel( Mixer& m ) {
  static void* PM = 0;

  if( PM==0 ) { 
    PM = ppmd_Alloc(); 
    if( PM ) if( ppmd_Init( PM, 12, 1680, 1 ) ) PM=0;
    if( PM==0 ) exit(1);
  } else {
    ppmd_Setbit(PM,y);
  }

  int p = ppmd_Predict(PM,4096);

//printf( "%i %4i\n", y, p );

//  m.set(0,1);
  m.add( stretch(4096-p) );

}


