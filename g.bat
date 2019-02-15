@echo off

del *.exe

set incs=-DSTRICT -DNDEBUG -DWIN32 -I../Lib
rem -march=sse2 -mtune=sse2 -msse2 

set opts=-fstrict-aliasing -fomit-frame-pointer ^
-fexceptions -fno-rtti -fpermissive  ^
-fno-stack-protector -fno-stack-check -fno-check-new

rem -fprofile-use -fprofile-correction  -fwhole-program 

rem -flto -ffat-lto-objects -Wl,-flto -fuse-linker-plugin -Wl,-O -Wl,--sort-common -Wl,--as-needed -ffunction-sections

:set gcc=C:\MinGW710\bin\g++.exe -m32 
set gcc=C:\MinGW810\bin\g++.exe -m32
set gcc=C:\MinGW810x\bin\g++.exe 
set gcc=C:\MinGW820x\bin\g++.exe
set gcc=C:\MinGW820x\bin\g++.exe -msse2 -m64 -DNOASM
set path=%gcc%\..\

del *.exe *.o

%gcc% -std=gnu++1z -O9 -s %incs% %opts% -static paq8p.cpp CM/modelC.cpp modppmd64.lib -o paq8p_PC.exe

