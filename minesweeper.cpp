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

int main()
{
    
}