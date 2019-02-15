
#ifndef MEM_COMMIT
  extern "C" {
    void* __stdcall VirtualAlloc( void* lpAddress, size_t dwSize, uint flAllocationType, uint flProtect );
    uint __stdcall VirtualFree( void* lpAddress, size_t dwSize, uint dwFreeType );
  }
  #define MEM_COMMIT     0x1000
  #define PAGE_READWRITE 0x04
  #define MEM_RELEASE    0x8000
#endif

template< class T >
T* VAlloc( qword s ) {
  s *= sizeof(T);
  s = (s+0xFFF) & (~qword(0xFFF));
  void* r = VirtualAlloc(0, s, MEM_COMMIT, PAGE_READWRITE );
  return (T*)r;
}

void VFree( void* p ) {
  VirtualFree( p, 0, MEM_RELEASE );
}
