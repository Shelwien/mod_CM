
template< int _cfg_tblbytes, class hashrec0 >
struct Hashtable {

  struct hashcell {
    hashrec0 key;
  };

  struct hinf {
    hashrec0 h0;
//    uint i; // primary cell (from h0)
    uint j; // actual cell
  };

  static const uint cfg_wrapwinsize = 0xFFFF0000;
  static const uint cfg_tbldepth = 32;
  static const uint cfg_tblbytes = _cfg_tblbytes << 20;
  static const uint cfg_tblsize = cfg_tblbytes/sizeof(hashcell);

  hashcell* htbl;

//  uint match_ofs;
  uint match_i;
  uint f_match;

//  uint w_ptr; // for htbl weeding
//  uint w_kill;

  hinf hrow[cfg_tbldepth];

  uint Init( void ) {
    //w_ptr=0; w_kill=0;
    //cfg_tblsize = cfg_tblbytes/sizeof(hashcell);
    htbl = VAlloc<hashcell>(cfg_tblsize);
    //memset( htbl, 0, sizeof(htbl) );
    return (htbl==0);
  }

  void Quit( void ) {
    VFree(htbl);
  }

  uint Prefetch( hashrec0& tmp ) {
    uint i0 = tmp.hash % cfg_tblsize;   
    volatile uint v = htbl[i0].ofs;
    return v;
  }

  // need to insert tmp_ofs at htbl[i0] or around
  NOINLINE
  uint FindInsert( hashrec0& tmp, uint tmp_ofs=-2 ) {

    uint i,j,i0,j0,k,l,f_match=0,ofs,hrow_k_i;
    tmp_ofs+=2; // reserve 0 and 1

    i0 = tmp.hash % cfg_tblsize; // primary cell
    j0 = (i0+cfg_tbldepth) % cfg_tblsize; // terminator cell

    // look up the required hash for tbldepth cells
    for( i=i0; i!=j0; i=(i+1)%cfg_tblsize ) {
      //ofs = htbl[i].ofs;
      //if( ofs==0 ) break; // empty cell
      //if( ofs==1 ) continue; // marked by Weed, skip
      if( htbl[i].key.hash==0 ) break;

//      if( tmp.hash==htbl[i].key.hash ) { f_match=1; match_i=i; match_ofs=ofs-2; break; }
      if( tmp.hash==htbl[i].key.hash ) { f_match=1; match_i=i; break; }
    }

    // insert if no match
    if( (f_match==0) && (tmp_ofs>0) ) {

      uint md=0; uint mk=0;

      for( k=0,i=i0; i!=j0; ++k,i=(i+1)%cfg_tblsize ) {
        if( htbl[i].key.hash==0 ) break;
//        ofs = htbl[i].ofs;
//        if( ofs==0 ) break;

        hrow[k].h0 = htbl[i].key;
//        hrow[k].ofs = ofs;
 
        hrow_k_i = hrow[k].h0.hash % cfg_tblsize; // preferable location //tmp.hash % cfg_tblsize;

//        hrow[k].i = hrow[k].h0.hash % cfg_tblsize; // preferable location //tmp.hash % cfg_tblsize;
        hrow[k].j = i; // actual location

        uint d = (hrow[k].j+cfg_tblsize-hrow_k_i) % cfg_tblsize;
//        uint d = (hrow[k].j+cfg_tblsize-hrow[k].i) % cfg_tblsize;
//        if_e0( hrow[k].ofs==1 ) d=-1;
        if( d>=md ) md=d,mk=k; // find the most distant cell
      }

      // move most distant cell to last pos
      if( k>0 ) {
        uint mkj=hrow[k-1].j;
        for( j=k-1; j>mk; j-- ) hrow[j].j=hrow[j-1].j;
        hrow[mk].j=mkj;
      }

      // shift the row right
      for( j=0; j<k; j++ ) hrow[j].j=(hrow[j].j+1)%cfg_tblsize;

      // choose the element to replace: first unused call in row, or most distant cell if all used
      if( k<cfg_tbldepth ) mk=k++;

      // insert
      hrow[mk].h0 = tmp;
//      hrow[mk].ofs = tmp_ofs;
      hrow[mk].j = i0;

      // store the row back into hashtable
      for( j=0; j<k; j++ ) {
        hashcell& tmp = htbl[hrow[j].j];
//        tmp.ofs = hrow[j].ofs;
        tmp.key = hrow[j].h0;
      }

      match_i = i0;
    }

    this->f_match = f_match;
    return f_match;
  }

};

