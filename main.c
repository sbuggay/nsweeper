#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <ncurses.h>

struct board {
	char **tiles; //numbers mines blanks
	char **masks; //# ! ?
	char **flood;
	int sizex;
	int sizey;
	int mines;
};


void create_board(struct board *in);
int check_tile(struct board *in, int x, int y);

int main() {
	srand(time(NULL));

	struct board temp;
	temp.sizex = 40;
	temp.sizey = 14;
	temp.mines = 100;
	create_board(&temp);

	int cursorx = 0, cursory = 0;

	initscr();

	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);


	init_pair(10, COLOR_WHITE, COLOR_RED); //!
	init_pair(11, COLOR_WHITE, COLOR_YELLOW); //?
	while(1) {
		clear();
		int i, j;
		for(i = 0; i < temp.sizex; i++) {
			for(j = 0; j < temp.sizey; j++) {
				
				if(temp.tiles[i][j] > 48 && temp.tiles[i][j] < 58)
					attron(COLOR_PAIR(temp.tiles[i][j] - 48));
				mvaddch(j, i, temp.tiles[i][j]);
				attroff(COLOR_PAIR(1));
					attroff(COLOR_PAIR(2));
					attroff(COLOR_PAIR(3));
					attroff(COLOR_PAIR(10));
					attroff(COLOR_PAIR(11));

				if(temp.masks[i][j] != ' ') {
					if(temp.masks[i][j] == '!')
						attron(COLOR_PAIR(10));
					if(temp.masks[i][j] == '?')
						attron(COLOR_PAIR(11));
					mvaddch(j, i, temp.masks[i][j]);
					
					attroff(COLOR_PAIR(1));
					attroff(COLOR_PAIR(2));
					attroff(COLOR_PAIR(3));
					attroff(COLOR_PAIR(10));
					attroff(COLOR_PAIR(11));
				}	
			}
		}
		mvaddch(cursory, cursorx, 'X');
		mvaddch(23, 79, ' ');
		refresh();
		int ch = getch();
		if(ch == KEY_LEFT || ch == 'h')
		{
			cursorx--;
		}
		if(ch == KEY_RIGHT || ch == 'l')
		{
			cursorx++;
		}
		if(ch == KEY_UP || ch == 'k')
		{
			cursory--;
		}
		if(ch == KEY_DOWN || ch == 'j')
		{
			cursory++;
		}
		if(ch == 'y')
		{
			cursorx--;
			cursory--;
		}	
		if(ch == 'u')
		{
			cursorx++;
			cursory--;
		}
		if(ch == 'b')
		{
			cursorx--;
			cursory++;
		}
		if(ch == 'n')
		{
			cursorx++;
			cursory++;
		}	
		if(ch == 'q')
		{
			temp.masks[cursorx][cursory] = ' ';
			if(temp.tiles[cursorx][cursory] == '*');
			//lose mechanics
		}	
		if(ch == 'w')
		{
			if(temp.masks[cursorx][cursory] == '!')
				temp.masks[cursorx][cursory] = '#';
			else
				temp.masks[cursorx][cursory] = '!';
		}
		if(ch == 'e')
		{	
			if(temp.masks[cursorx][cursory] == '?')
				temp.masks[cursorx][cursory] = '#';
			else
				temp.masks[cursorx][cursory] = '?';
		}



		//count all flags and all tiles to see if winning move
	}
	
	getch();
	endwin();

	return 0;
}

void create_board(struct board *in) {
	int i, j;

	//allocate tiles
	in->tiles = malloc(in->sizex * sizeof(char *));
	for(i = 0; i < in->sizex; i++) {
		in->tiles[i] = malloc(in->sizey * sizeof(char));
	}

	in->masks = malloc(in->sizex * sizeof(char *));
	for(i = 0; i < in->sizex; i++) {
		in->masks[i] = malloc(in->sizey * sizeof(char));
	}

	//fill tiles
	for(i = 0; i < in->sizex; i++) {
		for(j = 0; j < in->sizey; j++) {
			in->tiles[i][j] = '.';
		}
	}

	for(i = 0; i < in->sizex; i++) {
		for(j = 0; j < in->sizey; j++) {
			in->masks[i][j] = '#';
		}
	}

	//fill mines
	int x, y;
	for(i = 0; i < in->mines; i++) {
		do {
			x = rand() % in->sizex;
			y = rand() % in->sizey;
		} while(in->tiles[x][y] == '*');
		in->tiles[x][y] = '*';
	}

	//set numbers
	int temp = 0;
	for(i = 0; i < in->sizex; i++) {
		for(j = 0; j < in->sizey; j++) {
			temp = 0;
			for(x = -1; x < 2; x++) {
				for(y = -1; y < 2; y++) {
					printf("tile : %c %d %d %d %d %d\n", in->tiles[i][j], i, j, x, y, temp);
					if(!(x == 0 && y == 0))
						temp += check_tile(in, i + x, j + y);
				}
			}
			if(in->tiles[i][j] != '*' && temp != 0)
				in->tiles[i][j] = 48 + temp;
		}
	}
	
}

int check_tile(struct board *in, int x, int y) {
	if(x > 0 && x < in->sizex)
		if(y > 0 && y < in->sizey)
			if(in->tiles[x][y] == '*')
				return 1;
			return 0;
		}