#include <cstring>
#include <iostream>
#include "item.h"
#include "spell.h"
#include "living.h"
#include "inputHandler.h"

class Grid{
    
    private:
        /* Blocks of grid */

        class Market **markets;
        int marketsNum;

        class NonAccessive *nonAccessive;

        class Common *common;

        class Block ***blocks;
        //Common, Market, NonAccessive
        const char blockTypes[3] = {' ', 'M', '#'};  


        /* Game grid */
        static const int defaultHeight = 4;
        static const int defaultWidth = 4;

        static const char defaultMap[defaultHeight][defaultWidth];


        //const char **selectedMap;

        int height;
        int width;


        /* Heroes */
        struct HeroesLoc{
            int x,y;
            void setLoc(int x, int y){this->x = x; this->y = y;} 
            const bool isSame(int x, int y) const
            {return (this->x == x && this->y == y);}
        }heroesLoc;
        static const int maxHeroes = 3;
        int heroesNum;
        Hero *heroes[maxHeroes];
        

        //Up 'w' , down 's', left 'a', right 'd', back 'b'
        static const char direction[5];

        bool move(void);

        void display(void);

        void displayMap(void) const;

    public:
        Grid();
        ~Grid();

        void play(void);

};

class Block{

    
    protected:

        Hero **heroes;
        int heroesNum;
        std::string **interface;

        inline void addHeroes(Hero **heroes, int number){
            this->heroes = heroes; heroesNum = number;
        }

        inline void setInterface(std::string **interface){
            this->interface = interface;
        }

        inline std::string **getInterface(void){return interface;}

    public:

        Block()
        {heroes = nullptr; interface = nullptr; heroesNum = 0;}
        
        virtual ~Block() = 0;

        virtual bool access(Hero **heroes, int heroesNum) = 0;

        inline void leaveBlock(void){
            heroes = nullptr;
            heroesNum = 0;
        }

        virtual void interactWith(void) = 0;
        
        virtual std::string display(void) = 0;
        
        virtual void menu(void) = 0;


};


class NonAccessive : public Block{

    public:
    
        NonAccessive(){}
        ~NonAccessive(){}

        bool access(Hero **heroes, int heroesNum)override 
        {return false;}

        void interactWith(void)override {return;}
        
        std::string display(void)override {return "test";}

        void menu(void)override {}
};


class Market : public Block{

    private:
        enum type{weapon , armor, spell, potion};
        static std::string productType[4];

        std::string name;

        Weapon **weapons;

        Armor **armors;

        Spell **spells;

        Potion **potions;
        
        int productCounter[3];
        /* Show the list of specific type of items */
        void showItems(int type);
        /* Returns -1 if no product selected */
        int selectProduct(int type); 
        /* Market buying hero items */
        int buy(Item *item);
        /* Market selling to hero items */
        Item* sell(int type, int id, int &money, int heroLvl);
        /* Market selling to hero spells */
        Spell* sell(int id, int &money, int heroLvl);


    public:
    
        Market();
        ~Market();

        bool access(Hero **heroes, int heroesNum)
        {
            addHeroes(heroes, heroesNum);
            return true;
        }
        
        void interactWith(void)override;
    
        std::string display(void){return "test";}

        void menu(void){}
};


class Common : public Block{
    
    private:
        int monstersNum;
        Monster **monsters;

        int round;

        /* Fight ends when all heroes or all monster die */
        void combat(void);

        void initCombat(void);

        inline bool isPeacefull(void){return rand()%3 != 0;}

        /* Combat actions */
        void heroesTurn(void);

        void monstersTurn(void);

        int getRandTarget(void);

        void endOfRound(void);

        enum resutl{ stillFighting, heroesWon, monstersWon};
        int combatResult(void);


        /* After combat actions*/
        void receiveRewards(void);

        int gainMoney(int heroLvl);

        Item* gainItem(void);

        int gainExp(int heroLvl);

        void reviveHeroes(bool receivePenalty);

    public:

        Common(){}
        ~Common(){}

        bool access(Hero **heroes, int heroesNum)
        {
            addHeroes(heroes, heroesNum);
            return true;
        }
        
        void interactWith(void);

        std::string display(void)override {return "test";
        }

        void menu(void)override {}
};
