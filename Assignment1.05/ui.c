#include "ui.h"

void init_ncurses()
{
	initscr();
	printw("Hello World !!!");
	refresh();
	getch();
	endwin();
}