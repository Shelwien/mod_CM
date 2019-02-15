
typedef unsigned int   uint;

extern "C" {

uint __cdecl CM_Init( void );
uint __cdecl CM_Update( uint _bit ); // return probability
void __cdecl CM_Quit( void );

}
