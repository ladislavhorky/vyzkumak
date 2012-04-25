#include <vyzkumak/graphics/gfx_framework.h>

/*
By Martin Sochor 2012
*/

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
	if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
		//Convert the pixels to 32 bit
		Uint32 *pixels = (Uint32 *)surface->pixels;
		
		//Get the requested pixel
		return pixels[ ( y * surface->w ) + x ];
	} else return SDL_MapRGBA(surface->format, 0, 0, 0, 0);
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
		//Convert the pixels to 32 bit 
		Uint32 *pixels = (Uint32 *)surface->pixels; 
	
		//Set the pixel
		pixels[ ( y * surface->w ) + x ] = pixel;
	}
}

void draw_line( SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 pixel )
{
    signed char ix;
    signed char iy;
 
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x = (x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1;
    int delta_y = (y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1;
 
    put_pixel32(surface, x1, y1, pixel);
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error = delta_y - (delta_x >> 1);
 
        while (x1 != x2)
        {
            if (error >= 0)
            {
                if (error || (ix > 0))
                {
                    y1 += iy;
                    error -= delta_x;
                }
                // else do nothing
            }
            // else do nothing
 
            x1 += ix;
            error += delta_y;
 
            put_pixel32(surface, x1, y1, pixel);
        }
    }
    else
    {
        // error may go below zero
        int error = delta_x - (delta_y >> 1);
 
        while (y1 != y2)
        {
            if (error >= 0)
            {
                if (error || (iy > 0))
                {
                    x1 += ix;
                    error -= delta_y;
                }
                // else do nothing
            }
            // else do nothing
 
            y1 += iy;
            error += delta_x;
 
            put_pixel32(surface, x1, y1, pixel);
        }
    }
}

void draw_circle( SDL_Surface *surface, int x0, int y0, int radius, Uint32 pixel ) {
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	put_pixel32(surface, x0, y0 + radius, pixel);
	put_pixel32(surface, x0, y0 - radius, pixel);
	put_pixel32(surface, x0 + radius, y0, pixel);
	put_pixel32(surface, x0 - radius, y0, pixel);

	while(x < y) {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;    
		put_pixel32(surface, x0 + x, y0 + y, pixel);
		put_pixel32(surface, x0 - x, y0 + y, pixel);
		put_pixel32(surface, x0 + x, y0 - y, pixel);
		put_pixel32(surface, x0 - x, y0 - y, pixel);
		put_pixel32(surface, x0 + y, y0 + x, pixel);
		put_pixel32(surface, x0 - y, y0 + x, pixel);
		put_pixel32(surface, x0 + y, y0 - x, pixel);
		put_pixel32(surface, x0 - y, y0 - x, pixel);
	}
}

void wipe( SDL_Surface *surface, Uint32 pixel ) {
	for (int i = 0; i < surface->w; i++) {
		for (int j = 0; j < surface->h; j++) {
			put_pixel32(surface, i, j, pixel);
		}
	}
}
