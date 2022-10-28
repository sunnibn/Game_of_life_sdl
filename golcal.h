#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct game
{
	int row;
	int col;
	int rowCur;
	int colCur;
	int round;
	int wrap;
	bool **grid;
	bool **orig;
};
struct mouse
{
	int x;
	int y;
	int x2;
	int y2;
	bool hover;
	bool press;
	bool click;
};

void hello();
void init_game(struct game *g, int r, int c);
void read_file(FILE *fp, struct game *g);
void draw(struct game *g, struct mouse *m, int w, int h, int exw, bool play, TTF_Font *f, SDL_Renderer *renderer);

void next_gen(struct game *g);
bool next_alive(struct game *g, int r, int c);
bool next_alive_wrapped(struct game *g, int r, int c);
// void prev_gen();
void reset_game(struct game *g);
void free_game(struct game *g);