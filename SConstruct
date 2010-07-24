
env = Environment()
env = Environment(CPPPATH='#/include')

ministdlib = env.Clone()
ministdlib.Library('lib/ministdlib', Glob('src/MiniStdlib/*.c')+Glob('src/MiniStdlib/*.cpp'))

rfc1950 = env.Clone()
rfc1950.SharedLibrary('bin/rfc1950', Glob('src/rfc1950/*.c')+Glob('src/rfc1950/*.cpp'))

font_server = env.Clone()
font_server.Append(LIBS = ['ministdlib'])
font_server.Program('bin/FontServer', Glob('src/FontServer/*.cpp'), LIBPATH = ['#/lib'])

guix = env.Clone()
guix.Append(LIBS = ['X11', 'GL'])
guix.Program('bin/GuiX', Glob('src/GuiX/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

network_posix = env.Clone()
network_posix.Program('bin/NetworkPosix', Glob('src/NetworkPosix/*.cpp'))

testsuite = env.Clone()
# testsuite.Append(CCFLAGS = '-fprofile-arcs -ftest-coverage', LIBS = ['gcov'])
testsuite.Append(LIBS = ['rfc1950'])
testsuite.Program('bin/TestSuite', Glob('src/TestSuite/*.cpp'), LIBPATH = ['#/bin'])

