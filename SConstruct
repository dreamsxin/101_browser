
env = Environment()
env = Environment(CPPPATH='#/include')

testsuite = env.Clone()
testsuite.Append(CCFLAGS = '-fprofile-arcs -ftest-coverage', LIBS = 'gcov')
testsuite.Program('bin/TestSuite', Glob('src/TestSuite/*.cpp'))

guix = env.Clone()
guix.Append(LIBS = ['X11', 'GL'])
guix.Program('bin/GuiX', Glob('src/GuiX/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

network_posix = env.Clone()
network_posix.Program('bin/NetworkPosix', Glob('src/NetworkPosix/*.cpp'))

