# custom-discord-rpc
configurable Discord rich presence "client" written in **C**

## config format
see `config.ini` as an example  
```c
typedef struct {
	const char *app_id, *state, *details, *large_image_key, *large_image_text, *small_image_key, *small_image_text; /* check rich presence docs */
	bool elapsed_time_enabled;
	int remaining_time; /* max 86400 seconds (5 byte string) */
} Config;
```

## building
to build **custom-discord-rpc** you need:  
- a functional **C** compiler
- [Ninja](https://ninja-build.org/)
- [Meson](https://mesonbuild.com/Getting-meson.html)
- [discord-rpc](https://github.com/discordapp/discord-rpc)
- [inih](https://github.com/benhoyt/inih)

#### building **custom-discord-rpc**:  
- `libdiscord-rpc.so` and `libinih.so` in the project's root directory
- `meson build`
- `ninja -C build`
  
#### building **inih**
`meson.build` for **inih**: 
```
project('inih', 'c')
shared_library('inih', ['ini.c', 'ini.h'])
```
