if ARGUMENTS.get('debug', 0):
    env = Environment(CCFLAGS = '-g',CPPPATH='#/include')
else:
    env = Environment(CPPPATH='#/include')

MiniStdlib = env.Clone()
MiniStdlib.Library('lib/MiniStdlib', Glob('src/MiniStdlib/*.c')+Glob('src/MiniStdlib/*.cpp'))

Algorithm = env.Clone()
Algorithm.SharedLibrary('bin/Algorithm', Glob('src/Algorithm/*.c')+Glob('src/Algorithm/*.cpp'))

BigNumber = env.Clone()
BigNumber.Append(LIBS = ['MiniStdlib'])
BigNumber.SharedLibrary('bin/BigNumber', Glob('src/BigNumber/*.c')+Glob('src/BigNumber/*.cpp'), LIBPATH = ['#/lib'])

rfc1950 = env.Clone()
rfc1950.SharedLibrary('bin/rfc1950', Glob('src/rfc1950/*.c')+Glob('src/rfc1950/*.cpp'))

font_server = env.Clone()
font_server.Append(LIBS = ['MiniStdlib'])
font_server.Program('bin/FontServer', Glob('src/FontServer/*.cpp'), LIBPATH = ['#/lib'])

guix = env.Clone()
guix.Append(LIBS = ['X11', 'GL'])
guix.Program('bin/GuiX', Glob('src/GuiX/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

network_posix = env.Clone()
network_posix.Program('bin/NetworkPosix', Glob('src/NetworkPosix/*.cpp'))

Unicode = env.Clone()
Unicode.SharedLibrary('bin/Unicode', Glob('src/Unicode/*.c')+Glob('src/Unicode/*.cpp')+Glob('src/Unicode/generated/*.cpp'))

HTML5 = env.Clone()
HTML5.SharedLibrary('bin/HTML5', Glob('src/HTML5/*.c')+Glob('src/HTML5/*.cpp'))

CPU = env.Clone()
CPU.SharedLibrary('bin/CPU', Glob('src/CPU/*.c'))

PNG = env.Clone()
CPU.SharedLibrary('bin/PNG', Glob('src/PNG/*.c'))

testsuite = env.Clone()
# testsuite.Append(CCFLAGS = '-fprofile-arcs -ftest-coverage', LIBS = ['gcov'])
testsuite.Append(LIBS = ['MiniStdlib', 'BigNumber', 'rfc1950', 'Unicode', 'HTML5', 'Algorithm', 'CPU'])
testsuite.Program('bin/TestSuite', Glob('src/TestSuite/*.cpp')+Glob('src/TestSuite/*.c'), LIBPATH = ['#/bin','#/lib'])

