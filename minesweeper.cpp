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
    
    int get_adjacentmines() // Details in the adjacent mines function. Basic Purpose is to keep the track of how many mines are associated with a function
    {
        return adjacentmines; 
    }

    void set_adjacentmines(int mines) // Responsible for increasing the count of the mines to make sure we remember how many mines are associated with the function
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
    static const int ROWS = 5; // Fixed number of ROWS - Change the rows, columns, and the mines in ratios to ensure the prpoer functionality of the game. 
    static const int COLUMNS = 5; // Fixed number of the COLUMNS
    static const int TOTALMINES = 7; // The total number of mines that will be placed on the board.
    tile grid[ROWS][COLUMNS];
    RenderWindow window; // SFML keyword to make a window on which we can display our graphics - setting the properties according to the rows, and the columns 
    bool gameover;

public:
    board() : window(VideoMode(250,250), "Minesweeper") // Length, and Width of the window can be changed from here. Just increase it by the ratios as you increase the rows, and columns.
    {
        gameover = false; // Setting this condtion to false in the constructor to make sure that the program runs atleast once
        initialize(); // Places the mines on the board, uses a dynamic array to store the positions of the vectors. Uses the rand function to randomly place the rows, and columns on the board.
        putadjacentmines();
    }

    void reset() // Very convinient to reset the game using this, otherwise all the compilation needs to be done again. This function sets all the proeperties to the initial condition.
    {
        gameover = false;
        initialize(); 
        putadjacentmines();
    }

    void initialize() 
    {
        srand(time(0));
        vector<int> mine_positions; // Dynamic array of integers that will store the positions of the mines on board
        while (mine_positions.size() < TOTALMINES) // Making sure that everything is not a mine. :)
        {
            int pos = rand() % (ROWS * COLUMNS); // Creating a random number. The % determines that the number stays in the bounds
            if (find(mine_positions.begin(), mine_positions.end(), pos) == mine_positions.end()) // find function looks for the pos in the mine_positions, and if not found returns mine_positions.end(). Then the condition will be true, it will randomly place a mine.
            {
                mine_positions.push_back(pos); // push_back adds the new position to the vector
                int x = pos / COLUMNS; // calculates the row index
                int y = pos % COLUMNS; // calculates the column index 
                grid[x][y].place_mine(); // sets the mine to the (x,y) coordinate and calls the place_mine func that returns the ismine variable to true. Everytime it runs, it's going to mark change the status of one mine as true.
            }
        }
    }

    void putadjacentmines() // Function to make sure that each number has the exact number of mines associated with it - what we call the adjacent mines. It is one of the features of the minesweeper game
    {
        for (int x = 0; x < ROWS; x++) // This runs through all the rows of the board 
        {
            for (int y = 0; y < COLUMNS; y++) // This runs through the columns of every row 
            {
                if (!grid[x][y].get_ismine()) // Very important, this checks for if there is a mine at the tile. Basically how this works is this calls the function at the location x, and y, and returns the status of the tile at the function. If it is false(means), there is no mine then it runs, otherwise it skips over the tile, as we need to calculate the number of the adjacent mines.
                {
                    for (int dx = -1; dx <= 1; dx++) // Runs for three times(because each tile has two tiles, and for dx=0 means the tile itself), this is for the x-ccordinate of the surrounding tiles
                    {
                        for (int dy = -1; dy <= 1; dy++) // Same logic as for the rows, but in terms of the columns
                        {
                            if (x + dx >= 0 && x + dx < ROWS && y + dy >= 0 && y + dy < COLUMNS) // Very important(checks for the lower, and the higher bounds) 
                            {
                                if (!(dx == 0 && dy == 0)) // Skips the current tile, along which we are running the check 
                                {
                                    if (grid[x + dx][y + dy].get_ismine()) // this checks if there is a mine around the tile(the address of the surrounding mines y+dy basically tells you about the sath wali mine) 
                                    {
                                        grid[x][y].set_adjacentmines(grid[x][y].get_adjacentmines() + 1); // If there is a mine, then it updates the adjacent mines around the tile. How it does that is calls the get_adjacent mines function which returns the adjacent mines which is set to 0, adds one to it, and then it calls the set_adjacent mines which updates the value of the adjacent mines. Does it for all the columns, and the rows as the loops run. This is how we find out how many mines are associated with each tile
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    void display_grid() 
    {
        Texture coveredTexture, flagTexture, bombTexture;
        coveredTexture.loadFromFile("MINESWEEPER_X.png");
        flagTexture.loadFromFile("MINESWEEPER_F.png");
        bombTexture.loadFromFile("MINESWEEPER_M.png");

        Texture uncoveredTexture[9];
        for (int x = 0; x < 9; x++) 
        {
            uncoveredTexture[x].loadFromFile("MINESWEEPER_" + to_string(x) + ".png");
        }

        Font font;
        if (!font.loadFromFile("Roboto-Black.ttf")) 
        {
            cout << "Download Roboto - Black font please." << endl;
        }

        while (window.isOpen()) 
        {
            Event this_event;
            while (window.pollEvent(this_event)) 
            {
                if (this_event.type == Event::Closed) 
                {
                    window.close();
                }

                if (this_event.type == Event::MouseButtonPressed && !gameover) 
                {
                    int x_pos = this_event.mouseButton.x / 50;
                    int y_pos = this_event.mouseButton.y / 50;

                    if (x_pos >= 0 && x_pos < ROWS && y_pos >= 0 && y_pos < COLUMNS) 
                    {
                        if (this_event.mouseButton.button == Mouse::Left) 
                        {
                            if (!grid[x_pos][y_pos].get_isflagged()) 
                            {
                                uncover(x_pos, y_pos);
                                if (grid[x_pos][y_pos].get_ismine()) 
                                {
                                    gameover = true;
                                }
                            }
                        } 
                        else if (this_event.mouseButton.button == Mouse::Right)
                        {
                            grid[x_pos][y_pos].toggle_flag();
                        }
                    }
                }

                if (this_event.type == Event::KeyPressed && gameover) {
                    if (this_event.key.code == Keyboard::Y) {
                        reset();
                    } else if (this_event.key.code == Keyboard::N) {
                        window.close();
                    }
                }
            }

            window.clear(Color::Black);

            for (int x = 0; x < ROWS; x++) 
            {
                for (int y = 0; y < COLUMNS; y++) 
                {
                    int x_pos = x * 50;
                    int y_pos = y * 50;
                    Sprite tileSprite;

                    if (grid[x][y].get_iscovered())
                    {
                        tileSprite.setTexture(coveredTexture);
                        tileSprite.setScale(50.0f / tileSprite.getLocalBounds().width, 50.0f / tileSprite.getLocalBounds().height);
                        tileSprite.setPosition(x_pos, y_pos);
                        window.draw(tileSprite);

                        if (grid[x][y].get_isflagged()) 
                        {
                            tileSprite.setTexture(flagTexture);
                            tileSprite.setScale(50.0f / tileSprite.getLocalBounds().width, 50.0f / tileSprite.getLocalBounds().height);
                            tileSprite.setPosition(x_pos, y_pos);
                            window.draw(tileSprite);
                        }
                    } 
                    else 
                    {
                        if (grid[x][y].get_ismine()) 
                        {
                            tileSprite.setTexture(bombTexture);
                            tileSprite.setScale(50.0f / tileSprite.getLocalBounds().width, 50.0f / tileSprite.getLocalBounds().height);
                            tileSprite.setPosition(x_pos, y_pos);
                            window.draw(tileSprite);
                        } 
                        else 
                        {
                            tileSprite.setTexture(uncoveredTexture[grid[x][y].get_adjacentmines()]);
                            tileSprite.setScale(50.0f / tileSprite.getLocalBounds().width, 50.0f / tileSprite.getLocalBounds().height);
                            tileSprite.setPosition(x_pos, y_pos);
                            window.draw(tileSprite);
                        }
                    }
                }
            }

            if (gameover) // 
            {
                Text gameOverText("Game Over!", font, 48);
                gameOverText.setCharacterSize(36);
                gameOverText.setFillColor(Color::Red);
                gameOverText.setPosition((COLUMNS * 50) / 2 - 100, (ROWS * 50) / 2 - 50);
                window.draw(gameOverText);

                Text retryText("Do you want to retry? (y/n)", font, 24);
                retryText.setPosition((COLUMNS * 50) / 2 - 100, (ROWS * 50) / 2 + 50);
                window.draw(retryText);
            }

            window.display();
        }
    }

    void uncover(int row, int column) 
    {
        if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS || !grid[row][column].get_iscovered()) 
        {
            return;
        }

        grid[row][column].uncover_tile();
        
        if (grid[row][column].get_ismine()) 
        {
            return;
        }

        if (grid[row][column].get_adjacentmines() > 0) 
        {
            return;
        }

        for (int dx = -1; dx <= 1; dx++) 
        {
            for (int dy = -1; dy <= 1; dy++) 
            {
                if (dx != 0 || dy != 0) 
                {
                    uncover(row + dx, column + dy);
                }
            }
        }
    }
};

int main()
{
    board Board1; // Creates a object named Board1 with all the predefined properties in the board class; The code runs through till the put_adjacent mines function, and everything is setup;
    Board1.display_grid(); // the display grid function is called after the board has been setup setup after all the mines have been setup. Details of the function can be found infront of the function.
    return 0;
}