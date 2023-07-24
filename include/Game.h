#pragma once
#include <cstdint>
#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include "Block.h"

class Game
{
public:
    Game();

public:
    bool Init(int wWidth = 600, int wHeight = 642);
    void RunLoop();
    void Shutdown();

public:
    void LoadData();
    void UnloadData();

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

private:
    struct TextTexture GetTextureFromText(std::string text);
    void DrawTextTexture(struct TextTexture& tex, int offX, int offY);

private:
    class Block GetRandomBlock();
    std::vector<class Block> GetAllBlocks();
    void LockBlock();
    std::vector<Position> GetShadowBlock();

private:
    void MoveCurrentBlockLeft();
    void MoveCurrentBlockRight();
    void MoveCurrentBlockDown();
    void RotateBlockCW();
    void RotateBlockCCW();

    bool IsBlockOutside(class Block& block);
    bool CanBlockFit(class Block& block);

private:
    SDL_Window* mWindow;
    int mWindowWidth;
    int mWindowHeight;

    SDL_Renderer* mRenderer;

    TTF_Font* mFont;
    SDL_Texture* mBlocksTexture;

    bool mIsRunning;
    bool mIsGameOver;
    uint32_t mTicks;
    float mDeltaTime;

    class Grid* mGrid;

    std::vector<class Block> mBlocks;
    class Block mCurrentBlock;
    class Block mNextBlock;

    bool mReadyToPlay;

    bool mIsUpPressed;
    bool mIsDownPressed;
    bool mIsLeftPressed;
    bool mIsRightPressed;
    bool mIsCWReleased;
    bool mIsCCWReleased;

    bool mIsDownReleased;

    bool mIsRowCleared;
    float mClearedRowTimer;

    float mFallingTimer;
    float mFallingSpeed; // smaller the faster

    bool mShouldIncreaseLevel = false;


    int mTotalLinesRemoved;
    int mCurrentLinesRemoved;
    int mPrevScore;
    int mScore;
    SDL_Texture* mScoreText;

    bool mIsLockable = false;
    float mLockTimer = 0.0f;
    float count = 0.0f;
    int mLevel = 1;
};