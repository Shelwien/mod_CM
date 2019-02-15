
uint lowbias32( uint x ) {
  x ^= x >> 16;
  x *= uint(0x7feb352d);
  x ^= x >> 15;
  x *= uint(0x846ca68b);
  x ^= x >> 16;
  return x;
}

uint hash_o3( uint b1, uint b2, uint b3 ) {
  uint x = b1;
  x |= b2 <<  8;
  x |= b3 << 16;
  x |= b1 << 24;
  x = lowbias32(x); // & hashmask;
  return x;
}

uint hash_o4( uint b0, uint b1, uint b2, uint b3 ) {
  uint x = b0;
  x |= b1 <<  8;
  x |= b2 << 16;
  x |= b3 << 24;
  x = lowbias32(x); // & hashmask;
  return x;
}
