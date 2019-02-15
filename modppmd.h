extern "C" {

void* __stdcall ppmd_Alloc( void );

void __stdcall ppmd_Free( void* p );

int __stdcall ppmd_Init( void* p, unsigned ppmd_order, unsigned ppmd_memory, unsigned ppmd_restore );

void __stdcall ppmd_Quit( void* p );

int __stdcall ppmd_GetUsedMemory( void* p );

int __stdcall ppmd_Predict( void* p, int SCALE );

void __stdcall ppmd_Setbit( void* p, int bit );

}
