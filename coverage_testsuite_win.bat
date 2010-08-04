SET PATH=%VSINSTALLDIR%\Team Tools\Performance Tools\;%PATH%

vsinstr out\vs2008\Debug\TestSuite.exe /COVERAGE
vsinstr out\vs2008\Debug\BigNumber.dll /COVERAGE
vsinstr out\vs2008\Debug\HTML5.dll /COVERAGE
Start VSPerfMon.exe /COVERAGE /OUTPUT:testsuite
PAUSE
out\vs2008\Debug\TestSuite.exe
PAUSE
VSPerfCmd.exe /SHUTDOWN
