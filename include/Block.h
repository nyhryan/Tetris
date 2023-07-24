#pragma once

#include <map>
#include <vector>
#include <cstdint>

#include "Position.h"

class Game;

class Block
{
public:
    Block();
    friend Game;

public:
    void Draw(struct SDL_Renderer* renderer, SDL_Texture* tex);
    void DrawNext(struct SDL_Renderer* renderer, SDL_Texture* tex, int offX, int offY);
    void Move(int row, int col);
    void RotateCW();
    void RotateCCW();

public:
    std::vector<Position> GetCellPosition();  // returns offset applied positions of the block
    std::map<int, std::vector<Position>> mWallKickData;


protected:
    // contains cell coords of each rotatestate of tetrominos
    std::map<int, std::vector<Position>> mCells;

    int mId;
    int mRotationState;
    uint32_t mBlockColor;
    int mCellSize;

    int mRowOffset;
    int mColumnOffset;
};
