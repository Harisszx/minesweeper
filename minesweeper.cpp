#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/* Please refer to the project documentation to fully understand the functionality(hope to provide it) of the program. 
The comments provide limited information(and are for the project partners), and may not be enough 
to make changes to the code.*/

class tile 
{
private:
    bool ismine;
    bool iscovered;
    bool isflagged;
    int adjacentmines;

public:
    tile() 
    {
        ismine = false; // setting it to false, because no mines are present on the board initially (using rand() to place the mines randomly on the board).
        iscovered = true; // setting it to true, because all tiles are covered at the start of the game.
        isflagged = false; // setting it to false, this will toggle on the right click.
        adjacentmines = 0; // this is will change based on the number that is appearing on the board(1 means 1 mine is attached to it)
    }

    bool get_ismine() 
    { 
        return ismine; 
    }
    bool get_iscovered()
    { 
        return iscovered; 
    }
    bool get_isflagged() 
    {
        return isflagged; 
    }
    
    int get_adjacentmines() 
    {
        return adjacentmines; 
    }

    void set_adjacentmines(int mines) 
    { 
        adjacentmines = mines; 
    }

    void place_mine() 
    {
        ismine = true; 
    }

    void uncover_tile() 
    { 
        iscovered = false; 
    }

    void toggle_flag() 
    {
        if (iscovered) 
        { 
            isflagged = !isflagged; 
        }
    }
};

class board
{
private:
    static const int ROWS = 5; 
    static const int COLUMNS = 5;
    static const int TOTALMINES = 7;
    tile grid[ROWS][COLUMNS];
    RenderWindow window; // SFML keyword to make a window on which we can display our graphics
    bool gameover;

public:
    board() : window(VideoMode(250,250), "Minesweeper") // Length, and Width of the window can be changed from here. Just increase it by the ratios as you increase the rows, and columns.
    {
        gameover = false; // Setting this condtion to false in the constructor to make sure that the program runs atleast once
    }

};

int main()
{
    board Board1; // Creates a object named Board1 with all the predefined properties in the board class;

}