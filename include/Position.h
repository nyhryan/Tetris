#pragma once

struct Position
{
    Position(int row = 0, int col = 0)
        : row(row), col(col)
    {
    }
    int row;
    int col;
};