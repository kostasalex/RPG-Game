#include "item.h"
#include "spell.h"
#include "living.h"
#include "grid.h"

class Game{
  
  private:
        static const int maxHeroes = 3;

        int heroesNum;
        std::vector<Hero*> heroes;

        class Market *market;
        void shop(Hero *hero);

        class Combat *combat;

        Grid *grid;

        /* Different game state for each block type*/
        int gameState;
        enum state{peace, trading, fighting};
        void setGameState(int blockType);


        //!void combat(void);

        //!void menu(void);
        //!void display(void);
        //!void exit(void);


    public:
        Game();
        //~Game();

        void play(void);
        
};


class Market{

    private:
        std::string name;

        Weapon **weapons;

        Armor **armors;

        Spell **spells;
        
        int productCounter[3];
        
        void showItems(int type);

    public:
        enum type{weapon , armor, spell};
        static std::string productType[3];

        Market();
        ~Market();
        
        /* Returns -1 if no product selected */
        int selectProduct(int type); 

        bool buy(Item *item, int &money);

        Item* sell(int type, int id, int &money, int heroLvl);

        Spell* sell(int id, int &money, int heroLvl);

};


class Combat{
    
    private:
        int monstersNum;
        Monster **monsters;

        int heroesNum;
        Hero **heroes;

        int round;

        int lootMoney;

        int lootItemsNum;

        Item **lootItems;

        /* Fighting actions */
        void heroesTurn(void);

        void monstersTurn(void);
        int getRandTarget(void);

        void regeneration(void);

        bool isFightEnd(void);


        /* After fight actions*/
        //*void receiveRewards(void);

        //*void revive(void);




    public:

        Combat(Hero **heroes , int heroesNum);
        ~Combat();

        /* Fight ends when all heroes or all monster die */
        void fight(void);

};