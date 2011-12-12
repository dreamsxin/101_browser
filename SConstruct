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

RFC1951 = env.Clone()
RFC1951.SharedLibrary('bin/RFC1951', Glob('src/RFC1951/*.c'))

font_server = env.Clone()
font_server.Append(LIBS = ['MiniStdlib'])
font_server.Program('bin/FontServer', Glob('src/FontServer/*.cpp'), LIBPATH = ['#/lib'])

GuiXlib = env.Clone()
GuiXlib.Append(LIBS = ['X11', 'GL'])
GuiXlib.Program('bin/GuiXlib', Glob('src/GuiXlib/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

#GuiXcb = env.Clone()
#GuiXcb.Append(LIBS = ['xcb', 'GL'])
#GuiXlib.Program('bin/GuiXcb', Glob('src/GuiXcb/*.cpp')+Glob('src/GuiOpenGL/*.cpp'))

Unicode = env.Clone()
Unicode.Append(LIBS = ['MiniStdlib'])
Unicode.SharedLibrary('bin/Unicode', Glob('src/Unicode/*.c')+Glob('src/Unicode/*.cpp')+Glob('src/Unicode/generated/*.cpp'), LIBPATH = ['#/lib'])

HTML5 = env.Clone()
HTML5.SharedLibrary('bin/HTML5', Glob('src/HTML5/*.c')+Glob('src/HTML5/*.cpp'))

SetjmpUtil = env.Clone()
SetjmpUtil.SharedLibrary('bin/SetjmpUtil', Glob('src/SetjmpUtil/*.c'))

CoroutinePosix = env.Clone()
CoroutinePosix.SharedLibrary('bin/CoroutinePosix', Glob('src/CoroutinePosix/*.c'))

PNG = env.Clone()
PNG.SharedLibrary('bin/PNG', Glob('src/PNG/*.c'))

CRC = env.Clone()
CRC.SharedLibrary('bin/CRC', Glob('src/CRC/*.c')+Glob('src/CRC/*.cpp'))

Ogg = env.Clone()
Ogg.SharedLibrary('bin/Ogg', Glob('src/Ogg/*.c'))

Matroska = env.Clone()
Matroska.SharedLibrary('bin/Matroska', Glob('src/Matroska/*.c'))

IO = env.Clone()
IO.Append(LIBS = ['MiniStdlib'])
IO.SharedLibrary('bin/IO', Glob('src/IO/*.c'), LIBPATH = ['#/lib'])

GIF = env.Clone()
GIF.SharedLibrary('bin/GIF', Glob('src/GIF/*.c'))

Network = env.Clone()
Network.SharedLibrary('bin/Network', Glob('src/NetworkPosix/*.c'))

DNS = env.Clone()
DNS.SharedLibrary('bin/DNS', Glob('src/DNS/*.c'))

testsuite = env.Clone()
# testsuite.Append(CCFLAGS = '-fprofile-arcs -ftest-coverage', LIBS = ['gcov'])
testsuite.Append(LIBS = ['MiniStdlib', 'BigNumber', 'RFC1950', 'RFC1951', 'Unicode', 'HTML5', 'Algorithm', 'SetjmpUtil', 'CoroutinePosix', 'GIF', 'Network', 'DNS', 'IO', 'PNG', 'CRC', 'Ogg', 'Matroska'])
testsuite.Program('bin/TestSuite', Glob('src/TestSuite/*.cpp')+Glob('src/TestSuite/*.c'), LIBPATH = ['#/bin','#/lib'])

