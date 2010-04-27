vsinstr Debug\TestSuite.exe /COVERAGE
Start VSPerfMon.exe /COVERAGE /OUTPUT:testsuite
PAUSE
Debug\TestSuite.exe
PAUSE
VSPerfCmd.exe /SHUTDOWN
