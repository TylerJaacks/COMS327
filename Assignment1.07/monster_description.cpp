#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

#include <ncurses.h>

#include "monster_description.h"
#include "io.h"

void read_file()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    clear();

    std::string line;

    const char* env_p = std::getenv("HOME");

    std::string home = std::string(env_p);

    std::string directory = home + "/.rlg327/monster desc.txt";

    std::ifstream file(directory.c_str());

    while (std::getline(file, line)) 
    {
        if (line.compare("RLG327 MONSTER DESCRIPTION 1") == 0)
        {

        }

        else if (line.compare("BEGIN MONSTER") == 0)
        {

        }

        else if (line.compare(".") == 0)
        {

        }

        else if (line.compare("END") == 0)
        {

        }

        else
        {
            printw(line.c_str());
        }

        printw("\n");
    }

    refresh();
    getch();
    clear();
	endwin();
}