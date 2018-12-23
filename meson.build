project('custom-discord-rpc', 'c')

compiler = meson.get_compiler('c')

drpc_dep = compiler.find_library('libdiscord-rpc', dirs: meson.source_root())
inih_dep = compiler.find_library('libinih', dirs: meson.source_root())

cdr_dep = [drpc_dep, inih_dep]
cdr_inc = include_directories('discord-rpc/include', 'inih/include/')

subdir('src')
