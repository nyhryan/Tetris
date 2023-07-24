#pragma once
#include "Block.h"
#include "Position.h"
#include "Color.h"

class OBlock : public Block
{
public:
    OBlock()
    {
        mId = 4;
        mBlockColor = Color::Colors[mId];
        
        mCells[0] = {
            Position(0, 1),
            Position(0, 2),
            Position(1, 1),
            Position(1, 2)
        };

        // moves block spawn position to the center
        Move(0, 3);
    }
};