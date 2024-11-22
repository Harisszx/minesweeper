#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/* Please refer to the project documentation to fully understand the functionality(hope to provide it) of the program. 
The comments provide limited information(and are for the project partners), and may not be enough 
to make changes to the code, but can definitly help you provide an understanding of the project*/

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
    bool get_isflagged() // used for a check in the display grid, to make sure that the flagged tile can't be revealed
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

    void toggle_flag() // checks if the mine is covered or not. If it is then, flips the value of the isflagged varaible
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
        Texture coverpng, flagpng, bombpng; // Loading the necessary images into the memory, and alloting them a variable name - so that can be rendered easily during the process
        coverpng.loadFromFile("MINESWEEPER_X.png");
        flagpng.loadFromFile("MINESWEEPER_F.png"); // Took these images from a library on the internet(to add graphics to the game)
        bombpng.loadFromFile("MINESWEEPER_M.png");

        Texture uncoverpng[9]; // declared this as an array so we can easily create a different image on each section of the array
        for (int x = 0; x < 9; x++) 
        {
            uncoverpng[x].loadFromFile("MINESWEEPER_" + to_string(x) + ".png"); // Make sure you don't change this or the naming of the image files, that may lead to a lot of errors
        }

        Font font;
        if (!font.loadFromFile("Roboto-Black.ttf")) // Font has already been downloaded, but have set up a check in case the font file is missing 
        {
            cout << "Download Roboto - Black font please." << endl;
        }

        while (window.isOpen()) // Condition is placed to check if the window is open or close - the whole structure is based on how the user interacts with the game - the user clicks are manipulated in this section
        {
            Event this_event;
            while (window.pollEvent(this_event)) // Pulls the next event in the memory - the sequence in the which the user(you) have trigerred the events - breaks if there is no event
            {
                if (this_event.type == Event::Closed) // condition to check, if the user has clicked the close button(x)
                {
                    window.close();
                }

                if (this_event.type == Event::MouseButtonPressed && !gameover) // Processes the mosue buttons pressing, the only addition is we have entered a safety check which checks that gameover is false, added that in case as (!gameover)
                {
                    int x_pos = this_event.mouseButton.x / 50; // Converts the window click coordinates into the grid coordinates.
                    int y_pos = this_event.mouseButton.y / 50; 

                    if (x_pos >= 0 && x_pos < ROWS && y_pos >= 0 && y_pos < COLUMNS) // again checking if the click is in the bounds, before this the game had errors even if you clicked on top of the window, it would go out of bounds 
                    {
                        if (this_event.mouseButton.button == Mouse::Left) // to uncover the tile(left-click to uncover the tile)
                        {
                            if (!grid[x_pos][y_pos].get_isflagged()) // to check if the tile is flagged or not, it's important as flagged tiles can't be revealed 
                            {
                                uncover(x_pos, y_pos); // uncover the tiles using the uncover function
                                if (grid[x_pos][y_pos].get_ismine()) // checks the tile is a mine, using the get_ismine function in the tiles
                                {
                                    gameover = true; // if it is then returns the value of the gameover variable to the true, so this loop breaks on the next run
                                }
                            }
                        } 
                        else if (this_event.mouseButton.button == Mouse::Right) // right click is used to toggle the flag
                        {
                            grid[x_pos][y_pos].toggle_flag(); // calls the toggle_flag function which updates the value of the isflagged variable, which is important as the get_isflagged function returns the same value, which is used for the check to not reveal the flagged files
                        }
                    }
                }

                if (this_event.type == Event::KeyPressed && gameover)  // The new reset funtion, saves the hassle of recompiling again and again
                {
                    if (this_event.key.code == Keyboard::Y)
                    {
                        reset(); // as soon as Y is pressed the board is reintialized using the reset function, and this loop continues, but if N is pressed it closes the window - tried only switching the value of the gameover but the uncovered tiles remain there so it's very important to call the reset function
                    } else if (this_event.key.code == Keyboard::N) // Press N, and then the window closes
                    {
                        window.close();
                    }
                }
            }

            window.clear(Color::Black); // Resets the frame, before displaying the next frame. Added this because sometimes the game was having uncovered tiles all there before even it was dsiplayed

            for (int x = 0; x < ROWS; x++) // to display the images on the board - reminder these loops are inside the main while loop which is checking if the window is open
            {
                for (int y = 0; y < COLUMNS; y++) 
                {
                    int x_pos = x * 50; // converting the grid coordinates into the mouse click coordinates to make sure that the pictures are placed at the correct place
                    int y_pos = y * 50;
                    Sprite tilesdisplay;

                    if (grid[x][y].get_iscovered()) // if it is a covered tile, then it uses the cover png
                    {
                        tilesdisplay.setTexture(coverpng);
                        tilesdisplay.setScale(50.0f / tilesdisplay.getLocalBounds().width, 50.0f / tilesdisplay.getLocalBounds().height);
                        tilesdisplay.setPosition(x_pos, y_pos);
                        window.draw(tilesdisplay);

                        if (grid[x][y].get_isflagged())  // flagged tile then it uses a flag png
                        {
                            tilesdisplay.setTexture(flagpng);
                            tilesdisplay.setScale(50.0f / tilesdisplay.getLocalBounds().width, 50.0f / tilesdisplay.getLocalBounds().height);
                            tilesdisplay.setPosition(x_pos, y_pos);
                            window.draw(tilesdisplay);
                        }
                    } 
                    else // if it is uncovered, then checks if it a mine or a number
                    {
                        if (grid[x][y].get_ismine()) // if it is a mine, then it uses the bomb png   
                        {
                            tilesdisplay.setTexture(bombpng);
                            tilesdisplay.setScale(50.0f / tilesdisplay.getLocalBounds().width, 50.0f / tilesdisplay.getLocalBounds().height);
                            tilesdisplay.setPosition(x_pos, y_pos);
                            window.draw(tilesdisplay);
                        } 
                        else  // very important - as this needs to correspond to the number of mines associated with the tile, so it pulls the number of adjacent mines from the function using the get_adjacent mines, and gives it to the uncoverpng which is an array stores with all the number images, so it displays the corresponnding number accordingly
                        {
                            tilesdisplay.setTexture(uncoverpng[grid[x][y].get_adjacentmines()]);
                            tilesdisplay.setScale(50.0f / tilesdisplay.getLocalBounds().width, 50.0f / tilesdisplay.getLocalBounds().height);
                            tilesdisplay.setPosition(x_pos, y_pos);
                            window.draw(tilesdisplay);
                        }
                    }
                }
            }

            if (gameover) // this is placed here, and not inside the reset function is becasue windows.clear was not working on there, and the code was running into some problems 
            {
                Text gameovertext("Game Over!", font, 48);
                gameovertext.setCharacterSize(36);
                gameovertext.setFillColor(Color::Red);
                gameovertext.setPosition((COLUMNS * 50) / 2 - 100, (ROWS * 50) / 2 - 50);
                window.draw(gameovertext);

                Text retryText("Do you want to retry? (y/n)", font, 20);
                retryText.setPosition((COLUMNS * 50) / 2 - 120, (ROWS * 50) / 2 + 40);
                window.draw(retryText);
            }

            window.display(); // Necessary to display all the stuff to the user, took us lives to figure this thing out
        }
    }

    void uncover(int row, int column) // this function is called from inside the while(windowisOpen) loop - this is a recursive funtion which ensures that if there are no mines associated with a tile then we need to uncover till we find a mine so we use the same property of iterating over the rows, and columns - more detail in the PDF
    {
        if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS || !grid[row][column].get_iscovered()) // this is necesssary as it checks for the bounderies of the rows, and the columns, and also sees if the tile is already uncovered so returns the value.
        {
            return;
        }

        grid[row][column].uncover_tile(); // calls this fucntion in the tiles, and changes the status of the iscovered function
        
        if (grid[row][column].get_ismine())// checks if the tile is mine using the status of the get_ismine function 
        {
            return;
        }

        if (grid[row][column].get_adjacentmines() > 0) // if the function has some mines associated with it then, it only uncovers that tile, and leaves the system. 
        {
            return;
        }

        for (int dx = -1; dx <= 1; dx++) // iterates over the rows, and columns in the same way(that we did to check if there are any adjacent mines associated to a tile)
        {
            for (int dy = -1; dy <= 1; dy++) 
            {
                if (dx != 0 || dy != 0) 
                {
                    uncover(row + dx, column + dy); // this is a recursive calls to uncover the mines when the nearing tiles of the current tile aren't a mine, and there isn't any mine associated to them, uncovers till the mine is uncountered.
                }
            }
        }
    }
};

int main()
{
    board Board1; // Creates a object named Board1 with all the predefined properties in the board class; The code runs through till the put_adjacent mines function, and everything is setup;
    Board1.display_grid(); // the display grid function is called after the board has been setup - deals with all the functionalities of the minesweeper
    return 0; // finally this marks the end of the program - outcome - I got known as an SFML expert(psff I set-up the SFML using 5 tutorials) - should make a tutorial myself now!
}