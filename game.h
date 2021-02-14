#include "item.h"
#include "spell.h"
#include "living.h"
#include "grid.h"

class Game{
  
  private:
        std::vector<Hero*> heroes;
        std::vector<class Market*> markets;
        Monster **monsters;
        Grid *grid;

        int gameState;

        void combat(void);
        void shop(void);
        void menu(void);
        void display(void);
        void exit(void);

    public:
        Game();
        ~Game();

        void play(void);
        
};


class Market{

    private:
        std::string name;

        //std::vector <Item*> items;

        Weapon **weapons;

        Armor **armors;

        Spell **spells;
        
        int weaponsNum, armorsNum, spellsNum;

    public:
        enum type{weapon , armor, spell};
        static std::string type[3];

        Market();
        ~Market();

        bool buy(Item *item, int &money);

        Item* sell(int type, int id, int &money, int heroLvl);

        Spell* sell(int id, int &money, int heroLvl);

};