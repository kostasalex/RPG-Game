#ifndef LIVING_H
#define LIVING_H

#include <cstring>
#include <iostream>
#include <utility> 
#include <vector>
#include "item.h"
#include "spell.h"

/* Base class for all livings in game,
   heroes and monsters               */
class Living{

    private: 
        static const int startingHp = 1000;

        static const int startingLevel = 1;

        std::string name;
        int level;
        int maxHp;      //Health Power
        int hp;
            
        virtual void regeneration(void) = 0;

        virtual void goUnconscious(void) = 0;

    protected:
        
        inline void addHp(int points) //hp shouldn't be greater than maxHp
        { this->hp = ((hp + points) > maxHp)? maxHp : (hp + points); }

        inline void addMaxHp(int points) { this->maxHp += points; }

        inline void subHp(int points) //hp shouldn't be less than 0
        { this->hp = ((hp - points) < 0)? 0 : (hp - points); }

        inline void addLevel(int levels) { this->level += levels; }

    public:
        /* Constructor - Destructor */
        Living(std::string name);
        virtual ~Living() = 0;


        /* Virtual functions*/
        virtual int attack(Living *living) const = 0; 

        virtual int receiveDamage(int damage) = 0;
        /* Descrease  round for buffs / debuffs */
        virtual void roundPass(void) = 0;

        virtual void displayStats(void) const = 0;
        //virtual int regeneration(void) = 0;


        /* Inline functions */
        inline std::string getName(void) const { return this->name; }

        inline int getLevel(void) const { return this->level; }

        inline int getMaxHp(void) const { return this->maxHp; }

        inline int getHp(void) const { return this->hp; }


};


/* Heroes and their classes */
class Hero : public Living{

    static const int startingExperience = 90;
    
    static const int startingMoney = 1000;

    static const int maxLevel = 10;

    static const int maxExperience = 100; 

    static const int startingMp = 500;

    static const int statsPerLevel = 2;

    static constexpr double regenRate = 0.05;
    
    static const int hpPerLevel = 100;

    static const int mpPerLevel = 50;


    private: 
        struct Stats 
        {
            int str; //Strength, adds damage in attacks
            int dex; //Dexterity, adds damage to a spell.
            int agi; //agility, chance to dodge an attack.
        }
        base,        //Stats without modifiers: such a potion.
        current;     //Modified stats. Usually temporary.

        int mp;      //Magic Power
        int maxMp;
        int experience;

        Inventory *inventory;

        enum stats{strength, dexterity, agility};

        static const std::string statsTypeMsg[3];

        std::vector<Spell*> spells;
        
        static const int maxBuffs = 10;
        int buffCounter;
        struct Buff *buffs[maxBuffs];



        /* Combat  */
        bool usePotion(Potion *potion);

        void levelUp(void);

        void regeneration(void) override;

        void goUnconscious(void);

        void addBuff(struct Buff* buff);

        int findBuff(Buff *buff);


        //Equip weapon and armor
        bool equip(Item *item);

        int inventoryAdd(Item *item);



        void removeBuff(int buffType);

        bool findSpell(Spell *spell);



        /* Get data info */
        inline int getMoney(void){ return this->inventory->getMoney(); }



        /* Data modifiers */
        inline void addMoney(int money){ inventory->addMoney(money); }

        inline void subMoney(int money){ inventory->subMoney(money); }

        inline void setExp(int newExp){ this->experience = newExp; }

        inline void subStats(int str, int dex, int agi){
          this->current.str -= str; 
          this->current.dex -= dex; 
          this->current.agi -= agi;
        }

        inline void setStats(int str, int dex, int agi){
          this->current.str = str; 
          this->current.dex = dex; 
          this->current.agi = agi;
        }

        inline void addMaxMp(int points) { this->maxMp += points; }

        inline void addMp(int points) //mp shouldn't be grater than maxHp
        {this->mp= ((this->mp + points) > maxMp)? maxMp : (this->mp + points);}

        inline void subMp(int points) //mp shouldn't be less than 0
        { this->mp = ((this->mp  - points) < 0)? 0 : (this->mp  - points); }

    protected:
        /* Data modifiers */
        inline void addStats(int str, int dex, int agi){ 
          this->current.str += str; 
          this->current.dex += dex; 
          this->current.agi += agi;
        }
        inline void addBaseStats(int str, int dex, int agi){ 
          this->base.str += str; 
          this->base.dex += dex; 
          this->base.agi += agi;
        }

        void printBuffs(void) const;

    public:
        static const int startingStr = 10;
        static const int startingDex = 10;
        static const int startingAgi = 10;

        /* Constructor - Destructor */
        Hero(std::string name);
        virtual ~Hero();


        /* Combat  */
        bool castSpell(int spellId, Living *living);

        int receiveDamage(int damage);

        void receiveExperience(int exp);
      
        void pickUp(Item *item);

        void pickUp(int money);
        //Descrease round of active buffs
        void roundPass(void) override;

        int attack(Living *living) const override;

        void revive(bool getPenalty = true);
        
        void checkSpells(void) const;



        //Returns true if an item equipped / or potions drinked
        bool checkInventory(bool equip = false, bool drinkPotion = false);

        virtual void displayStats(void) const = 0;



        /* Trade */
        void buy(Item *item);

        void buy(Spell *spell);

        Item *sell(int inventorySlot);

        bool learnSpell(Spell *spell);





        /* Inline functions */        
        inline int getBaseStr(void) const { return this->base.str; }
        inline int getBaseDex(void) const { return this->base.dex; }   
        inline int getBaseAgi(void) const { return this->base.agi; }       

        inline int getStr(void) const { return this->current.str; }
        inline int getDex(void) const { return this->current.dex; }  
        inline int getAgi(void) const { return this->current.agi; }   

        inline int getExp(void)const{ return this->experience; }

        inline int getInventorySize(void) const{return this->inventory->getSize();}

        inline int getSpellNum(void) const{return this->spells.size();}



        inline int getMp(void) const { return this->mp; }
        inline int getMaxMp(void) const { return this->maxMp; }     


};



class Warrior : public Hero{
    
    public:
        static const int additionalStr = 5;
        static const int additionalAgi = 5;
        Warrior(std::string name);
        ~Warrior();
        void displayStats(void)  const override;
};

class Sorcerer : public Hero{

    public:
        static const int additionalDex = 5;
        static const int additionalAgi = 5;
        Sorcerer(std::string name);
        ~Sorcerer();
        void displayStats(void) const override;
};

class Paladin : public Hero{

    public:
        static const int additionalStr = 5;
        static const int additionalDex = 5;
        Paladin(std::string name);
        ~Paladin();
        void displayStats(void) const override;
};


/* Monsters and their types */
class Monster : public Living{

    private:
        static const int startingDef = 10;
        static const int startingDodge = 10;
        static const int startingDmg = 50;
        static const int damageRange = 50;
        static const int statsPerLevel = 2;
        static const int damagePerLevel = 10;

        struct dmg{ //Damage range 
            int lb; //Lower bound
            int ub; //Upper bound
        }
        baseDmg,      //Stats without modifiers: such a debuff from spell.
        currentDmg;   //Modified damage. Usually temporary.
           
        struct Stats 
        {
            int defence; 
            int dodge; 
        }
        base,        
        current;     

        static const std::string deBuffTypeMsg[Spell::spellTypes];

        int findDeBuff(Buff *deBuff);

        void removeDeBuff(int deBuffSlot);

        void goUnconscious(void);

        static constexpr double regenRate = 0.03;
        void regeneration(void) override;

        static const int maxDeBuffs = 10;
        int deBuffCounter;
        struct Buff *deBuffs[maxDeBuffs];
    protected:

        void printDeBuffs(void) const;

        /* Inline functions */
        inline void addBaseDmg(int damage)
        {
            this->baseDmg.lb += damage;
            this->baseDmg.ub += damage;
        }
        inline void addDmg(int damage)
        {
            this->currentDmg.lb += damage;
            this->currentDmg.ub += damage;
        }

        inline void addStats(int defence, int dodge)
        { 
          this->current.defence += defence; 
          this->current.dodge += dodge; 
        }
        inline void addBaseStats(int defence, int dodge)
        { 
          this->base.defence += defence; 
          this->base.dodge += dodge; 
        }


        inline void subDmg(int points){
            currentDmg.lb -= points;
            currentDmg.ub -= points;
            if(currentDmg.lb < 0)currentDmg.lb = 0;
            if(currentDmg.ub < 0)currentDmg.ub = 0;
        }

        inline void subDef(int points)
        {current.defence = ((current.defence  - points) < 0)? 0 \
                           : (current.defence  - points); }

        inline void subDodge(int points)
        {current.dodge = ((current.dodge  - points) < 0)? 0 \
                           : (current.dodge  - points); }

        inline int getDmgLb(void) const{return currentDmg.lb;}

        inline int getDmgUb(void) const{return currentDmg.ub;}

        inline int getDef(void) const{return current.defence;}

        inline int getDodge(void) const{return current.dodge;}
    public:

        /* Constructor - Destructor */
        Monster(std::string name, int additionalLevel);
        virtual ~Monster();



        virtual void displayStats(void) const = 0;

        int attack(Living *living) const override;

        int receiveDamage(int damage) override;

        void receiveDeBuff(struct Buff*);

        /* Descrease round of active buffs */
        void roundPass(void) override;

        /*inline void getBaseDmg(int &lb, int &ub)
        { lb = this->currentDmg.lb; ub = this->currentDmg.ub; }
        inline void getDmg(int &lb, int &ub)
        { lb = this->currentDmg.lb; ub = this->currentDmg.ub; }*/
};

class Dragon : public Monster{

    public:
        static const int additionalDmg = 30;

        Dragon(std::string name, int additionalLevel);
        ~Dragon();

        void displayStats(void) const override;
};

class Exoskeleton : public Monster{

    public:
        static const int additionalDef = 10;

        Exoskeleton(std::string name, int additionalLevel);
        ~Exoskeleton();

        void displayStats(void) const override;
};

class Spirit : public Monster{

    public:
        static const int additionalDodge = 10;

        Spirit(std::string name, int additionalLevel);
        ~Spirit();

        void displayStats(void) const override;
};

#endif