#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

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
        ismine = false;
        iscovered = true;
        isflagged = false;
        adjacentmines = 0;
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
    board() : window(VideoMode(250,250), "Minesweeper") // Lentght, and Width of the window can be changed from here. Just increase it by the ratios as you increase the rows, and columns.
    {
        gameover = false; // Setting this condtion to false in the constructor to make sure that the program runs atleast once
    }

};

int main()
{
    board Board1;

}