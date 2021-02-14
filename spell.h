#ifndef SPELL_H
#define SPELL_H

#include <cstring>
#include <iostream>

class Spell{

    private:
        std::string name;
        int requiredLevel;
        int mpCost;                   //Magical power needed
        int pointsAffect;
        int price;
        struct dmg{ int lb, ub; }dmg;   //Damage range 
        static int ub_damage;           //Added to lb_damage
        
        /*struct TargetStat{
            int stat, points;
        }targetStat;
    protected:
        inline int debuff(int &stat){ }*/

    public:
        enum statAffect{damage, defence, dodge};
        static std::string statMsg[3];

        Spell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);

        virtual ~Spell() = 0;

        void virtual cast(int dexterity, int &targetStat,\
         int &points,  int &damage) = 0;

        void virtual print(void) = 0;


        inline int getMp(void) const {return this->mpCost;}

        inline int getLvl(void) const {return this->requiredLevel;}

        inline std::string getName(void) const {return this->name;}

        inline int getLbDmg(void){return this->dmg.lb;}

        inline int getUbDmg(void){return this->dmg.ub;}

        inline int getPoints(void){return this->pointsAffect;}

        inline int getPrice(void){return this->price;}
        
        inline int getDamage(int dexterity)
        {return ((dmg.lb + dexterity) > dmg.ub)? dmg.ub : (dmg.lb + dexterity);}

};



class IceSpell : public Spell{

    public:
        IceSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);

        ~IceSpell();

        void cast(int dexterity, int &targetStat, \
         int &points,  int &damage) override;

        void print(void) override;



};



class FireSpell : public Spell{

    public:
        FireSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);
        
        ~FireSpell();

        void cast(int dexterity, int &targetStat,\
         int &points,  int &damage)override;

        void print(void) override;


};



class LightingSpell : public Spell{

    public:
        LightingSpell(std::string name, int lb_damage, int pointsAffect, \
        int price, int requiredLevel, int mpCost);

        ~LightingSpell();

        void cast(int dexterity, int &targetStat,\
         int &points,  int &damage) override;

        void print(void) override;


};
#endif