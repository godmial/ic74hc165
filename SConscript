import os

src = []
cwd = GetCurrentDir()

src += Glob('*.c')

if GetConfig('PKG_IC74HC165_SAMPLE'):
    src += Glob('sample/*.c')

group = DefineGroup('Package/ic74hc165', src, depend = ['RT_USING_PIN'], CPPPATH = [cwd])

Return('group')
