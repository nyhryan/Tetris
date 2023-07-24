#pragma once
#include "Block.h"
#include "Position.h"
#include "Color.h"

class IBlock : public Block
{
public:
    IBlock()
    {
        mId = 1;
        mBlockColor = Color::Colors[mId];

        // rotation state 0's block cell coods
        mCells[0] = {
            Position(1, 0),
            Position(1, 1),
            Position(1, 2),
            Position(1, 3)
        };

        mCells[1] = {
            Position(0, 2),
            Position(1, 2),
            Position(2, 2),
            Position(3, 2)
        };
        mCells[2] = {
            Position(2, 0),
            Position(2, 1),
            Position(2, 2),
            Position(2, 3)
        };
        mCells[3] = {
            Position(0, 1),
            Position(1, 1),
            Position(2, 1),
            Position(3, 1)
        };

        // https://tetris.wiki/Super_Rotation_System - Wall kick data
        // data converted to (x, y) -> Position(-y, x)
        mWallKickData[0] = { // 0 -> R : CW
            Position(0, 0),
            Position(0, -2),
            Position(0, 1),
            Position(1, -2),
            Position(-2, 1)
        };

        mWallKickData[1] = { // R -> 0 : CCW
            Position(0, 0),
            Position(0, 2),
            Position(0, -1),
            Position(-1, 2),
            Position(2, -1)
        };

        mWallKickData[2] = {
            // R -> 2 : CW
            Position(0, 0),
            Position(0, -1),
            Position(0, 2),
            Position(-2, -1),
            Position(1, 2),
        };

        mWallKickData[3] = { // 2 -> R : CCW
            Position(0, 0),
            Position(0, 1),
            Position(0, -2),
            Position(2, 1),
            Position(-1, -2)
        };

        mWallKickData[4] = { // 2 -> L : CW
            Position(0, 0),
            Position(0, 2),
            Position(0, -1),
            Position(-1, 2),
            Position(2, -1)
        };

        mWallKickData[5] = { // L -> 2 : CCW
            Position(0, 0),
            Position(0, -2),
            Position(0, 1),
            Position(1, -2),
            Position(-2, 1)
        };

        mWallKickData[6] = { // L -> 0 : CW
            Position(0, 0),
            Position(0, 1),
            Position(0, -2),
            Position(2, 1),
            Position(-1, -2)
        };

        mWallKickData[7] = { // 0 -> L : CCW
            Position(0, 0),
            Position(0, -1),
            Position(0, 2),
            Position(-2, -1),
            Position(1, 2)
        };


        // moves block spawn position to the center
        Move(-1, 3);
    }
};
