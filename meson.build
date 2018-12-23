project('custom-discord-rpc', 'c')

compiler = meson.get_compiler('c')

drpc_dep = compiler.find_library('libdiscord-rpc', dirs: meson.source_root())
inih_dep = compiler.find_library('libinih', dirs: meson.source_root())

inc = include_directories('discord-rpc/include', 'inih/include/')

executable('cdr', 'main.c', 'stuff.c', 'stuff.h', dependencies: [drpc_dep, inih_dep], include_directories: inc)
