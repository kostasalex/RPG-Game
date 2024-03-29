#include <cstring>
#include <iostream>
#include "item.h"
#include "spell.h"
#include "living.h"

class Grid{
    
    private:
        static const int logoSize = 7;
        const std::string gameLogo[logoSize] = {
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

        virtual bool access(Hero **heroes, int heroesNum) = 0;

        inline void leave(void){
            heroes = nullptr;
            heroesNum = 0;
        }

        virtual void interactWith(void) = 0;    

};


class NonAccessive : public Block{

    public:
    
        NonAccessive(){}
        ~NonAccessive(){}

        bool access(Hero **heroes, int heroesNum) override 
        {return false;}

        void interactWith(void)override {return;}

};


class Market : public Block{

    private:
        enum type{weapon , armor, spell, potion};
        static std::string productType[4];

        static const int logoSize = 8;
        const std::string logo[logoSize] = 
        {
            "  _______________",
            "  |=============|",
            "  |___|Market|__|",
            "  |     ____    | ",
            "  |  [] | _| [] |",
            "  |     |  |    |",
            "vvvvvvvvvvvvvvvvvvvvvv",
            "______________________\n"
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
        /* Market buying hero items/spells */
        template<typename T>
        int buy(T product);
        /* Market selling to hero items */
        Item* sell(int type, int id, int &money, int heroLvl);
        /* Market selling to hero spells */
        Spell* sell(int id, int &money, int heroLvl);

        void printLogo(void) const;

    public:
    
        Market();
        ~Market();

        bool access(Hero **heroes, int heroesNum)override;

        void interactWith(void)override;

};


class Common : public Block{
    
    private:
        class Combat *combat;

        inline bool isPeacefull(void){return rand()%3 != 0;}

    public:

        Common();
        ~Common();

        bool access(Hero **heroes, int heroesNum);
        
        void interactWith(void);

};


class Combat{

    private:
        int monstersNum, round;
        Monster **monsters;

        int heroesNum;
        Hero **heroes;

        const int xpRate = 40;

        const int goldRate =  20;

        static bool isStaticInit;

        /*Statistics */
        static int monsterKilled, goldGained, xpGained, heroesRevives;

    
        enum monsterTypes{dragon, exoskeleton, spirit};
        static std::vector <std::string> monsterNames[3];
        
        void initStatics();

        /* Combat actions */
        void heroesTurn(void);

        void monstersTurn(void);

        void endOfRound(void);

        enum resutl{ stillFighting, heroesWon, monstersWon};
        int result(void);

        void print(int lSelected, int rSelected, bool heroTurn = true);

        /* After combat actions*/
        void receiveRewards(void);

        int gainMoney(int heroLvl);

        Item* gainItem(void);

        int gainExp(int heroLvl);

        void reviveHeroes(bool receivePenalty);


    public:
        Combat(Hero **heroes, int heroesNum);
        ~Combat();
        
        /* Combat ends when all heroes or all monster die */
        void start(void);

        static void statistics(void);
};