#include <cstring>
#include <iostream>


class Grid{
    
    private:
        int **grid;
        int height, width;
        int heroes;

        struct HeroesLocation{ int x,y; }heroesLoc;

    public:
        enum blockType{ common, market, nonAccessible };
        
        Grid();

        ~Grid();

        bool move(char direction);

        void displayMap(void) const;

        inline int getBlockType(void)
        { return this->grid[heroesLoc.x][heroesLoc.y] - heroes; }


};