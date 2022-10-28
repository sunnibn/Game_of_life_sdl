#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "golcal.h"

void hello()
{
	printf("hello~\n");
}

void init_game(struct game *g, int row, int col)
{
	int i;
	bool *arr;
	bool *arr2;
	//form the struct game g.
	g->row = row;
	g->col = col;
	g->rowCur = 0;
	g->colCur = 0;
	//initialize memory of g->grid
	g->grid = (bool**) malloc(sizeof(bool*) * row);
	g->orig = (bool**) malloc(sizeof(bool*) * row);
	//form each memory of grid's rows
	for (i=0; i<row; i++) {
		arr = (bool*) calloc(col, sizeof(bool));
		arr2 = (bool*) calloc(col, sizeof(bool));
		g->grid[i] = arr;
		g->orig[i] = arr2;
	}
}

void read_file(FILE *fp, struct game *g)
{
	char c;
	int inChar;
	int i = 0;
	int j = 0;
	int row = 0;
	int col = 0;
	bool colget = false;
	bool *tmp;
	bool **temp;
	bool *tmp2;
	bool **temp2;
	bool *arr;
	bool *arr2;
	//form the struct game g.
	g->rowCur = 0;
	g->colCur = 0;
	//initialize memory of g->grid
	g->grid = (bool**) malloc(sizeof(bool*));
	g->orig = (bool**) malloc(sizeof(bool*));
	arr = (bool*) malloc(sizeof(bool));
	arr2 = (bool*) malloc(sizeof(bool));
	temp = g->grid;
	tmp = arr;
	temp2 = g->orig;
	tmp2 = arr2;
	//read in file
	while (1) {
		inChar = fscanf(fp, "%c", &c);
		if (inChar == EOF) {
			break;
		} 
		if (!colget) { //first row, count cols.
			col += 1;
			tmp = (bool*) realloc(arr, sizeof(bool)*col);
			tmp2 = (bool*) realloc(arr2, sizeof(bool)*col);
			if (tmp == NULL && tmp2 == NULL) {
				exit(1);
			}
			arr = tmp;
			arr2 = tmp2;
			if (c == '0') {
				arr[col-1] = false;
				arr2[col-1] = false;
			} else if (c == '1') {
				arr[col-1] = true;
				arr2[col-1] = true;
			} else {
				colget = true;
				row += 2;
				temp = (bool**) realloc(g->grid, sizeof(bool*)*row);
				temp2 = (bool**) realloc(g->orig, sizeof(bool*)*row);
				if (temp == NULL && temp2 == NULL) {
					exit(1);
				}
				g->grid = temp;
				g->grid[0] = arr;
				g->orig = temp2;
				g->orig[0] = arr2;
				col -= 1;
				arr = (bool*) malloc(sizeof(bool)*col);
				arr2 = (bool*) malloc(sizeof(bool)*col);
				g->grid[1] = arr;
				g->orig[1] = arr2;
			}
		} else if (c == '\n') { //rest of rows.
			row += 1;
			temp = (bool**) realloc(g->grid, sizeof(bool*)*row);
			temp2 = (bool**) realloc(g->orig, sizeof(bool*)*row);
			if (temp == NULL && temp2 == NULL) {
				exit(1);
			}
			g->grid = temp;
			g->orig = temp2;
			arr = (bool*) malloc(sizeof(bool)*col);
			arr2 = (bool*) malloc(sizeof(bool)*col);
			g->grid[row-1] = arr;
			g->orig[row-1] = arr2;
			i = 0;
		} else { //each chars.
			if (c == '0') {
				g->grid[row-1][i] = false;
				g->orig[row-1][i] = false;
			} else {
				g->grid[row-1][i] = true;
				g->orig[row-1][i] = true;
			}
			i++;
		}
	}
	printf("check %d %d \n", row, col);
	for (i=0; i<row; i++) {
		for (j=0; j<col; j++) {
			printf("%d", g->grid[i][j]);
		}
		printf("\n");
	}
	g->row = row;
	g->col = col;
}

void draw(struct game *g, struct mouse *m, int w, int h, int exw, bool play, TTF_Font *f, SDL_Renderer *renderer)
{
	// vars
	SDL_Texture *texture;
	SDL_Surface *surf;
	SDL_Texture *sideTexture;
	SDL_Surface *sideSurf;
	SDL_Surface *tempSurf;
	SDL_Color fontColor = {0,200,0,255};
	SDL_Rect rect;
	int i;
	int j;
	int ws = w/g->col;
	int hs = h/g->row;
	char numstr[10];
	// form surf
	surf = SDL_CreateRGBSurface(0, w,h, 32, 0,0,0,0);
	for (i=0; i<g->row; i++) {
		for (j=0; j<g->col; j++) {
			if (g->grid[i][j]) {
				rect.x = j*ws;
				rect.y = i*hs;
				rect.w = ws;
				rect.h = hs;
				SDL_FillRect(surf, &rect, SDL_MapRGBA(surf->format,255,255,255,255));
			}
		}
	}
	// form sideSurf
	//line
	sideSurf = SDL_CreateRGBSurface(0, exw,h, 32, 0,0,0,0);
	rect.x = 0, rect.y = 0, rect.w = 2, rect.h = h;
	SDL_FillRect(sideSurf, &rect, SDL_MapRGBA(sideSurf->format,0,200,0,255));
	//pixel
	tempSurf = TTF_RenderUTF8_Blended(f, "pixel: ", fontColor);
	rect.x = 2, rect.y = 0, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	sprintf(numstr, "%d", g->row);
	tempSurf = TTF_RenderUTF8_Blended(f, numstr, fontColor);
	rect.x += rect.w, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	tempSurf = TTF_RenderUTF8_Blended(f, " x ", fontColor);
	rect.x += rect.w, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	sprintf(numstr, "%d", g->col);
	tempSurf = TTF_RenderUTF8_Blended(f, numstr, fontColor);
	rect.x += rect.w, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	//round
	tempSurf = TTF_RenderUTF8_Blended(f, "round: ", fontColor);
	rect.x = 2, rect.y += rect.h, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	sprintf(numstr, "%d", g->round);
	tempSurf = TTF_RenderUTF8_Blended(f, numstr, fontColor);
	rect.x += rect.w, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	//play/pause
	if (play) {
		tempSurf = TTF_RenderUTF8_Blended(f, "[P] playing...", fontColor);
	} else {
		tempSurf = TTF_RenderUTF8_Blended(f, "[P] paused", fontColor);
	}
	rect.x = 2, rect.y += rect.h, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	//reset
	tempSurf = TTF_RenderUTF8_Blended(f, "[R] reset", fontColor);
	rect.x = 2, rect.y += rect.h, rect.w = tempSurf->w, rect.h = tempSurf->h;
	SDL_BlitSurface(tempSurf, NULL, sideSurf, &rect);
	SDL_FreeSurface(tempSurf);
	// into texture
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	rect.x = 0, rect.y = 0, rect.w = w, rect.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	sideTexture = SDL_CreateTextureFromSurface(renderer, sideSurf);
	rect.x = w, rect.y = 0, rect.w = exw, rect.h = h;
	SDL_RenderCopy(renderer, sideTexture, NULL, &rect);
	// free
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(sideSurf);
	SDL_DestroyTexture(sideTexture);
}

bool next_alive(struct game *g, int r, int c)
{
	int i;
	int j;
	int count = 0;
	for (i=r-1; i<r+2; i++) {
		if (i>=0 && i<g->row) {
			for (j=c-1; j<c+2; j++) {
				if (j>=0 && j<g->col) {
					if (i == r && j == c) {
						continue;
					}
					if (g->grid[i][j] == true) {
						count++;
					}
				}
			}
		}
	}
	if (g->grid[r][c]) {
		if (count == 2 || count == 3) {
			return true;
		}
	} else {
		if (count == 3) {
			return true;
		}
	}
	return false;
}

bool next_alive_wrapped(struct game *g, int r, int c)
{
	int i;
	int j;
	int wr;
	int wc;
	int count = 0;
	for (i=-1; i<2; i++) {
		wr = r+i;
		if (wr<0) {
			wr += g->row;
		} else if (wr>=g->row) {
			wr -= g->row;
		}
		for (j=-1; j<2; j++) {
			wc = c+j;
			if (wc<0) {
				wc += g->col;
			} else if (wc>=g->col) {
				wc -= g->col;
			}
			if (wr == r && wc == c) {
				continue;
			}
			if (g->grid[wr][wc] == true) {
				count++;
			}
		}
	}
	if (g->grid[r][c]) {
		if (count == 2 || count == 3) {
			return true;
		}
	} else {
		if (count == 3) {
			return true;
		}
	}
	return false;
}

void next_gen(struct game *g)
{
	int i;
	int j;
	bool tmpg[g->row][g->col];
	for (i=0; i<g->row; i++) {
		for (j=0; j<g->col; j++) {
			if (g->wrap) {
				tmpg[i][j] = next_alive_wrapped(g, i, j);
			} else {
				tmpg[i][j] = next_alive(g, i, j);
			}
		}
	}
	for (i=0; i<g->row; i++) {
		for (j=0; j<g->col; j++) {
			if (g->grid[i][j] != tmpg[i][j]) {
				g->grid[i][j] = tmpg[i][j];
			}
		}
	}
	g->round++;
}

void reset_game(struct game *g)
{
	int i;
	int j;
	g->round = 0;
	for (i=0; i<g->row; i++) {
		for (j=0; j<g->col; j++) {
			if (g->grid[i][j] != g->orig[i][j]) {
				g->grid[i][j] = g->orig[i][j];
			}
		}
	}
}

void free_game(struct game *g)
{
	int i;
	for (i=0; i<g->row; i++) {
		free(g->grid[i]);
	}
	free(g->grid);
}