# Tetris
SDL2 Tetris 🕹

Simple tetris project with SDL2 & C++.
I followed this video tutorial. [🎞 [Creating Tetris in C++ with raylib - Beginner Tutorial (OOP)]](https://youtu.be/wVYKG_ch4yM)

- Implemented basic features like hard drop, shadow blocks, wall kicks (but I am not sure if wall kick is properly working.)
- Difficulty increases by increasing tetromino's falling speed every 2 line clears.
- Scoring system follows [Original BPS scoring system](https://tetris.wiki/Scoring#Recent_guideline_compatible_games).

## Build
After building the project with VS, `/Assets` and SDL `.dll` files needs to be in a same directory where `.exe` file sits.

## Keys
- <kbd>Left Arrow</kbd>, <kbd>Right Arrow</kbd> : Move tetromino left, right
- <kbd>Up Arrow</kbd> : Hard drop
- Hold <kbd>Down Arrow</kbd> : Drop faster (but you can also drop it faster while tapping repetitively, which is unintended)
- <kbd>Enter</kbd> : Start game

## TODO
- Proper Combo system & scoring system
- Sometimes I-Block will clip out from the playing grid and makes game crash?
- Original tetris game has a feature that after tetromino has placed on its position, you can move the tetromino for a while. Need this feature.
- MUSIC 🎶
- Main menu, pause menu, etc.
- Bugs that I might not have noticed
- It is quite difficult to calculate anything related to frames and timings, which can be used on combo system, free to move around after placing the tetromino for a while, I need further understanding. 
