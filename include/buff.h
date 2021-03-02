#ifndef BUFF_H
#define BUFF_H
/* Effect that modifies Monster and Hero stats for certain amount
   of rounds. Enabled after using a potion or spell.    */
struct Buff{
    private: 
        std::string name;
        int roundCounter;
        int stat;
        int points;
        
    public:
        Buff(std::string name_, int rounds, int stat_, int points_): 
        name(name_), roundCounter(rounds), stat(stat_), points(points_){}
        ~Buff(){}
        inline int roundPass(void){return --roundCounter;}
        inline int getStat(void){return stat;}
        inline std::string getName(void){return name;}
        inline int getPoints(void){return points;}
        inline int getRounds(void){return roundCounter;}
        inline bool isTheSame(Buff *buff){return buff->name == name;}
};
#endif