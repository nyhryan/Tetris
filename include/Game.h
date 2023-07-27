#pragma once
#include <cstdint>
//#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include "Block.h"

struct GameInput
{
    bool mIsUpPressed = false;
    bool mIsUpReleased = false;
    
    bool mIsDownPressed = false;
    bool mIsDownReleased = false;

    bool mIsLeftPressed = false;
    bool mIsRightPressed = false;

    bool mIsCWReleased = false;
    bool mIsCCWReleased = false;

    bool mIsHoldReleased = false;
};

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
    void InitTetris();
    void UpdateScore();
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
    struct SDL_Window* mWindow = nullptr;
    int mWindowWidth = 0;
    int mWindowHeight = 0;

    struct SDL_Renderer* mRenderer = nullptr;

    TTF_Font* mFont = nullptr;
    int mFontSize = 24;
    struct SDL_Texture* mBlocksTexture = nullptr;

    bool mIsRunning = false;
    bool mIsGameOver = false;
    bool mShowResult = false;
    uint32_t mTicks = 0;
    float mDeltaTime = 0.0f;

    class Grid* mGrid = nullptr;

    std::vector<class Block> mBlocks = GetAllBlocks();
    class Block mCurrentBlock = GetRandomBlock();
    class Block mNextBlock = GetRandomBlock();
    class Block mHoldBlock = NullBlock();
 
    bool mIsBlockLockable = false;
    float mBlockLockTimer = 0.0f;

    bool mIsHoldBlockAvail = false;
    bool mIsBlockHoldTriggered = false;

    bool mReadyToPlay = false;

    GameInput mInput;


    bool mIsRowCleared = false;
    float mClearedRowTimer = 0.0f;

    float mFallingTimer = 0.0f;
    float mFallingSpeed = 0; // smaller the faster

    bool mShouldIncreaseLevel = false;


    int mTotalLinesRemoved = 0;
    int mCurrentLinesRemoved = 0;
    int mPrevScore = 0;
    int mScore = 0;
    struct SDL_Texture* mScoreText = nullptr;

    int mLevel = 1;

    int mFinalScore = 0;
    int mFinalLines = 0;
    int mFinalLevel = 0;
};