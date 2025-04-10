#pragma once
#include "TextureManager.h"
#include "Tile.h"
#include "Counter.h"
#include "Random.h"
#include <vector>
#include <map>
#include <fstream>
#include <map>
#include <chrono>
#include <algorithm>
using namespace std;

class Board
{
private:
    int mineCount = 0;
    int flagCount = 0;
    vector<Tile> board;
    unsigned int width;
    unsigned int height;
    bool debug = false;
    bool pause = false;
    bool leaderboard = false;
    bool gameOver = false;
    bool victory = false;
public:
    Board();
    void readBoard();
    Tile& GetTile(unsigned int tile);
    unsigned int GetWidth();
    unsigned int GetHeight();
    unsigned int GetSize();
    bool GetGameOver();
    bool GetVictory();
    int GetFlagCount();
    int GetMineCount();
    void SetAllNeighbors();
    void SetNeighborNumbers();
    void ToggleDebug();
    void SetGameOver();
    void RevealTile(Tile* tile);
    void ToggleFlag(Tile* tile);
    void TogglePause();
    vector<string> loadLeaderboard();
    bool addToLeaders(string name, double time);
};


