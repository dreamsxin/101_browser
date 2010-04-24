SET PATH=%VSINSTALLDIR%\Team Tools\Performance Tools;%PATH%
del testsuite.coverage
vsinstr Debug\TestSuite.exe /COVERAGE
Start VSPerfMon.exe /COVERAGE /OUTPUT:testsuite
PAUSE
Debug\TestSuite.exe
PAUSE
VSPerfCmd.exe /SHUTDOWN
