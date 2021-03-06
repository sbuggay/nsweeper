//Devan Buggay
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <ncurses.h>

#define version "0.1.1"

///////////
//TO DO
//1. Class-ify the game
//2. Add in win mechanics
//3. Add in lose mechanics
//4. Add bound checking for player moves
//5. Add in color scheme options
//6. Add in control menu/explanation screen
////////////

struct board {
    char **tiles; //numbers mines blanks
    char **masks; //# ! ?
    int sizex;
    int sizey;
    int mines;
};

void create_board(struct board *in);
int check_tile(struct board *in, int x, int y);
void flood(struct board *in, int x, int y);

int main() {
    srand(time(NULL));
    struct board temp;

    int cursorx = 0, cursory = 0;
    char game = 1, playing = 1, menu = 1, won = 0;
    int ch;

    initscr();

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);


    init_pair(10, COLOR_WHITE, COLOR_RED); //!
    init_pair(11, COLOR_WHITE, COLOR_YELLOW); //?

    while(game) {

        while(playing) {
            while (menu) {
                clear();
                printw("nsweeper %s\n", version);
                printw("a) easy\n");
                printw("b) medium\n");
                printw("c) hard\n");
                printw("d) curstom\n");
                printw("e) exit\n");
                refresh();
                ch = getch();
                if(ch == 'a') {
                    temp.sizex = 20;
                    temp.sizey = 10;
                    temp.mines = 30;
                    create_board(&temp);
                    menu = 0;
                }
                if(ch == 'b') {
                    temp.sizex = 40;
                    temp.sizey = 20;
                    temp.mines = 60;
                    create_board(&temp);
                    menu = 0;
                }
                if(ch == 'c') {
                    temp.sizex = 60;
                    temp.sizey = 30;
                    temp.mines = 100;
                    create_board(&temp);
                    menu = 0;
                }
                if(ch == 'd') {

                }
                if(ch == 'e') {
                    menu = 0;
                    playing = 0;
                    endwin();
                    return 1;
                }
            }


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
            ch = getch();
            switch(ch)
            {
                case KEY_LEFT:
                case 'h':
                    cursorx--;
                    break;
                case KEY_RIGHT:
                case 'l':
                    cursorx++;
                    break;
                case KEY_UP:
                case 'k':
                    cursory--;
                    break;
                case KEY_DOWN:
                case 'j':
                    cursory++;
                    break;
                case 'y':
                    cursorx--;
                    cursory--;
                    break; 
                case 'u':
                    cursorx++;
                    cursory--;
                    break;
                case 'b':
                    cursorx--;
                    cursory++;
                    break;
                case 'n':
                    cursorx++;
                    cursory++;
                    break;
                case 'q':
                    flood(&temp, cursorx, cursory);
                    break;
                case 'w':
                    if(temp.masks[cursorx][cursory] == '!')
                        temp.masks[cursorx][cursory] = '#';
                    else
                        temp.masks[cursorx][cursory] = '!';
                    break;
                case 'e':
                    if(temp.masks[cursorx][cursory] == '?')
                        temp.masks[cursorx][cursory] = '#';
                    else
                        temp.masks[cursorx][cursory] = '?';
                    break;
            }

            //bound checking to make sure user does not go beyond bounds of the board
            if(cursorx<0) {
                cursorx=0;
            }
            else if(cursorx>=temp.sizex) {
                cursorx=temp.sizex-1;
            }
            if(cursory<0) {
                cursory=0;
            }
            else if(cursory>=temp.sizey) {
                cursory=temp.sizey-1;
            }



            //count all flags and all tiles to see if winning move
        }
    }
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
                    if(!(x == 0 && y == 0))
                        if(check_tile(in, i + x, j + y) == 1)
                            if(in->tiles[i + x][j + y] == '*')
                                temp++;
                }
            }
            if(in->tiles[i][j] != '*' && temp != 0)
                in->tiles[i][j] = 48 + temp;
        }
    }
}

int check_tile(struct board *in, int x, int y) {
    if(x >= 0 && x < in->sizex)
        if(y >= 0 && y < in->sizey)
            return 1;
    return 0;
}

void flood(struct board *in, int x, int y) {
    if(check_tile(in, x, y) == 0 || in->masks[x][y] == ' ')
        return;
    if(in->tiles[x][y] != '.') {
        in->masks[x][y] = ' ';
        return;
    }
    in->masks[x][y] = ' ';
    flood(in, x - 1, y);
    flood(in, x + 1, y);
    flood(in, x, y - 1);
    flood(in, x, y + 1);
    flood(in, x - 1, y - 1);
    flood(in, x + 1, y - 1);
    flood(in, x - 1, y + 1);
    flood(in, x + 1, y + 1);
    return;
}
