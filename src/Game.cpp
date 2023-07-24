#define DEBUG

#include <random>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "Grid.h"
#include "Block.h"
#include "TextTexture.h"

#include "IBlock.h"  // block index 1
#include "JBlock.h"  // block index 2
#include "LBlock.h"  // block index 3
#include "OBlock.h"  // block index 4
#include "SBlock.h"  // block index 5
#include "ZBlock.h"  // block index 6
#include "TBlock.h"  // block index 7

Game::Game()
{
}

bool Game::Init(int wWidth, int wHeight)
{
    // ================================================================
    int initResult = SDL_Init(SDL_INIT_VIDEO);
    if (initResult)
    {
        SDL_Log("failed to init SDL : %s\n", SDL_GetError());
        return false;
    }

    // ================================================================
    mWindowWidth = wWidth;
    mWindowHeight = wHeight;

    mWindow = SDL_CreateWindow(
        "ATAI Tetris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        wWidth,
        wHeight,
        0);
    if (!mWindow)
    {
        SDL_Log("failed to create window : %s\n", SDL_GetError());
        return false;
    }

    // ================================================================
    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("failed to create renderer : %s\n", SDL_GetError());
        return false;
    }

    // load blocks.png
    // ================================================================
    IMG_Init(IMG_INIT_PNG);
    auto blocksSurf = IMG_Load("Assets/Blocks.png");
    if (!blocksSurf)
    {
        SDL_Log("failed to load Blocks.png : %s\n", IMG_GetError());
        return false;
    }
    mBlocksTexture = SDL_CreateTextureFromSurface(mRenderer, blocksSurf);
    SDL_FreeSurface(blocksSurf);

    // ================================================================
    auto ttfInit = TTF_Init();
    if (ttfInit)
    {
        SDL_Log("Failed to init ttf : %s\n", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("Assets/Fonts/ModernDOS8x16.ttf", 24);
    if (!mFont)
    {
        SDL_Log("failed to open font : %s\n", SDL_GetError());
        return false;
    }

    // ================================================================
    LoadData();

    mIsRunning = true;
    mTicks = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        if (!mIsGameOver)
        {
            ProcessInput();
            UpdateGame();
            GenerateOutput();
        }
        // if gameover, reset the game
        else
        {
            UnloadData();
            LoadData();
            mIsGameOver = false;
        }
    }
}

void Game::Shutdown()
{
    UnloadData();
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyTexture(mScoreText);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::LoadData()
{
    mGrid = new Grid();
    InitTetris();
}

void Game::UnloadData()
{
    delete mGrid;
}

void Game::ProcessInput()
{
    // mIsDownPressed = false;
    // mIsDownReleased = false;
    // mIsUpPressed = false;
    SDL_Event event;
    GameInput input;
    while (SDL_PollEvent(&event))
    {
        auto keyCode = event.key.keysym.sym;
        if (event.type == SDL_QUIT)
            mIsRunning = false;
        if (event.type == SDL_KEYDOWN && keyCode == SDLK_ESCAPE)
            mIsRunning = false;
        if (event.type == SDL_KEYUP && keyCode == SDLK_RETURN)
            mReadyToPlay = true;
        if (event.type == SDL_KEYDOWN && mReadyToPlay)
        {
            if (keyCode == SDLK_LEFT)
                input.mIsLeftPressed = true;
            if (keyCode == SDLK_RIGHT)
                input.mIsRightPressed = true;
            if (keyCode == SDLK_UP)
                input.mIsUpPressed = true;
            if (keyCode == SDLK_DOWN)
                input.mIsDownPressed = true;
        }
        if (event.type == SDL_KEYUP && mReadyToPlay)
        {
            if (keyCode == SDLK_DOWN)
                input.mIsDownReleased = true;
            if (keyCode == SDLK_x)
                input.mIsCWReleased = true;
            if (keyCode == SDLK_z)
                input.mIsCCWReleased = true;
        }
    }

    mInput = input;
}

void Game::UpdateGame()
{
    // update deltaTime
    // ================================================================

    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicks + 16))
        ;

    mDeltaTime = (SDL_GetTicks() - mTicks) / 1000.0f;

    if (mDeltaTime > 0.016)
        mDeltaTime = 0.016f;

    mTicks = SDL_GetTicks();

    // ================================================================
    if (mReadyToPlay)
    {
        // press down to fall faster
        mFallingTimer += mDeltaTime;

        if (mInput.mIsDownPressed && !mInput.mIsDownReleased)
        {
            auto fasterSpeed = 10.0f;
            std::swap(mFallingSpeed, fasterSpeed);
            MoveCurrentBlockDown();
            std::swap(mFallingSpeed, fasterSpeed);
        }
        // or automatically move down if time has passed enough
        // : falls down every mFallingSpeed frames
        else if (mFallingTimer > mDeltaTime * mFallingSpeed && !mInput.mIsDownPressed)
        {
            MoveCurrentBlockDown();
            mFallingTimer = 0.0f;
        }

        // if the block is ready to be lock to the grid, waits 10 frames before lock
        if (mIsBlockLockable && !mInput.mIsDownPressed)
        {
            mBlockLockTimer += mDeltaTime;

            if (mBlockLockTimer > mDeltaTime * 10.0f)
            {
                LockBlock();
                mBlockLockTimer = 0.0f;
                mIsBlockLockable = false;
                mFallingTimer = 0.0f;
            }
        }

        // press up to hard drop
        if (mInput.mIsUpPressed)
        {
            while (!IsBlockOutside(mCurrentBlock) && CanBlockFit(mCurrentBlock))
                mCurrentBlock.Move(1, 0);

            mCurrentBlock.Move(-1, 0);
            LockBlock();
        }

        if (mInput.mIsLeftPressed)
        {
            MoveCurrentBlockLeft();
        }

        if (mInput.mIsRightPressed)
        {
            MoveCurrentBlockRight();
        }

        if (mInput.mIsCWReleased)
        {
            RotateBlockCW();
        }

        if (mInput.mIsCCWReleased)
        {
            RotateBlockCCW();
        }

        // do disappearing animation during this timer
        if (mIsRowCleared)
        {
            mClearedRowTimer += mDeltaTime;
            if (mClearedRowTimer > mDeltaTime * 10.0f)  // animation for 10 frames?
            {
                mIsRowCleared = false;
                mClearedRowTimer = 0.0f;
            }
        }
    }
}

void Game::GenerateOutput()
{
    // erase buffer
    // ================================================================
    Color::SetDrawColor(mRenderer, Color::GRAY);
    SDL_RenderClear(mRenderer);
    // ================================================================

    // write score on the right
    int& scoreCounter = mPrevScore;
    if (scoreCounter < mScore)
    {
        auto scoreDiff = mScore - mPrevScore;
        if (scoreDiff <= 40)
            ++scoreCounter;
        else if (scoreDiff <= 100)
            scoreCounter += 5;
        else if (scoreDiff <= 300)
            scoreCounter += 10;
        else if (scoreDiff <= 1200)
            scoreCounter += 100;
    }
    // if the counter exceeds current score, make it current score
    if (scoreCounter >= mScore)
        scoreCounter = mScore;

    auto calcOffsetX = [this](TextTexture& tex)
    {
        int gridWidth = mGrid->mCols * mGrid->mCellSize;
        return static_cast<int>(gridWidth + (mWindowWidth - gridWidth - tex.texWidth) / 2);
    };

    TextTexture score = GetTextureFromText("Score - " + std::to_string(scoreCounter));
    int scoreAreaOffsetX = calcOffsetX(score);
    const int scoreOffsetY = 50;
    DrawTextTexture(score, scoreAreaOffsetX, scoreOffsetY);

    TextTexture totalLines = GetTextureFromText("Lines - " + std::to_string(mTotalLinesRemoved));
    int totalLinesAreaOffsetX = calcOffsetX(totalLines);
    DrawTextTexture(totalLines, totalLinesAreaOffsetX, scoreOffsetY + score.texHeight + 1);

    TextTexture level = GetTextureFromText("Level - " + std::to_string(mLevel));
    int levelAreaOffsetX = calcOffsetX(level);
    DrawTextTexture(level, levelAreaOffsetX, scoreOffsetY + score.texHeight * 2 + 1);
    // say good, amazing, ... when lines are cleared
    if (mCurrentLinesRemoved != 0)
    {
        std::string complimentText;
        switch (mCurrentLinesRemoved)
        {
            case 2:
                complimentText = "Great!";
                break;
            case 3:
                complimentText = "Amazing!!";
                break;
            case 4:
                complimentText = "TETRIS!!!";
                break;
            default:
                complimentText = "Good!";
                break;
        }

        TextTexture compliment = GetTextureFromText(complimentText);
        int lineAreaOffsetX = calcOffsetX(compliment);
        DrawTextTexture(compliment, lineAreaOffsetX, scoreOffsetY + score.texHeight * 3 + 1);
    }


    int gridWidth = mGrid->mCols * mGrid->mCellSize;
    const int nextBlockBoxWidth = (mWindowWidth - gridWidth) * 2 / 3;
    const int nextBlockBoxY = 150;
    int nextBlockAreaOffsetX = static_cast<int>(gridWidth + (mWindowWidth - gridWidth - nextBlockBoxWidth) / 2);
    SDL_Rect nextBlockSquare{ nextBlockAreaOffsetX, nextBlockBoxY, nextBlockBoxWidth, nextBlockBoxWidth };
    Color::SetDrawColor(mRenderer, Color::BLACK);
    SDL_RenderFillRect(mRenderer, &nextBlockSquare);

    mNextBlock.DrawNext(mRenderer, mBlocksTexture, nextBlockAreaOffsetX + (nextBlockBoxWidth - mNextBlock.mCellSize * 3) / 2, nextBlockBoxY + (nextBlockBoxWidth - mNextBlock.mCellSize * 2) / 2);

    // ================================================================
    // do disappearing animation while mIsRowCleared
    if (mIsRowCleared)
        mGrid->DrawRowCleared(mRenderer, mBlocksTexture);

    // ================================================================
    // or just normally draw everything
    else
    {
        // draw background grid first
        mGrid->Draw(mRenderer, mBlocksTexture);

        // then, draw shadow block if available
        auto shadow = GetShadowBlock();
        if (!shadow.empty())
        {
            SDL_Rect shadowCell;
            shadowCell.w = shadowCell.h = mGrid->mCellSize - 2;
            for (auto& item : shadow)
            {
                shadowCell.x = item.col * mGrid->mCellSize + 2;
                shadowCell.y = item.row * mGrid->mCellSize + 2;

                // draw transparent version of current block for the shadow
                SDL_Rect srcRect{ (mCurrentBlock.mId - 1) % 4 * 32, (mCurrentBlock.mId - 1) / 4 * 32, 32, 32 };
                SDL_SetTextureBlendMode(mBlocksTexture, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(mBlocksTexture, 127);
                SDL_RenderCopy(mRenderer, mBlocksTexture, &srcRect, &shadowCell);

                SDL_SetTextureAlphaMod(mBlocksTexture, 255);
                SDL_SetTextureBlendMode(mBlocksTexture, SDL_BLENDMODE_NONE);
            }
        }

        mCurrentBlock.Draw(mRenderer, mBlocksTexture);
    }


    // ================================================================
    SDL_RenderPresent(mRenderer);
}

void Game::InitTetris()
{
    mPrevScore = mScore = 0;
    mCurrentLinesRemoved = mTotalLinesRemoved = 0;
    mIsGameOver = false;
    mFallingSpeed = 50.0f;
}

void Game::UpdateScore()
{
    mIsRowCleared = true;
    mPrevScore = mScore;
    if (mCurrentLinesRemoved == 1)
        mScore += 40;
    else if (mCurrentLinesRemoved == 2)
        mScore += 100;
    else if (mCurrentLinesRemoved == 3)
        mScore += 300;
    else
        mScore += 1200;

    // gets faster every 2 lines
    if (mTotalLinesRemoved > 0 && mTotalLinesRemoved % 2 == 0)
    {
        mFallingSpeed -= 2.0f;
        if (mFallingSpeed < 4.0f)
            mFallingSpeed = 4.0f;
        mLevel++;
    }
}

TextTexture Game::GetTextureFromText(std::string text)
{
    SDL_Color White = { 255, 255, 255 };
    SDL_Surface* surf = TTF_RenderText_Solid(
        mFont,
        text.c_str(),
        White);
    auto tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    auto w = surf->w;
    auto h = surf->h;

    SDL_FreeSurface(surf);

    return TextTexture(tex, w, h);
}

void Game::DrawTextTexture(TextTexture& tex, int offX, int offY)
{
    SDL_Rect rect{
        offX,
        offY,
        tex.texWidth,
        tex.texHeight
    };
    SDL_RenderCopy(mRenderer, tex.tex, nullptr, &rect);
}

Block Game::GetRandomBlock()
{
    if (mBlocks.empty())
        mBlocks = GetAllBlocks();

    auto getRandomInt = [](int min, int max)
    {
        std::random_device rd;
        std::default_random_engine e1(rd());
        std::uniform_int_distribution<int> uniform_dist(min, max);
        return uniform_dist(e1);
    };

    int randomIndex = getRandomInt(0, mBlocks.size() - 1);
    Block randomBlock(mBlocks[randomIndex]);
    mBlocks.erase(mBlocks.begin() + randomIndex);

    return randomBlock;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {
        IBlock(),
        JBlock(),
        LBlock(),
        OBlock(),
        SBlock(),
        ZBlock(),
        TBlock()
    };
}

void Game::LockBlock()
{
    const auto tiles = mCurrentBlock.GetCellPosition();
    for (auto& item : tiles)
        (*mGrid)(item.row, item.col) = mCurrentBlock.mId;

    mCurrentBlock = mNextBlock;
    if (!CanBlockFit(mCurrentBlock))
    {
        mIsGameOver = true;
        mReadyToPlay = false;
    }
    mNextBlock = GetRandomBlock();

    mFallingTimer = 0.0f;

    mCurrentLinesRemoved = mGrid->ClearFullRows();
    mTotalLinesRemoved += mCurrentLinesRemoved;
    // if lines are removed
    if (mCurrentLinesRemoved != 0)
        UpdateScore();
    else
        mIsRowCleared = false;
}

std::vector<Position> Game::GetShadowBlock()
{
    Block temp(mCurrentBlock);

    while (!IsBlockOutside(temp) && CanBlockFit(temp))
        temp.Move(1, 0);

    temp.Move(-1, 0);
    return temp.GetCellPosition();
}

void Game::MoveCurrentBlockLeft()
{
    mCurrentBlock.Move(0, -1);
    if (IsBlockOutside(mCurrentBlock) || !CanBlockFit(mCurrentBlock))
        mCurrentBlock.Move(0, 1);
}

void Game::MoveCurrentBlockRight()
{
    mCurrentBlock.Move(0, 1);
    if (IsBlockOutside(mCurrentBlock) || !CanBlockFit(mCurrentBlock))
        mCurrentBlock.Move(0, -1);
}


void Game::MoveCurrentBlockDown()
{
    mCurrentBlock.Move(1, 0);

    if (IsBlockOutside(mCurrentBlock) || !CanBlockFit(mCurrentBlock))
    {
        mCurrentBlock.Move(-1, 0);
        mIsBlockLockable = true;
        // timer += mDeltaTime * mFallingSpeed;

        // SDL_Log("%lf", timer);

        // if (timer > mDeltaTime * 5.0f)
        //{
        //     SDL_Log("timer : %lf", timer);
        //     LockBlock();
        //     timer = 0.0f;
        // }
    }
}

void Game::RotateBlockCW()
{
    mCurrentBlock.RotateCW();
    // apply wall kick if the block went outside after rotation
    if (IsBlockOutside(mCurrentBlock) || !CanBlockFit(mCurrentBlock))
    {
        int rotationState = mCurrentBlock.mRotationState;
        // rotationState -> wallKickData[]
        // CW is 0, 2, 4, 6th row of the data table
        // 0 -> 0, 1 -> 2, 2 -> 4, 3 -> 6
        int wallKickDataIndex = 2 * rotationState;

        auto& data = mCurrentBlock.mWallKickData[wallKickDataIndex];
        for (const auto& pos : data)
        {
            mCurrentBlock.Move(pos.row, pos.col);
            if (!IsBlockOutside(mCurrentBlock) && CanBlockFit(mCurrentBlock))
                break;
            else
                mCurrentBlock.Move(-pos.row, -pos.col);
        }
    }
}

void Game::RotateBlockCCW()
{
    mCurrentBlock.RotateCCW();
    if (IsBlockOutside(mCurrentBlock) || !CanBlockFit(mCurrentBlock))
    {
        int rotationState = mCurrentBlock.mRotationState;
        // rotationState -> wallKickData[]
        // CCW is 1, 3, 5, 7th row of the data table
        // 0 -> 1, 1 -> 3, 2 -> 5, 3 -> 7
        int wallKickDataIndex = 2 * rotationState + 1;

        auto& data = mCurrentBlock.mWallKickData[wallKickDataIndex];

        for (auto& pos : data)
        {
            mCurrentBlock.Move(pos.row, pos.col);
            if (!IsBlockOutside(mCurrentBlock) && CanBlockFit(mCurrentBlock))
                break;
            else
                mCurrentBlock.Move(-pos.row, -pos.col);
        }
    }
}

// true if current block is outside the play area
bool Game::IsBlockOutside(Block& block)
{
    // coods of current block (offset applied)
    const auto tiles = block.GetCellPosition();
    for (auto& item : tiles)
    {
        if (mGrid->IsCellOutside(item.row, item.col))
            return true;
    }
    return false;
}

bool Game::CanBlockFit(Block& block)
{
    const auto tiles = block.GetCellPosition();
    for (auto& item : tiles)
    {
        // if any of position isn't empty, block can't fit = return false
        if (!mGrid->IsCellEmpty(item.row, item.col))
            return false;
    }
    return true;
}
