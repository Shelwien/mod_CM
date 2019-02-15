
struct M_Holder {
  Mixer* restrict pM;
  uint ctx;

  uint WC; 
  uint PC;
  uint wr; 
  uint p0;
  uint p1;
  uint p2;
  uint p2a;
  uint p3;
  uint* restrict _p0;
  uint* restrict _p1;

  INLINE
  void SetCoefs( void* _X0, uint DIM_X0, uint W0, uint _WC, uint _PC, uint _wr, uint* restrict pp0, uint* restrict pp1 ) {
    uint i;

    Mixer* restrict X0 = (Mixer*)_X0;
    for( i=0; i<DIM_X0; i++ ) X0[i].Init(W0);

    WC=_WC; PC=_PC; wr=_wr; pM=X0; ctx=0;

    _p0=pp0; _p1=pp1;
  }

  INLINE
  uint Eval( uint _ctx, uint mask=-1 ) {
    ctx = _ctx & mask;
    Mixer& M = pM[ctx];
    p0 = *_p0; p1 = *_p1;
    uint w0 = M.w; 
    w0 = WExtrap(w0,WC);
    p2 = M.Mixup( w0, p0, p1 ); p2a = t_sq[p2];
    p2 = Extrap( p2, PC ); 
    p3 = p2;
    p2 = t_sq[p2];
    return p2;
  }

  INLINE
  void Update( uint bit ) {
    Mixer& M = pM[ctx];
    M.Update( bit, p0, p1, wr, p2a );
  }

};

