#ifndef _GFX_FRAMEWORK_H_
#define _GFX_FRAMEWORK_H_

#include <SDL.h>
//#undef main		//lol WTF

//#pragma comment(lib,"SDL.lib")
//#pragma comment(lib,"SDLmain.lib")

//obecné kreslící funkce

Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
void draw_line( SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 pixel );
void draw_circle( SDL_Surface *surface, int x0, int y0, int radius, Uint32 pixel );
void wipe( SDL_Surface *surface, Uint32 pixel );


#endif