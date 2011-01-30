if ARGUMENTS.get('debug', 0):
    env = Environment(CCFLAGS = '-g', CXXFLAGS = '-std=gnu++0x', CPPPATH='#/include')
else:
    env = Environment(CCFLAGS = '-Wall', CXXFLAGS = '-std=gnu++0x', CPPPATH='#/include')

MiniStdlib = env.Clone()
MiniStdlib.Library('lib/MiniStdlib', Glob('src/MiniStdlib/*.c')+Glob('src/MiniStdlib/*.cpp'))

Algorithm = env.Clone()
Algorithm.SharedLibrary('bin/Algorithm', Glob('src/Algorithm/*.c')+Glob('src/Algorithm/*.cpp'))

BigNumber = env.Clone()
BigNumber.Append(LIBS = ['MiniStdlib'])
BigNumber.SharedLibrary('bin/BigNumber', Glob('src/BigNumber/*.c')+Glob('src/BigNumber/*.cpp'), LIBPATH = ['#/lib'])

RFC1950 = env.Clone()
RFC1950.SharedLibrary('bin/RFC1950', Glob('src/RFC1950/*.c')+Glob('src/RFC1950/*.cpp'))

font_server = env.Clone()
font_server.Append(LIBS = ['MiniStdlib'])
font_server.Program('bin/FontServer', Glob('src/FontServer/*.cpp'), LIBPATH = ['#/lib'])

guix = env.Clone()
guix.Append(LIBS = ['X11', 'GL'])
guix.Program('bin/GuiX', Glob('src/GuiX/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

network_posix = env.Clone()
network_posix.Program('bin/NetworkPosix', Glob('src/NetworkPosix/*.cpp'))

Unicode = env.Clone()
Unicode.Append(LIBS = ['MiniStdlib'])
Unicode.SharedLibrary('bin/Unicode', Glob('src/Unicode/*.c')+Glob('src/Unicode/*.cpp')+Glob('src/Unicode/generated/*.cpp'), LIBPATH = ['#/lib'])

HTML5 = env.Clone()
HTML5.SharedLibrary('bin/HTML5', Glob('src/HTML5/*.c')+Glob('src/HTML5/*.cpp'))

CPU = env.Clone()
CPU.SharedLibrary('bin/CPU', Glob('src/CPU/*.c'))

CoroutinePosix = env.Clone()
CoroutinePosix.SharedLibrary('bin/CoroutinePosix', Glob('src/CoroutinePosix/*.c'))

PNG = env.Clone()
PNG.SharedLibrary('bin/PNG', Glob('src/PNG/*.c')+Glob('src/PNG/*.cpp'))

IO = env.Clone()
IO.SharedLibrary('bin/IO', Glob('src/IO/*.c'))

GIF = env.Clone()
GIF.SharedLibrary('bin/GIF', Glob('src/GIF/*.c'))

testsuite = env.Clone()
# testsuite.Append(CCFLAGS = '-fprofile-arcs -ftest-coverage', LIBS = ['gcov'])
testsuite.Append(LIBS = ['MiniStdlib', 'BigNumber', 'RFC1950', 'Unicode', 'HTML5', 'Algorithm', 'CPU', 'CoroutinePosix', 'GIF', 'IO', 'PNG'])
testsuite.Program('bin/TestSuite', Glob('src/TestSuite/*.cpp')+Glob('src/TestSuite/*.c'), LIBPATH = ['#/bin','#/lib'])

