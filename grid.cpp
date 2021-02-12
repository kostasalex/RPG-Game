#include <cstdlib>
#include "grid.h"

using namespace std;


Grid::Grid(){

    this->height = 3;
    this->width = 3;
    this->heroes = 3;
    
    heroesLoc = {0,0};

    //Creating default grid
    grid = new int*[3];
    for(int i = 0; i < this->height; i++)
        grid[i] = new int[3];

    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            grid[i][j] = common;
        }
    }

    grid[0][2] = grid[1][2] = grid[2][2] = nonAccessible;
    grid[1][1] = market;
    grid[0][0] += heroes;

    cout << "Creating grid " << this->height << " * " << this->width << endl;

}



Grid::~Grid(){

    cout << "A grid " << this->height << " * " << this->width 
         << " to be destroyed!!" << endl;


    for(int i = 0; i < this->height; i++){
        delete[] grid[i]; 
    }


        delete[] grid;

}



bool Grid::move(char direction){

    bool inGrid = false;
    int x, y;

    /* Check if the requested location is inside the grid */
    switch(direction){
        case 'w':
            if((heroesLoc.x - 1) >= 0){
                x = heroesLoc.x - 1; y = heroesLoc.y;
                inGrid = true;
            }
            break;

        case 's':
            if((heroesLoc.x + 1) < height ){
                x = heroesLoc.x + 1; y = heroesLoc.y;
                inGrid = true;
            }
            break;

        case 'd':
            if((heroesLoc.y + 1) < width ){
                x = heroesLoc.x; y = heroesLoc.y + 1;
                inGrid = true;
            }
            break;

        case 'a':
            if((heroesLoc.y - 1) >= 0){
                x = heroesLoc.x; y = heroesLoc.y - 1;
                inGrid = true;
            }
            break;
    }

    /* Check if new block is accessible */ 
    if(inGrid){  //If yes change heroes location
        if(grid[x][y] != nonAccessible){
            grid[heroesLoc.x][heroesLoc.y] -= heroes;
            grid[x][y] += heroes;
            heroesLoc.x = x; 
            heroesLoc.y = y;
            return true;
        }
    }

    return false;
}


//Todo: display hero location on grid
void Grid::displayMap(void) const{

    string block[5] = {"[__]","[_M]","[xx]", "[H_]", "[HM]"};

    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            cout << block[grid[i][j]];
        }
        cout << endl;
    }

    cout << endl;

}