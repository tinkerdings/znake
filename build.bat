@echo off

for /f "tokens=* usebackq" %%f in (`dir /b /s "C:\Program Files\*vcvarsall.bat*"`) do (set "file=%%f" & goto :next)
:next
call "%file%" x86

cl /EHsc /O2 /I .\include /Fo.\obj\ /Fe.\bin\znake.exe /std:c++20 .\*.cpp /link  /SUBSYSTEM:windows /ENTRY:mainCRTStartup .\lib\*.lib shell32.lib
