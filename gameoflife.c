#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "golcal.h"

int main(int argc, char *argv[])
{
	// the game & file pointer
	struct game g;
	struct mouse m;
	FILE *fp;
	// sdl vars.
	SDL_Window *window;
	SDL_Renderer *renderer;
	int winW = 400;
	int winH = 400;
	int extraW = 200;
	bool quit = false;
	SDL_Event e;
	TTF_Font *font;
	char fontPath[] = "assets/Ubuntu_Mono/UbuntuMono-Regular.ttf";
	// other vars
	bool play = false;
	int time;

	// initialize game, mouse.
	// init_game(&g, 10, 10);
	g.round = 0;
	g.wrap = true;
	hello();
	m.hover = true;
	m.press = false;
	m.click = false;
	// file input
	fp = fopen("input2.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "failed to load input file.\n");
		exit(1);
	}
	read_file(fp, &g);

	// sdl initialize
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Game of Life Player",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        winW+extraW, winH,
        0 // SDL_WINDOW_RESIZABLE
	);
	if (window == NULL) {
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);

	// sdl ttf initialize
	TTF_Init();
	font = TTF_OpenFont(fontPath, 20);

	// sdl main loop
	while (!quit)
	{
		// reset vars
		m.click = false;
		// sdl event handle
		while(SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				if (SDL_GetModState() & KMOD_CTRL) {
					switch (e.key.keysym.sym) {
						case SDLK_s:
							break;
					}
				} else {
					switch (e.key.keysym.sym) {
						case SDLK_p:
							play = !play;
							break;
						case SDLK_r:
							reset_game(&g);
							play = false;
							break;
						case SDLK_o:
							break;
						case SDLK_w:
							g.wrap = !g.wrap;
							break;
					}
				}
			} 
			// else if (e.type == SDL_WINDOWEVENT) {
			// 	if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			// 		SDL_GetWindowSize(window, &winW, &winH);
			// 		if (winW < winH) {
			// 			SDL_GetWindowSize(window, &winW, &winW);
			// 		} else {
			// 			SDL_GetWindowSize(window, &winH, &winH);
			// 		}
			// 	}
			// } 
			else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					m.hover = false;
					m.press = true;
					SDL_GetMouseState(&m.x, &m.y);
					printf("%d %d\n", m.x, m.y);
				} else if (e.type == SDL_MOUSEBUTTONUP) {
					m.hover = true;
					m.press = false;
					m.click = true;
				} else {
					if (m.hover) {
						SDL_GetMouseState(&m.x, &m.y);
					}
				}
			}
		}
		// clear & render texture
		SDL_RenderClear(renderer);
		time = clock();
		draw(&g, &m, winW, winH, extraW, play, font, renderer);
		SDL_RenderPresent(renderer);
		if (play) {
			next_gen(&g);
			while (clock() < time + 500);
		}
	}

	free_game(&g);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}