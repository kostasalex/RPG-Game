#include <cstring>
#include <iostream>
#include "item.h"
#include "spell.h"
#include "living.h"

class Grid{
    
    private:

        const std::string gameLogo[7] = {
        "=================================",
        "||     Rpg game started!!      ||",
        "||                             ||",
        "||       /|________________    ||",
        "|| O|===|* >________________>  ||",
        "||       \\|                    ||",
        "================================="
        };

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

        void display(void) const;


        void printLogo(void);


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

        virtual bool access(void) = 0;

        inline void leave(void){
            heroes = nullptr;
            heroesNum = 0;
        }

        virtual void interactWith(Hero **heroes, int heroesNum) = 0;
        
        virtual std::string display(void) = 0;
        

};


class NonAccessive : public Block{

    public:
    
        NonAccessive(){}
        ~NonAccessive(){}

        bool access(void)override 
        {return false;}

        void interactWith(Hero **heroes, int heroesNum)override {return;}
        
        std::string display(void)override {return "test";}

};


class Market : public Block{

    private:
        enum type{weapon , armor, spell, potion};
        static std::string productType[4];

        const std::string logo[8] = 
        {
            "  _______________",
            "  |=============|",
            "  |___|Market|__|",
            "  |     ____    | ",
            "  |  [] | _| [] |",
            "  |     |  |    |",
            "  vvvvvvvvvvvvvvvv",
            "______________________"
        };

        std::string name;

        Weapon **weapons;

        Armor **armors;

        Spell **spells;

        Potion **potions;
        
        int productCounter[4] = {};
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

        void printLogo(void) const;
    public:
    
        Market();
        ~Market();

        bool access(void)override
        {
            return true;
        }
        void interactWith(Hero **heroes, int heroesNum)override;

        std::string display(void){return "test";}

};


class Common : public Block{
    
    private:
        class Combat *combat;

        std::vector <Item*> itemsDropped;

        inline bool isPeacefull(void){return rand()%3 != 0;}

    protected:

        void addItem(Item *item){itemsDropped.push_back(item);}

    public:

        Common();
        ~Common();

        bool access(void)override
        {
            return true;
        }
        
        void interactWith(Hero **heroes, int heroesNum);

        std::string display(void)override {return "test";
        }

};


class Combat : public Common{

    private:
        int monstersNum;
        Monster **monsters;

        int round;

        /* Combat actions */
        void heroesTurn(void);

        void monstersTurn(void);

        int getRandTarget(void);

        void endOfRound(void);

        enum resutl{ stillFighting, heroesWon, monstersWon};
        int result(void);


        /* After combat actions*/
        void receiveRewards(void);

        int gainMoney(int heroLvl);

        Item* gainItem(void);

        int gainExp(int heroLvl);

        void reviveHeroes(bool receivePenalty);

    public:

        Combat(Hero **heroes, int heroesNum);
        ~Combat();

        bool access(void)
        {
            return true;
        }
        
        /* Combat ends when all heroes or all monster die */
        void start(void);

};