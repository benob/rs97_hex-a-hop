AM_CFLAGS = $(CFLAGS) $(SDL_CFLAGS)
AM_CXXFLAGS = $(CXXFLAGS) $(SDL_CFLAGS)

bin_PROGRAMS = \
	hex-a-hop

noinst_PROGRAMS = \
	dat2bmp \
	bmp2dat

hex_a_hop_SOURCES = \
	gfx.cpp \
	gfx_list.h \
	hex_puzzzle.cpp \
	i18n.cpp \
	i18n.h \
	level_list.h \
	menus.h \
	packfile.h \
	savestate.h \
	sfx.cpp \
	sfx.h \
	state.h \
	system-directory.c \
	system-directory.h \
	system-relative.c \
	system-relative.h \
	text.cpp \
	text.h \
	tiletypes.h \
	video.h

hex_a_hop_LDADD = \
	$(SDL_LIBS)

dat2bmp_SOURCES = \
	tools/dat2bmp.c

dat2bmp_LDADD = \
	$(SDL_LIBS)

bmp2dat_SOURCES = \
	tools/bmp2dat.c

bmp2dat_LDADD = \
	$(SDL_LIBS)
