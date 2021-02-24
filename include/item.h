#ifndef ITEM_H
#define ITEM_H

#include <cstring>
#include <iostream>
#include <vector>


/* Base class for all item int game: 
   Potions, armors, weapons and spells*/

class Item{

    private:
        std::string name;
        int type;    
        int price;
        int requiredLevel; //Level needed in order to use


    public:
        /* Constructor - Destructor */
        Item(std::string name, int price, int requiredLevel);
        virtual ~Item() = 0;


        /* Virtual functions */
        virtual void print() const = 0;

        /* Inline functions */
        inline int getPrice(void) const { return this->price; }

        inline int getLevel(void) const { return this->requiredLevel; }

        inline std::string getName(void) const { return this->name; }       

};



class Potion : public Item{

    private:
        int stat;//Stat that will be affected
        int points;
        /* Buff duration before modifiers */
        static const int buffRounds = 5;
        int rounds; //Buff duration

        static const std::string buffNames[3];
        static const std::string statTypeMsg[3];
    public:
        enum statsType{strength, dexterity, agility};
        static const int potionTypes = 3;

        /* Constructor - Destructor */
        Potion(std::string name, int stat,\
        int price, int points, int requiredLevel);
        ~Potion();

        struct Buff* drink(void);

        /* Override functions */
        void print() const override;


        /* Inline functions */
        inline int getStat(void) { return this->stat; }

        inline int getPoints(void){return this->points;}


};



class Weapon : public Item{

    private:
        std::string stat; //Stat that will be affected
        int damage;
        int handsRequired;  //One-handed:1 , Two-handed:2

    public:
        /* Constructor - Destructor */
        enum hands{oneHanded, twoHanded};
        Weapon(std::string name, int damage, int price,\
         int requiredLevel, int handsRequired);
        ~Weapon();


        /* Override functions */
        void print() const override;


        /* Inline functions */
        inline int getDamage(void) const { return damage; }

        inline int getHandsRequired(void) const { return handsRequired; }


};



class Armor : public Item{

    private:
        std::string stat; //Stat that will be affected
        int defence;

    public:
        /* Constructor - Destructor */
        Armor(std::string name, int defence, int price, int requiredLevel);
        ~Armor();


        /* Override functions */
        void print() const override;


        /* Inline functions */
        inline int getDefence(void) const { return defence; }


};


class Inventory{

    private:
        //Equipped items
        Weapon *weapon;

        Armor *armor;
        int money;

        Item *handSlot[2];

        std::vector<Item*> items;

        void disarmWeapon(void);
        void disarmArmor(void);


    public:

        Inventory(Weapon *weaponToequip, Armor *armorToEquip, int money);
        ~Inventory();

        void print(void) const;

        /* Check if object is the valid type before use these functions */
        bool equipWeapon(Weapon *weapon);
        bool equipArmor(Armor *armor);
        
        Item* popItem(int inventorySlot);
        Item *getItem(int inventorySlot);
        int addItem(Item *item);

        inline int getSize(void) const {return this->items.size();}

        inline Weapon *getWeapon(void){return this->weapon;}
        inline Armor *getArmor(void){return this->armor;}
        inline void addMoney(int money){ this->money+= money; }
        inline void subMoney(int money)
        {this->money = ((this->money - money) < 0)? 0 : (this->money - money);}
        
        inline int getMoney(void){ return this->money; }

        
};

#endif