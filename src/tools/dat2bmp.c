// gcc -Wall -g dat2bmp.c -o dat2bmp `sdl-config --cflags --libs`

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include <SDL.h>

#define ERROR_PRINTF(...) { fprintf(stderr, "%s:%s:%u Error: ", __FILE__, __PRETTY_FUNCTION__, __LINE__); fprintf(stderr,__VA_ARGS__); fflush(stderr); }

int main(int argc, char** argv)
{
	FILE* f = NULL;
	uint32_t *tmp = NULL;
	const char *bmp = NULL;
	const char *dat = NULL;
	SDL_Surface * g = NULL;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s file.dat file.bmp\n", argv[0]);
		return -1;
	}

	dat = argv[1];
	bmp = argv[2];

	f = fopen(dat, "rb");
	if (!f)
	{
		ERROR_PRINTF("Unable to open file %s: %s", dat, strerror(errno));
		return -1;
	}

	short w,h;
	fread(&w, sizeof(w), 1, f);
	fread(&h, sizeof(h), 1, f);

	if (w>1500 || h>1500)
	{
		ERROR_PRINTF("Invalid file %s", dat);
		return -1;
	}

	tmp = (uint32_t *)calloc((int) w * h, sizeof(uint32_t));

	uint32_t c = 0;
	uint32_t cnt = 0;
	int p = 0;
	for (p = 0; p < (int) w * h; p++)
	{
		if (cnt)
			cnt -= 0x1;
		else
		{
			fread(&c, sizeof(c), 1, f);
			cnt = c >> 24;
			if (cnt==255)
				fread(&cnt, sizeof(cnt), 1, f);
		}
		tmp[p] = c | 0xff000000;
	}

	g = SDL_CreateRGBSurfaceFrom(tmp, w, h, 32, w*4, 
		0xff0000,
		0xff00,
		0xff,
		0xff000000 );

	if (SDL_SaveBMP(g, bmp) == -1)
	{
		ERROR_PRINTF("Unable to write BMP file %s: %s", bmp, strerror(errno));
		return -1;
	}

	fclose(f);
	free(tmp);
	return 0;
}
