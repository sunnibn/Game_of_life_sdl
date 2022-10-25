#include <stdbool.h>

struct game
{
	int row;
	int col;
	int rowCur;
	int colCur;
	bool **grid;
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
void draw(struct game *g, struct mouse *m);

// void next_gen();
// void prev_gen();