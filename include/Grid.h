#pragma once
#include <vector>


class Game;

class Grid
{
public:
    Grid(int rows = 20, int cols = 10, int cellSize = 32);
    friend Game;

public:
    int& operator()(int row, int col)
    {
        return mGrid[row][col];
    }

public:
    void Print();
    void Draw(struct SDL_Renderer* renderer, SDL_Texture* tex);
    void DrawRowCleared(struct SDL_Renderer* renderer, SDL_Texture* tex);



public:
    bool IsCellOutside(int row, int col)
    {
        bool result;

        result = !(row >= 0 && row < mRows && col >= 0 && col < mCols);
        
        return result;
    }

    bool IsCellEmpty(int row, int col)
    {
        return (mGrid[row][col] == 0);
    }

    int ClearFullRows();

private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowsDown(int row, int downAmount); // move specific row by downAmount times

private:
    int mRows;
    int mCols;
    int mCellSize;
    std::vector<std::vector<int>> mGrid;

    std::vector<int> mClearedRows;
    std::vector<std::vector<int>> mGridSnapshot;


    float mGridAnimationTimer;

};