#include "item.h"
#include "spell.h"
#include "living.h"
#include "grid.h"

class Game{
  
  private:
        int heroesNum;
        std::vector<Hero*> heroes;

        int monstersNum;
        Monster **monsters;

        class Market *market;
        //std::vector<class Market*> markets;


        Grid *grid;

        int gameState;

        void combat(void);
        void shop(Hero *hero);
        //!void menu(void);
        //!void display(void);
        //!void exit(void);
        void setGameState(int blockType);

        enum state{peace, trading, fighting};
        inline int getGameState(void){ return this->gameState; }

    public:
        Game();
        //~Game();

        void play(void);
        
};


class Market{

    private:
        std::string name;

        //std::vector <Item*> items;

        Weapon **weapons;

        Armor **armors;

        Spell **spells;
        
        int itemsCounter[3];
        
        void showItems(int type);

    public:
        enum type{weapon , armor, spell};
        static std::string type[3];

        Market();
        ~Market();
        
        /* Returns -1 if nothing is selected */
        int selectItem(int type); 

        bool buy(Item *item, int &money);

        Item* sell(int type, int id, int &money, int heroLvl);

        Spell* sell(int id, int &money, int heroLvl);

};