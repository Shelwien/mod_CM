
struct C_Holder {
  Counter* restrict pC;
  uint ctx;

  uint mw; // noise weight
  uint K;  // prior-to-posterior coef
  uint wr; // same-as-current bit's freq update rate
  uint dr; // different bit's freq update rate delta
  uint p0;

  INLINE
  void SetCoefs( void* _C0, uint DIM_C0, uint P0, uint P1, uint _mw, uint _K, uint _wr, uint _dr ) {
    uint i;

    Counter* restrict C0 = (Counter*)_C0;
    for( i=0; i<DIM_C0; i++ ) C0[i].Init(P0,P1);

    mw=_mw; K=_K; wr=_wr; dr=_dr; pC=C0; ctx=0;
  }

  INLINE
  uint Eval( uint _ctx, uint mask=-1 ) {
    ctx = _ctx & mask;
    Counter& C = pC[ctx]; 
    p0 = C.Eval(mw);
    p0 = t_st[p0]; 
    p0 = Extrap( p0, K );
    return p0;
  }

  INLINE
  void Update( uint bit ) {
    Counter& C = pC[ctx];
    C.Update( bit, wr, dr );
  }
};

