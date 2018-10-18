// gcc -Wall -g bmp2dat.c -o bmp2dat `sdl-config --cflags --libs`

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include <SDL.h>

#define IMAGE_DAT_OR_MASK 0xff030303 // Reduce colour depth of images slightly for better compression (and remove useless top 8 bits!)

#define ERROR_PRINTF(...) { fprintf(stderr, "%s:%s:%u Error: ", __FILE__, __PRETTY_FUNCTION__, __LINE__); fprintf(stderr,__VA_ARGS__); fflush(stderr); }

static int Peek(SDL_Surface* i, int x, int y)
{
	if (x<0 || y<0 || x>=i->w || y>=i->h)
		return 0;
	unsigned int p=0;
	const int BytesPerPixel = i->format->BytesPerPixel;
	const int BitsPerPixel = i->format->BitsPerPixel;
	if (BitsPerPixel==8)
		p = ((unsigned char*)i->pixels)[i->pitch*y + x*BytesPerPixel];
	else if (BitsPerPixel==15 || BitsPerPixel==16)
		p = *(short*)(((char*)i->pixels) + (i->pitch*y + x*BytesPerPixel));
	else if (BitsPerPixel==32)
		p = *(unsigned int*)(((char*)i->pixels) + (i->pitch*y + x*BytesPerPixel));
	else if (BitsPerPixel==24)
		p = (int)((unsigned char*)i->pixels)[i->pitch*y + x*BytesPerPixel]
		  | (int)((unsigned char*)i->pixels)[i->pitch*y + x*BytesPerPixel] << 8
		  | (int)((unsigned char*)i->pixels)[i->pitch*y + x*BytesPerPixel] << 16;

	return p;
}

static int IsEmpty(SDL_Surface* im, int x, int y, int w, int h)
{
	int i, j;
	for (i=x; i<x+w; i++)
		for (j=y; j<y+h; j++)
			if (Peek(im,i,j) != Peek(im,0,im->h-1)) 
				return 0;
	return 1;
}

int main(int argc, char** argv)
{
	typedef unsigned int uint32;
	SDL_Surface * g = NULL;
	const char *bmp = NULL;
	const char *dat = NULL;
	int colourKey = 1;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s file.bmp file.dat\n", argv[0]);
		return -1;
	}

	bmp = argv[1];
	dat = argv[2];

	g = SDL_LoadBMP(bmp);

//	SDL_PixelFormat p;
//	p.sf = 1;
//	SDL_Surface* tmp = SDL_ConvertSurface(g, &p, SDL_SWSURFACE);

	short w=g->w, h=g->h;
	char* buf = (char*) g->pixels;
	if (colourKey)
	{
		while (IsEmpty(g, w-1, 0, 1, h) && w>1)
			w--;
		while (IsEmpty(g, 0, h-1, w, 1) && h>1)
			h--;
	}

	FILE* f = fopen(dat, "wb");
	fwrite(&w, sizeof(w), 1, f);
	fwrite(&h, sizeof(h), 1, f);

	uint32 mask = IMAGE_DAT_OR_MASK;
	int i;
	for (i=0; i<(int)w*h; )
	{
		uint32 c = (*(uint32*)&buf[(i%w)*3 + (i/w)*g->pitch] | mask);
		int i0 = i;
		while (i < (int)w*h && c == (*(uint32*)&buf[(i%w)*3 + (i/w)*g->pitch] | mask))
			i++;
		c &= 0xffffff;
		i0 = i-i0-1;
		if (i0 < 0xff)
			c |= i0 << 24;
		else
			c |= 0xff000000;

		fwrite(&c, sizeof(c), 1, f);

		if (i0 >= 0xff)
			fwrite(&i0, sizeof(i0), 1, f);
	}
	fclose(f);

	SDL_FreeSurface(g);

	return 0;
}
