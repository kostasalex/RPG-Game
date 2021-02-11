#include <cstring>
#include <iostream>

/* Base class for all item int game: 
   Potions, armors, weapons and spells*/
class Item{

    private:
        std::string name;
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
        std::string stat;//Stat that will be affected
        int points;

    public:
        /* Constructor - Destructor */
        Potion(std::string name, std::string stat,\
        int price, int requiredLevel, int points);
        ~Potion();

        /* Override functions */
        void print() const override;


        /* Inline functions */
        inline std::string getStat(void) { return stat; }

        inline void inscrease(int &stat){ stat += points; }


};



class Weapon : public Item{

    private:
        std::string stat; //Stat that will be affected
        int damage;
        int handsRequired;  //One-handed:1 , Two-handed:2

    public:
        /* Constructor - Destructor */
        Weapon(std::string name, int damage, int price, int requiredLevel);
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