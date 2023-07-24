#include <iostream>
#include <SDL.h>

#include "Grid.h"
#include "Color.h"

Grid::Grid(int rows, int cols, int cellSize)
    : mRows(rows),
      mCols(cols),
      mCellSize(cellSize),
      mGridAnimationTimer(0.0f)
{
    mGrid.resize(mRows);
    for (auto& col : mGrid)
        col.resize(mCols);
}

void Grid::Print()
{
    std::cout << '\n';
    for (auto& row : mGrid)
    {
        for (auto& col : row)
        {
            if (col == 0)
                std::cout << "  ";
            else
                std::cout << col << ' ';
        }
        std::cout << '\n';
    }
}

void Grid::Draw(SDL_Renderer* renderer, SDL_Texture* tex)
{
    SDL_Rect cell;
    cell.w = cell.h = mCellSize - 2;

    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mCols; ++c)
        {
            int cellId = mGrid[r][c];
            cell.x = c * mCellSize + 2;
            cell.y = r * mCellSize + 2;

            // blocks
            if (cellId != 0)
            {
                SDL_Rect srcRect{ (cellId - 1) % 4 * 32, (cellId - 1) / 4 * 32, 32, 32 };
                SDL_RenderCopy(renderer, tex, &srcRect, &cell);
            }
            // background grid
            else
            {
                Color::SetDrawColor(renderer, Color::Colors[cellId]);
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    mGridAnimationTimer = mCellSize / 2;
}

void Grid::DrawRowCleared(SDL_Renderer* renderer, SDL_Texture* tex)
{
    SDL_Rect cell;

    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mCols; ++c)
        {
            int cellId = mGridSnapshot[r][c];
            cell.w = cell.h = mCellSize - 2;
            cell.x = c * mCellSize + 2;
            cell.y = r * mCellSize + 2;

            if (cellId != 0 && cellId != 8)
            {
                SDL_Rect srcRect{ (cellId - 1) % 4 * 32, (cellId - 1) / 4 * 32, 32, 32 };
                SDL_RenderCopy(renderer, tex, &srcRect, &cell);
            }
            else
            {
                // draw black square on background first
                Color::SetDrawColor(renderer, Color::BLACK);
                SDL_RenderFillRect(renderer, &cell);


                // then draw shrinking white square
                if (cellId == 8)
                {
                    Color::SetDrawColor(renderer, Color::WHITE);
                    cell.w = cell.h = mGridAnimationTimer;
                    cell.x = c * mCellSize + (mCellSize - mGridAnimationTimer) / 2;
                    cell.y = r * mCellSize + (mCellSize - mGridAnimationTimer) / 2;
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
    }
    if (mGridAnimationTimer > 0)
        mGridAnimationTimer--;
}


int Grid::ClearFullRows()
{
    mGridSnapshot.clear();
    mGridSnapshot = mGrid;

    mClearedRows.clear();
    int clear = 0;
    for (int row = mRows - 1; row >= 0; --row)
    {
        if (IsRowFull(row))
        {
            ClearRow(row);
            mClearedRows.push_back(row);
            ++clear;
        }
        else if (clear > 0)
            MoveRowsDown(row, clear);
    }

    for (auto& row : mClearedRows)
    {
        for (auto& col : mGridSnapshot[row])
            col = 8;
    }

    return clear;
}

bool Grid::IsRowFull(int row)
{
    for (int col = 0; col < mCols; ++col)
    {
        if (mGrid[row][col] == 0)
            return false;
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for (int col = 0; col < mCols; ++col)
        mGrid[row][col] = 0;
}

void Grid::MoveRowsDown(int row, int downAmount)
{
    for (int col = 0; col < mCols; ++col)
    {
        mGrid[row + downAmount][col] = mGrid[row][col];
        mGrid[row][col] = 0;
    }
}
