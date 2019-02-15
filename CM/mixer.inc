
struct Mixer {
  word w;  // weights

  void Init( int w0 ) { 
    w = w0 + hSCALE;
  }

  int rdiv( int x, int a, int d ) {  
    return x>=0 ? (x+a)>>d : -((-x+a)>>d);
  }

  int Mixup( int w, int s1, int s0 /*, int wr*/ ) {
    int x = s1 + rdiv( int(w-hSCALE)*(s0-s1), 1<<(SCALElog-1), SCALElog );
if_e0( int(x)<1 ) x=1;
if_e0( x>mSCALE ) x=mSCALE;
    return x;
  }

  void Update( int y, int p0,int p1, int wq, int pm ) {
    int py = SCALE - (y<<SCALElog);
    int e = (py-pm);
    int d = rdiv( e*(p0-p1), 1<<(SCALElog-1), SCALElog );
    d = rdiv( d*wq, 1<<(SCALElog-1), SCALElog );
    int w1 = w + d;
if_e0( w1<0 ) w1=0;
if_e0( w1>SCALE+mSCALE ) w1=SCALE+mSCALE;
//if_e0( w1<=hSCALE ) w1=hSCALE+1;
//if_e0( w1>=hSCALE+SCALE ) w1=hSCALE+SCALE-1;
    w = w1;
  }
};

uint Extrap( int p1, int C ) {
  p1 = (((p1-hSCALE)*C) >> 13) + hSCALE;
//  p1 = ((sqword(p1-hSCALE)*C)/8192) + hSCALE;
  if_e0( int(p1)<1 ) p1=1;
  if_e0( p1>mSCALE ) p1=mSCALE;
//  cmovs( int(p1-1),      p1, 1      );
//  cmovs( int(mSCALE-p1), p1, mSCALE );
  return p1;
}

uint WExtrap( int p1, int C ) {
  p1 -= hSCALE;

//  if_e0( p1<1 ) p1=1;
//  if_e0( p1>mSCALE ) p1=mSCALE;
  if_e1( (p1>=1) && (p1<=mSCALE) ) {
    p1 = t_st[p1];
    p1 = Extrap( p1, C );
    p1 = t_sq[p1];
  }

  p1 += hSCALE;
  return p1;
}
