
#include <math.h>

#define M_LOG2E 1.44269504088896340736
double log2( double a ) { return M_LOG2E*log(a); }

double exp2( double a ) { return exp( a/M_LOG2E ); }

double st( double p ) { return log2((1-p)/p); }

double sq( double p ) { return 1.0/(1.0+exp2(p)); }

const double st_coef = (hSCALE-1)/log2(SCALE-1);
const double sq_coef = 1.0 / st_coef;

uint st_i( uint p ) {
  p = st(double(p)/SCALE) * st_coef + hSCALE;
  if_e0( int(p)<1 ) p=1;
  if_e0( p>mSCALE ) p=mSCALE;
  return p;
}

uint sq_i( uint p ) {
//double q = double(int(p)-hSCALE) * sq_coef;
//printf( "%7.5lf ", q );
  p = sq( double(int(p)-hSCALE) * sq_coef ) * SCALE;
  if_e0( int(p)<1 ) p=1;
  if_e0( p>mSCALE ) p=mSCALE;
  return p;
}

double st_d( double p ) {
  p = st(p/SCALE);
  return p;
}

double sq_d( double p ) {
  p = sq(p) * SCALE;
  return p;
}

word t_st[SCALE];
word t_sq[SCALE];

uint Init_ST_SQ( void ) {
  uint i,s,p,x,y;

  for( i=1; i<SCALE; i++ ) t_sq[i] = sq_i(i);

  x=0; t_st[x]=0;
  for( i=1; i<SCALE; i++ ) {
    s = st_i(i);
    t_st[i] = s;

    if( s!=t_st[x] ) {
      y = i-1;
//      if( x<y ) printf( "%i .. %i; %i %i %i\n", x, y, t_sq[st_i(x)],t_sq[st_i(y)], (x+y+1)/2 );
      t_sq[ t_st[x] ] = (x+y+1)/2;
      x = i;
    }
  }

  for( i=1; i<SCALE; i++ ) {
    s = t_st[i];
    p = t_sq[s];
//    printf( "%5i %5i %5i\n", i, s, p );
  }
  return 0;
}
