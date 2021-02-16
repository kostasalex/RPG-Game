#ifndef LIVING_H
#define LIVING_H

#include <cstring>
#include <iostream>
#include <utility> 
#include <vector>
#include "item.h"


/* Base class for all livings in game,
   heroes and monsters               */
class Living{

    private: 
        std::string name;
        int level;
        int maxHp;      //Health Power
        int hp;
    
    public:
        /* Constructor - Destructor */
        Living(std::string name);
        virtual ~Living() = 0;


        /* Virtual functions*/
        virtual void print(void) const = 0;

        virtual int attack(Living *living) const = 0; 

        virtual int receiveDamage(int damage) = 0;


        /* Inline functions */
        inline std::string getName(void) const { return this->name; }

        inline int getLevel(void) const { return this->level; }

        inline int getMaxHp(void) const { return this->maxHp; }

        inline int getHp(void) const { return this->hp; }

        inline void addHp(int points) //hp shouldn't be greater than maxHp
        { this->hp = ((hp + points) > maxHp)? maxHp : (hp + points); }

        inline void subHp(int points) //hp shouldn't be less than 0
        { this->hp = ((hp - points) < 0)? 0 : (hp - points); }

        inline void addMaxHp(int points) { this->maxHp += maxHp; }

        inline void addLevel(int levels) { this->level += levels; }

};


/* Heroes and their classes */
class Hero : public Living{

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

        std::vector<class Spell*> spells;

        void levelUp(void);
        inline void setExp(int newExp){ this->experience = newExp; }


    public:
        static const int startingStr = 10;
        static const int startingDex = 10;
        static const int startingAgi = 10;

        /* Return status when trying to equip item or learning a spell */
        enum equipLearn{succeed, notFound, higherLevel, wrongType};
        static std::string statusMsg[4];
        
        enum stats{strength, dexterity, agility};
        static std::string statsMsg[3];

        /* Constructor - Destructor */
        Hero(std::string name);
        virtual ~Hero() = 0;

        bool learnSpell(class Spell *spell);

        int castSpell(int spellId, Living *living);

        /* Override functions */
        int attack(Living *living) const override;

        int receiveDamage(int damage);

        void receiveExperience(int exp);
        
        void checkSpells(void) const;

        void checkInventory(void) const;

        int inventoryAdd(Item *item);

        int equip(int inventorySlot);
        int usePotion(int inventorySlot);


        void print(void) const override;

        /* Inline functions */        
        inline int getBaseStr(void){ return this->base.str; }
        inline int getBaseDex(void){ return this->base.dex; }   
        inline int getBaseAgi(void){ return this->base.agi; }       

        inline int getStr(void){ return this->current.str; }
        inline int getDex(void){ return this->current.dex; }  
        inline int getAgi(void){ return this->current.agi; }   

        inline int getExp(void){ return this->experience; }
        
        inline int getMoney(void){ return this->inventory->getMoney(); }

        inline void addMoney(int money){ inventory->addMoney(money); }

        inline void subMoney(int money){ inventory->subMoney(money); }
        
        inline Item *dropItem(int inventorySlot)
        {return inventory->popItem(inventorySlot);}

        inline int getMp(void){ return this->mp; }
        inline int getMaxMp(void){ return this->maxMp; }     

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

        inline void addMp(int points) //mp shouldn't be grater than maxHp
        {this->mp= ((this->mp + points) > maxMp)? maxMp : (this->mp + points);}


        inline void subStats(int str, int dex, int agi){
          this->current.str -= str; 
          this->current.dex -= dex; 
          this->current.agi -= agi;
        }
        inline void subMp(int points) //mp shouldn't be less than 0
        { this->mp = ((this->mp  - points) < 0)? 0 : (this->mp  - points); }



};

class Warrior : public Hero{

    public:

        static const int additionalStr = 5;
        static const int additionalAgi = 5;

        Warrior(std::string name);
        ~Warrior();
};

class Sorcerer : public Hero{

    public:
        /* Sorcerers have additional dexterity and agility */
        static const int additionalDex = 5;
        static const int additionalAgi = 5;
        Sorcerer(std::string name);
        ~Sorcerer();
};

class Paladin : public Hero{

    public:
        /* Paladins have additional strength and dexterity */
        static const int additionalStr = 5;
        static const int additionalDex = 5;
        Paladin(std::string name);
        ~Paladin();
};


/* Monsters and their types */
class Monster : public Living{

    private:

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

    public:
        static const int startingDef = 10;
        static const int startingDodge = 10;
        static const int startingDmg = 30;
        static const int damageRange = 50;
        /* Constructor - Destructor */
        Monster(std::string name);
        virtual ~Monster() = 0;

        
        /* Override functions */
        void print(void) const override;

        int attack(Living *living) const override;

        int receiveDamage(int damage) override;

        /* Inline functions */
        inline void addBaseDmg(int damage)
        {
            this->baseDmg.lb += damage;
            this->baseDmg.ub = this->baseDmg.lb + damageRange;
        }
        inline void addDmg(int damage)
        {
            this->currentDmg.lb += damage;
            this->currentDmg.ub += this->currentDmg.lb + damageRange;
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
        /*inline void getBaseDmg(int &lb, int &ub)
        { lb = this->currentDmg.lb; ub = this->currentDmg.ub; }
        inline void getDmg(int &lb, int &ub)
        { lb = this->currentDmg.lb; ub = this->currentDmg.ub; }*/
};

class Dragon : public Monster{

    public:
        static const int additionalDmg = 20;

        Dragon(std::string name);
        ~Dragon();

};

class Exoskeleton : public Monster{

    public:
        static const int additionalDef = 20;

        Exoskeleton(std::string name);
        ~Exoskeleton();
};

class Spirit : public Monster{

    public:
        static const int additionalDodge = 20;

        Spirit(std::string name);
        ~Spirit();
};

#endif