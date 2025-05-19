from building import *
import rtconfig

cwd = GetCurrentDir()

src = Glob('*.c')
CPPPATH = [cwd]
LOCAL_CCFLAGS = ''


if GetDepend(['PKG_IC74HC165_SAMPLE']):
    src += Glob('sample/*.c')

group = DefineGroup('ic74hc165', src, depend=['PKG_USING_IC74HC165'], CPPPATH=CPPPATH, LOCAL_CCFLAGS=LOCAL_CCFLAGS)

Return('group')
