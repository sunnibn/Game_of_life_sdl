#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
	bool quit = false;
	SDL_Event e;

	// initialize
	// init_game(&g, 10, 10);
	hello();
	m.hover = true;
	m.press = false;
	m.click = false;
	// file input
	fp = fopen("input1.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "failed to load input file.\n");
		exit(1);
	}
	read_file(fp, &g);

	// sdl initialize
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"ASCII Art Editor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        winW, winH,
        SDL_WINDOW_RESIZABLE
	);
	if (window == NULL) {
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);

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
							break;
						case SDLK_r:
							break;
						case SDLK_o:
							break;
					}
				}
			} else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					m.hover = false;
					m.press = true;
					SDL_GetMouseState(&m.x, &m.y);
				} else if (e.type == SDL_MOUSEBUTTONUP) {
					m.hover = true;
					m.press = false;
					m.click = true;
				} else {
					if (m.hover) {
						SDL_GetMouseState(&m.x, &m.y);
					}
				}
				printf("%d %d\n", m.x, m.y);
			}
		}
		// clear & render texture
		SDL_RenderClear(renderer);
		draw(&g, &m);
		SDL_RenderPresent(renderer);
	}

	return 0;
}