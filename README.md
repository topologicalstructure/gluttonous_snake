# Gluttonous Snake Game

A course project, Gluttonous Snake  game implemented in C++, with the help of Easyx graphics library.This project is a high-level language programming course project, with C++ language to complete a simple snake game, graphics with the help of Easyx library.Key features include the classic Snake game mode as well as two new single-player and two multiplayer modes. In addition, the design of the game score record, search record, delete record function.

## 1.Main work

### 1.1.Definition and implementation of base classes

Two basic classes are defined in the game section. A block class is used to store the blocks that form a snake, which are successively connected to form a linked list. A snake class is used to store the whole snake. Functions such as printing, regenerating, judging, moving, adding, changing, and ui display of snakes are implemented through member functions of specific classes.

### 1.2.Implementation of games

The program defines a game() function to implement the game, by the parameter value c to determine the game mode. To achieve the title requirements of the entry version, advanced version, advanced version and expansion of the two-person cooperation version and two-person competition version of the game.

### 1.3.Menu and history

A menu() function is defined to print a menu, make a mouse selection by reading the mouse and return the result of the mouse selection. In this file, the highest score of the three game modes is stored with 12 bytes at first, and then the data of each game is stored in the file in turn. In addition, the function of adding, deleting and reading the record is realized through the function.

## 2.Algorithm implementation

### 2.1.Snake action

The game's map is stored in a two-dimensional array, with empty positions having a value of 0, walls and snake heads having a value of -1, tail having a value of -2, and various other foods and items having a value each. After obtaining the moving direction by reading the keyboard, the member function judge() of snake class is called first to obtain the corresponding value of the location of the snake head after moving in the map array. Then the result after the snake head moves is judged according to the value and the corresponding operation is carried out according to the result. If the snake hits the wall or the snake body after moving, the action is terminated. If it eats food, it calls the add() member function of the snake class to extend the snake and change the score. If the item is eaten after moving, the state changes; In other cases, the snake class's move() member function is called.
## 3.Innovation and expansion

### 3.1.Shape improvement

A data member representing the type and direction of the block is added to the block class. When printing the block, the corresponding shape is printed according to its value. The SDF anti-aliasing algorithm used in the graph clock is used to print the fan in it. Special attention should be paid to the formation of new turning blocks on the head and the disappearance of old turning blocks on the tail during the snake's movement and addition.

### 3.2.Snake state design

In the game, four different states are designed for the snake. Except for the initial ordinary state, the other states need to be obtained by eating the corresponding props, and after a certain number of steps, the snake in different states has different colors and effects.A data member representing the state of the snake and the number of remaining steps in the state is added to the snake class. The number of remaining steps in the state is reduced in each step of the snake's movement. The corresponding implementation mode is selected according to the current state of the snake in the process of additional points, movement, food generation and Sleep delay, and the whole snake is reprinted when the state changes.

### 3.3.Two-player mode

In the game design of the two-person mode, the realization of two people on the same computer respectively through the W, A, S, D keys and direction keys control the movement of their snakes. And design the cooperative version and the competitive version of two modes.A new read key mode is designed for the two-person mode. In a move, two parameters k1 and k2 are defined first, with initial values of zero, corresponding to two snakes respectively. Then, a read key is carried out to change the parameter k corresponding to the moving snake to 1, followed by a second read key. Finally move the unmoved snake forward one unit. The ui was also designed for two two-player games.
