#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "golcal.h"

void hello()
{
	printf("hello~\n");
}

void init_game(struct game *g, int row, int col)
{
	int i;
	bool *arr;
	//form the struct game g.
	g->row = row;
	g->col = col;
	g->rowCur = 0;
	g->colCur = 0;
	//initialize memory of g->grid
	g->grid = (bool**) malloc(sizeof(bool*) * row);
	//form each memory of grid's rows
	for (i=0; i<row; i++) {
		arr = (bool*) calloc(col, sizeof(bool));
		g->grid[i] = arr;
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
	bool *arr;
	//form the struct game g.
	g->rowCur = 0;
	g->colCur = 0;
	//initialize memory of g->grid
	g->grid = (bool**) malloc(sizeof(bool*));
	arr = (bool*) malloc(sizeof(bool));
	temp = g->grid;
	tmp = arr;
	//read in file
	while (1) {
		inChar = fscanf(fp, "%c", &c);
		if (inChar == EOF) {
			break;
		} 
		if (!colget) { //first row, count cols.
			col += 1;
			tmp = (bool*) realloc(arr, sizeof(bool)*col);
			if (tmp == NULL) {
				exit(1);
			}
			arr = tmp;
			if (c == '0') {
				arr[col-1] = false;
			} else if (c == '1') {
				arr[col-1] = true;
			} else {
				colget = true;
				row += 2;
				temp = (bool**) realloc(g->grid, sizeof(bool*)*row);
				if (temp == NULL) {
					exit(1);
				}
				g->grid = temp;
				g->grid[0] = arr;
				col -= 1;
				arr = (bool*) malloc(sizeof(bool)*col);
				g->grid[1] = arr;
			}
		} else if (c == '\n') { //rest of rows.
			row += 1;
			temp = (bool**) realloc(g->grid, sizeof(bool*)*row);
			if (temp == NULL) {
				exit(1);
			}
			g->grid = temp;
			arr = (bool*) malloc(sizeof(bool)*col);
			g->grid[row-1] = arr;
			i = 0;
		} else { //each chars.
			if (c == '0') {
				g->grid[row-1][i] = false;
			} else {
				g->grid[row-1][i] = true;
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

void draw(struct game *g, struct mouse *m)
{
	
}