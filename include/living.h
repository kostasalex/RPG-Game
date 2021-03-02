#ifndef LIVING_H
#define LIVING_H

#include "item.h"
#include "spell.h"

/* Base class for all livings in game,
   heroes and monsters               */
class Living{

    private: 
        static const int startingHp = 500;

        static const int startingLevel = 1;

        std::string name;
        int level;
        int hp, maxHp;      //Health Power
            
        virtual void regeneration(void) = 0;

        virtual void goUnconscious(void) = 0;

    protected:
        
        inline void addHp(int points) //hp shouldn't be greater than maxHp
        { hp = ((hp + points) > maxHp)? maxHp : (hp + points); }

        inline void addMaxHp(int points) { maxHp += points; }

        inline void subHp(int points) //hp shouldn't be less than 0
        { hp = ((hp - points) < 0)? 0 : (hp - points); }

        inline void addLevel(int levels) { level += levels; }

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

        /* Inline functions */
        inline std::string getName(void) const { return this->name; }

        inline int getLevel(void) const { return this->level; }

        inline int getMaxHp(void) const { return this->maxHp; }

        inline int getHp(void) const { return this->hp; }
};


/* Heroes and their classes */
class Hero : public Living{

    private: 
        static const int startingMp = 500;
        static const int startingExperience = 0;
        static const int startingMoney = 100;

        static const int additionalHp = 100;

        static const int maxLevel = 10;
        static const int maxExperience = 100; 

        static constexpr double regenRate = 0.05;
        
        static const int statsPerLevel = 2;
        static const int hpPerLevel = 100;
        static const int mpPerLevel = 50;

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

        Weapon *starterWeapon;
        Armor *starterArmor;

        static const int maxSpells = 10;
        SpellBook *spellBook;

        Inventory *inventory;

        enum stats{strength, dexterity, agility};

        static const std::string statsTypeMsg[3];
        
        static const int maxBuffs = 10;
        int buffCounter;
        struct Buff *buffs[maxBuffs];


        /* Combat  */
        bool usePotion(Potion *potion);
        //Improve stats per level
        void levelUp(void);
        //Adds regenRate * maxHp to hp
        void regeneration(void) override;
        //When hp == 0 hero will lose all active buffs
        void goUnconscious(void);

        void addBuff(struct Buff* buff);

        int findBuff(Buff *buff);


        //Equip weapon and armor
        bool equip(Item *item);

        bool inventoryAdd(Item *item);



        void removeBuff(int buffType);

        bool findSpell(Spell *spell);



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

        inline int getInventorySize(void) const
        {return this->inventory->getSize();}

    protected:
        static const int startingStr = 10;
        static const int startingDex = 10;
        static const int startingAgi = 10;

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

        int getDamage(void) const;

        int getDefence(void) const;

        /* Inline functions */     
        inline int getBaseStr(void) const { return this->base.str; }
        inline int getBaseDex(void) const { return this->base.dex; }   
        inline int getBaseAgi(void) const { return this->base.agi; }       

        inline int getStr(void) const { return this->current.str; }
        inline int getDex(void) const { return this->current.dex; }  
        inline int getAgi(void) const { return this->current.agi; }   

        inline int getExp(void)const{ return this->experience; }

        void checkSpells(void) const;

        inline int getMp(void) const { return this->mp; }
        inline int getMaxMp(void) const { return this->maxMp; } 

    public:
        /* Constructor - Destructor */
        Hero(std::string name);
        virtual ~Hero();


        /* Combat  */
        int attack(Living *living) const override;

        int receiveDamage(int damage);

        //Returns spell id, or -1 if no spells learned yet
        int selectSpell(void) const;

        //If target couldn't avoid spell, he will receive 
        //a damage and a debuff that will reduce his stats.
        bool castSpell(int spellId, Living *living);

        //For every experience = maxExperience, hero gains level
        void receiveExperience(int exp);


        void pickUp(int money);
        //Same with pickUp money with diffrent print msg
        void receiveMoney(int money);

        //Decrease buff duration, regenerates hp/mp
        void roundPass(void) override;

        //Restore 50% of an unconscious hero's hp.
        //With get penalty on, hero will lost -50% money.
        void revive(bool getPenalty = true);


        //Displays all items equipped and unused. 
        //Can equip weapon/armor or use a potion with the proper arg
        //Returns true if an item equipped / or a potion is used
        bool checkInventory(bool equip = false, bool usePotion = false);
        
        //Additionally displays active buffs and learned spells
        virtual void displayStats(void) const = 0;
        

        /* Trade */
        //In order to complete a purchase hero should have the required
        //money and level.
        void buy(Item *item);
        //If purchase was successfull, hero will automaticaly
        //learn the spell.
        void buy(Spell *spell);
        
        void sell(Spell *&spell);

        void sell(Item *&item);

        bool learnSpell(Spell *spell);
};



class Warrior : public Hero{
    private:
        static const int additionalStr = 5;
        static const int additionalAgi = 5;

    public:
        /* Constructor - Destructor */
        Warrior(std::string name);
        ~Warrior();

        void displayStats(void)  const override;

        static int getStrength(void){return (additionalStr + startingStr);}
        static int getDexterity(void){return startingDex;}
        static int getAgility(void){return (additionalAgi + startingAgi);}
};


class Sorcerer : public Hero{

    private:
        static const int additionalDex = 5;
        static const int additionalAgi = 5;

    public:
        /* Constructor - Destructor */
        Sorcerer(std::string name);
        ~Sorcerer();

        void displayStats(void) const override;

        static int getStrength(void){return startingStr;}
        static int getDexterity(void){return (additionalDex + startingDex);}
        static int getAgility(void){return (additionalAgi + startingAgi);}
};


class Paladin : public Hero{
    private:
        static const int additionalStr = 5;
        static const int additionalDex = 5;

    public:
        /* Constructor - Destructor */
        Paladin(std::string name);
        ~Paladin();

        void displayStats(void) const override;

        static int getStrength(void){return (additionalStr + startingStr);}
        static int getDexterity(void){return (additionalDex + startingDex);}
        static int getAgility(void){return startingAgi;}
};


/* Monsters and their types */
class Monster : public Living{

    private:
        static const int startingDef = 10;
        static const int startingDodge = 10;
        
        static const int startingDmg = 50;
        static const int damageRange = 50;
        
        static const int statsPerLevel = 2;
        static const int hpPerLevel = 100;
        static const int damagePerLevel = 30;

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
        
        //When hp == 0 monster will lose all active debuffs
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
            this->baseDmg.lb += damage;this->baseDmg.ub += damage;
        }
        inline void addDmg(int damage)
        {
            this->currentDmg.lb += damage;this->currentDmg.ub += damage;
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
        /* Returns the id of the selected hero */
        int selectTarget(Hero **heroes, int num);   

        int attack(Living *living) const override;

        int receiveDamage(int damage) override;

        void receiveDeBuff(struct Buff*);

        /* Decrease debuff duration, regenerates hp/mp */
        void roundPass(void) override;

};

class Dragon : public Monster{
        static const int additionalDmg = 20;
        
    public:

        Dragon(std::string name, int additionalLevel);
        ~Dragon();

        void displayStats(void) const override;
};

class Exoskeleton : public Monster{
        static const int additionalDef = 5;

    public:

        Exoskeleton(std::string name, int additionalLevel);
        ~Exoskeleton();

        void displayStats(void) const override;
};

class Spirit : public Monster{
        static const int additionalDodge = 5;

    public:

        Spirit(std::string name, int additionalLevel);
        ~Spirit();

        void displayStats(void) const override;
};

#endif