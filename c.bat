@echo off

set ICLCFG=icl64.cfg

set icl=C:\IntelJ0117\bin-intel64\icl2a.bat /D_fputc=fputc /DNOASM

call %icl% paq8p.cpp CM/modelC.cpp modppmd64.lib 

del *.exp *.obj
