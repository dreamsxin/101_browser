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

set PATH=%OLDPATH%
