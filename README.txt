Title: Orc Run!
Programmer: Anthony J Mojica

Controls:
--Menus--
ENTER: Navigate Menus
Up Arrow: Up in the Menu
Down Arrow: Up in the Menu
Left Arrow: Decrease Sound, Options Menu
Right Arrow: Increase Sound, Options Menu
Escape: Navigate Menus & Pause

--Gameplay--
A: Move Left
D: Move Right
Left Mouse: Slash
Right Mouse Hold: Pull Bow
Release Right Mouse: Fire

Victory Condition: Beat previous best distance traveled
Lose Condition: Die before reaching previous best distance traveled

Orc with Sword: Shoot or Slash
Orc with Bow: Shoot or Slash

----------------------
Features:


Animations:

Walk
Trigger: Not Slashing, Not Firing, Not dead.
Entity: Hero

Sword Slash
Trigger: Left Click
Entity: Hero

Bow Fire
Trigger: Hold Right click to charge, Release to fire and finish Animation
Entity: Hero

Orc Sword Walk
Trigger: Not Slashing, Not dead
Entity: Orc With Sword

Orc Sword Slash
Trigger: Reaches the Hero
Entity: Orc with Sword

Orc Bow Walk
Trigger: not Firing, not Dead
Entity: Orc with Bow

Orc Bow Fire
Trigger: Every few Seconds, Distance is based on where Hero is in relation
to the Bow Orc with some randomization to make them innaccurate.
If too close to the hero trajectory is completely changed to fire 
more directly.
Entity: Orc with Bow

Levels:
Default Level: main level
Includes: Clouds, Mountains, Trees, Bushes, Plants


Enemy Types:
Orc with Sword:
Movement: Quick
Melee Attack

Orc with Bow:
Movement: Slow, stopping to fire
Fires to attack, produces arrows for the player to pickup


Weapons:
Bow: Fires an Arrow in an arch depending how much you charge the shot
Requires: Arrows

Sword: Slashes in Melee Range
Requires: Stamina


Characters:
Main Character: Hero
Weapons: Sword/Bow
Stamins Starting/Max: 4
Health Starting/Max: 4


Intro Screen: 
Plays a Short Cinimatic that displays the title with Text Effects
As the Character Runs by
Only Plays once before the menu appears, stop with anykey.

Tally Screen:
Once Player dies, score is subtracted from the bottom and Tallied into
the End game Report based on time.

Scrolling Credits:
Credits Scroll by with all necessary requirements
Small Cinimatic is played to help credits not be boring
Credits repeat until Escape is hit

Slide Show:
The How to Play Screen has 3 slides to explain Gameplay and Controls

Attact Mode:
If nothing is hit for 10 seconds in the menu, the How to Play Screen Plays
going through all 3 slides and then the credits roll
Stops if any key is hit and then waits 10 seconds to play again

Text Effects:
In the intro Text appears 1 by 1 as the character runs by.

Progress Bar:
Health and Stamina is represented by orbs below the Character.
(not quite a bar, but similar effect)

Paralax Scrolling:
Background is displayed on a tiered system
Clouds in the back - Slowest
Then Mountains
Then Trees
Then Bushes and Plants 
Then Character Entities - Quickest

Weapon Cooldown:
Player must wait 1 second to Re fire the bow
The Bow below the character represents if the cooldown is done.
(arrow in the bow indicates ready to fire)

Charge Shot: 
Holding Right mouse Charges a shot. 
The Animation represents how charges the bow is and the bow sound played
is deeper depending on how charged it is. 

Stacked Based Game State Machine:
Game States are Stacked on top of each other in a push and pop system
Using a vector of Game States in Game.h
(the Game is loaded at most times without the character entites or scores)

