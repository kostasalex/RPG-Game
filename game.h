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

        char direction[4] = {'w', 's', 'a', 'd'};
        
        bool move(void);
        
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

        Potion **potions;
        
        int productCounter[3];
        
        void showItems(int type);

    public:
        enum type{weapon , armor, spell, potion};
        static std::string productType[4];

        Market();
        ~Market();
        
        /* Returns -1 if no product selected */
        int selectProduct(int type); 

        int buy(Item *item);

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


        /* Fighting actions */
        void heroesTurn(void);

        void monstersTurn(void);

        int getRandTarget(void);

        void endOfRound(void);

        enum resutl{ stillFighting, heroesWon, monstersWon};
        int fightResult(void);


        /* After fight actions*/
        void receiveRewards(void);

        void receivePenalty(void);

        int gainMoney(int heroLvl);

        Item* gainItem(void);

        int gainExp(int heroLvl);

        void reviveHeroes(bool receivePenalty);




    public:

        Combat(Hero **heroes , int heroesNum);
        ~Combat();

        /* Fight ends when all heroes or all monster die */
        void fight(void);

};