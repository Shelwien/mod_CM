
struct Counter {

  uint n0;
  uint n1;

  void Init( uint p0=1, uint p1=1 ) {
    n0 = p0 << SCALElog;
    n1 = p1 << SCALElog;
    if( n0+n1==0 ) n0=n1=1;
  }

  uint Eval( const uint mw ) {
//    if_e0( n0+n1==0 ) n0=n1=1;
    int p0 = (qword(n0)<<SCALElog) / (n0+n1+2);
    p0 = (p0*(SCALE-mw) + hSCALE*mw) / SCALE;
    if_e0( int(p0)<1 ) p0=1;
    if_e0( p0>mSCALE ) p0=mSCALE;
//    cmovs( int(p0-1),      p0, 1      );
//    cmovs( int(mSCALE-p0), p0, mSCALE );
    return p0;
  }

  void Update( const uint bit, const uint wr0, const uint dr1 ) {
    const int d = (dr1&1) ? -int(dr1>>1) : (dr1>>1);
    const uint wr1 = (int(wr0)+d) & mSCALE;

    if( bit ) {
      n0 = (qword(n0)*wr1+hSCALE) >> SCALElog;
      n1 = (qword(n1)*wr0+hSCALE) >> SCALElog;
      n1 += SCALE;
    } else {
      n0 = (qword(n0)*wr0+hSCALE) >> SCALElog;
      n1 = (qword(n1)*wr1+hSCALE) >> SCALElog;
      n0 += SCALE;
    }
  }

};

