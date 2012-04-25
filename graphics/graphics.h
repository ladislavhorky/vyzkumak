#ifndef __VYZKUM_GRAPHICS__
#define __VYZKUM_GRAPHICS__

/*
InitGraphics() by Martin Sochor 2011
*/

#include <ctime>
#include <cmath>
#include <cstdlib>
#include <vector>

#include <vyzkumak/graphics/gfx_framework.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *screen = NULL;
SDL_Event event;


bool InitGraphics(){
	srand(time(0));

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;

	//Initialize screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if( screen == NULL )return false;
	SDL_WM_SetCaption( "GO", NULL );

	//If everything initialized properly
	return true;
}

void DestroyGraphics(){
	//Quit SDL
	SDL_Quit();
}

/*
draws one candidate to cell CELL_WIDTH x CELL_WIDTH with top left corner specified by (x,y)
in given scale
*/
#define CELL_WIDTH 160

template<int dim, typename vectorType, typename evalType>
inline void DrawCandidate(int x, int y, basicCandidate<dim,vectorType>* cand, double scale){
	#define DIST(p1,p2) moleculePotentialEnergy<dim,vectorType,evalType>::dist[dim*(p1) + (p2)]
	#define COORD(idx) (cand->components[(idx)]*scale + CELL_WIDTH/2)
	
	int i,j,k;
	//draw line, where atoms are connected
	for(i=0;i<dim/2;i++){
		for(j=i+1;j<dim/2;j++){
			if(DIST(i,j)){
				//draw_line(screen,COORD(2*i)+x,COORD(2*i+1)+y,COORD(2*j)+x,COORD(2*j+1)+y,SDL_MapRGB(screen->format, 0,255,0));
			}
		}
	}
	//draw circles at atom positions
	for(i=0;i<dim/2;i++){
		draw_circle(screen,COORD(2*i)+x,COORD(2*i+1)+y,2,SDL_MapRGB(screen->format, 255,0,0));
	}
}

template<int dim, typename vectorType,typename evalType>
int DrawScene(int popSize, basicCandidate<dim,vectorType>** pop){
	//flush event queue
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT){
			DestroyGraphics();
			return 0;
		}
	}
	//clear screen
	wipe(screen, SDL_MapRGB(screen->format, 0, 0, 0));
	//draw grid
    
	draw_line(screen,160,0,160,480,SDL_MapRGB(screen->format, 255,255,255));
	draw_line(screen,320,0,320,480,SDL_MapRGB(screen->format, 255,255,255));
	draw_line(screen,480,0,480,480,SDL_MapRGB(screen->format, 255,255,255));
	draw_line(screen,0,160,640,160,SDL_MapRGB(screen->format, 255,255,255));
	draw_line(screen,0,320,640,320,SDL_MapRGB(screen->format, 255,255,255));

	//find scale
	vectorType max=0;
	int i,j,k;
	for(i=0;i<popSize;i++){
		for(j=0;j<dim;j++){
			if(abs(pop[i]->components[j])>max) max = abs(pop[i]->components[j]);
		}
	}
	double scale = ((double)CELL_WIDTH/2-5)/max;	//draw it a bit smaller

	//draw candidates to cells
	k=0;
	for(i=0;i<480;i+=CELL_WIDTH){
		for(j=0;j<640;j+=CELL_WIDTH){
			DrawCandidate<dim,vectorType,evalType>(j,i,pop[k],scale);
			k++;
		}
	}
	SDL_Flip(screen);
	return 1;
}

#endif