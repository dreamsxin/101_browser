vsinstr out\vs2008\Debug\TestSuite.exe /COVERAGE
Start VSPerfMon.exe /COVERAGE /OUTPUT:testsuite
PAUSE
out\vs2008\Debug\TestSuite.exe
PAUSE
VSPerfCmd.exe /SHUTDOWN
