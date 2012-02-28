set OLDPATH=%PATH%
SET PATH=%VSINSTALLDIR%\Team Tools\Performance Tools\;%PATH%

vsinstr out\vs2010\Debug\TestSuite.exe /COVERAGE
vsinstr out\vs2010\Debug\Algorithm.dll /COVERAGE
vsinstr out\vs2010\Debug\BigNumber.dll /COVERAGE
vsinstr out\vs2010\Debug\DNS.dll /COVERAGE
vsinstr out\vs2010\Debug\GIF.dll /COVERAGE
vsinstr out\vs2010\Debug\HTML5.dll /COVERAGE
vsinstr out\vs2010\Debug\IO.dll /COVERAGE
vsinstr out\vs2010\Debug\Unicode.dll /COVERAGE



Start VSPerfMon.exe /COVERAGE /OUTPUT:testsuite
PAUSE
out\vs2010\Debug\TestSuite.exe
PAUSE
VSPerfCmd.exe /SHUTDOWN

move out\vs2010\Debug\TestSuite.exe.orig out\vs2010\Debug\TestSuite.exe
move out\vs2010\Debug\Algorithm.dll.orig out\vs2010\Debug\Algorithm.dll
move out\vs2010\Debug\BigNumber.dll.orig out\vs2010\Debug\BigNumber.dll
move out\vs2010\Debug\DNS.dll.orig out\vs2010\Debug\DNS.dll
move out\vs2010\Debug\GIF.dll.orig out\vs2010\Debug\GIF.dll
move out\vs2010\Debug\HTML5.dll.orig out\vs2010\Debug\HTML5.dll
move out\vs2010\Debug\IO.dll.orig out\vs2010\Debug\IO.dll
move out\vs2010\Debug\Unicode.dll.orig out\vs2010\Debug\Unicode.dll

set PATH=%OLDPATH%
