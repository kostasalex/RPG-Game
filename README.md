# RPG-game

A simple, object-oriented, grid-based text RPG where the player can create up to three heroes, chosen from three different classes (warrior, sorcerer, paladin) to battle against a group of randomly spawning creatures. As the heroes navigate the grid, they will encounter and fight these creatures, earning experience and gold to purchase better items.


## Compiling and Running

* Navigate to the "RPG-Game" folder in your local copy of the repository.

* In the terminal, run the command "./make run". This will compile and run the program

* To remove the executable and object files run "./make clean".




## Game Demo

### Load screen
```
||                             ||
||       /|________________    ||
|| O|===|* >________________>  ||
||       \|                    ||
=================================

Select a class :

1. Warrior     || A Warrior class has additional strength and agility
               || Strength : 15
               || Dexterity : 10
               || Agility : 15

2. Sorcerer    || A Sorcerer class has additional dexterity and agility
               || Strength : 10
               || Dexterity : 15
               || Agility : 15

3. Paladin     || A Paladin class has additional strength and dexterity
               || Strength : 15
               || Dexterity : 15
               || Agility : 10

```

### Grid navigation
```
-----------------
|H  |   |   | ##|
-----------------
|   | ##| [M| ##|
-----------------
|   |   |   | ##|
-----------------
|   |   | ##| ##|
-----------------

Where do you want to go? [w] = up, [s] = down, [a] = left, [d] = right
Or 'b' to go back
Direction: 


### Combat 
===============================================================

                  ***    Round 1   ***

===================== Heroes turn =============================
///////////////////////////////////////////////////////////////

kostas [600/600]              ->Adalinda [397/500]
->alex [600/600]              Xiuhcoatl [500/500]

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

1. Attack
2. Cast Spell
3. Display stats

0. Go back

>1
alex attacks Adalinda..
Adalinda received 99 damage!

Press Enter to Continue
```
### Rewards
```
===============================================================

             +++++ Combat Rewards +++++

===============================================================

kostas received 40 experience!
Experience: 0 --> 40
kostas Picked up 24 gold!!!
------------------------------------------
alex received 41 experience!
Experience: 0 --> 41
alex Picked up 27 gold!!!
------------------------------------------

Press Enter to Continue

```
