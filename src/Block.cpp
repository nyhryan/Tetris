#include <map>
#include <vector>
#include <algorithm>
#include <SDL.h>

#include "Block.h"
#include "Position.h"
#include "Color.h"

Block::Block()
    : mId(0),
      mRotationState(0),
      mBlockColor(0),
      mCellSize(32),
      mRowOffset(0),
      mColumnOffset(0)
{
}

void Block::Draw(SDL_Renderer* renderer, SDL_Texture* tex)
{
    const std::vector<Position>& tiles = GetCellPosition();

    SDL_Rect cell;
    cell.w = cell.h = mCellSize - 2;
    for (auto& item : tiles)
    {
        cell.x = item.col * mCellSize + 2;
        cell.y = item.row * mCellSize + 2;

        SDL_Rect srcRect{(mId - 1) % 4 * 32, (mId - 1) / 4 * 32, 32, 32};
        SDL_RenderCopy(renderer, tex, &srcRect, &cell);
    }
}

void Block::DrawNext(SDL_Renderer* renderer, SDL_Texture* tex, int offX, int offY)
{
    const std::vector<Position>& tiles = GetCellPosition();
    

    SDL_Rect cell;
    cell.w = cell.h = mCellSize - 2;
    for (auto& item : tiles)
    {
        if (mId == 1 || mId == 4)
            cell.x = (item.col - 3) * mCellSize - mCellSize / 2 + 2 + offX;
        else
            cell.x = (item.col - 3) * mCellSize + 2 + offX;

        if (mId == 1)
            cell.y = item.row * mCellSize + mCellSize / 2 + 2 + offY;
        else
            cell.y = item.row * mCellSize + 2 + offY;

        SDL_Rect srcRect{ (mId - 1) % 4 * 32, (mId - 1) / 4 * 32, 32, 32 };
        SDL_RenderCopy(renderer, tex, &srcRect, &cell);
    }
}


void Block::Move(int row, int col)
{
    mRowOffset += row;
    mColumnOffset += col;
}

void Block::RotateCW()
{
    mRotationState = (mRotationState + 1) % static_cast<int>(mCells.size());
}

void Block::RotateCCW()
{
    if (mRotationState == 0)
        mRotationState = mCells.size() - 1;
    else
        --mRotationState;
}

std::vector<Position>
Block::GetCellPosition()
{
    std::vector<Position> convertedPosition;

    for (const auto& item : mCells[mRotationState])
    {
        convertedPosition.emplace_back(
            item.row + mRowOffset,
            item.col + mColumnOffset);
    }

    return convertedPosition;
}