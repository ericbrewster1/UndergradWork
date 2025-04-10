#include "Board.h"

Board::Board()
{
    width = 22;
    height = 16;
    mineCount = 0;
}

void Board::readBoard()
{
    ifstream someFile("files/board_config.cfg");
    vector<int> boardStats;
    if(someFile.is_open())
    {
        while(boardStats.size() < 3)
        {
            string temp;
            getline(someFile, temp);
            boardStats.push_back(stoi(temp));
        }
    }
    width = boardStats[0];
    height = boardStats[1];
    mineCount = boardStats[2];

    vector<int> randomMines;
    randomMines.push_back(Random::mineSpots(0, width * height));
    while (randomMines.size() < mineCount)
    {
        int x = Random::mineSpots(0,width * height);
        bool temp = false;
        for (unsigned int i = 0; i < randomMines.size(); i++)
        {
            if (x == randomMines.at(i))
            {
                temp = true;
                break;
            }
        }
        if (!temp)
            randomMines.push_back(x);
    }

    gameOver = false;
    victory = false;
    board.clear();
    mineCount = 0;
    flagCount = 0;

    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            sf::Vector2f position = sf::Vector2f((float)(j) * 32, (float)(i) * 32);
            Tile::SecretState sec_state = Tile::SecretState::EMPTY;
            Tile temp = Tile(position, "files/images/tile_hidden.png", sec_state);
            board.push_back(temp);
        }
    }

    for (unsigned int i = 0; i < randomMines.size(); i++)
    {
        board.at(randomMines.at(i)).SetSecretState(Tile::SecretState::MINE);
        mineCount++;
        if (debug)
            board.at(randomMines.at(i)).SetSprite("files/images/mine.png");
    }

    SetAllNeighbors();
    SetNeighborNumbers();
}

Tile& Board::GetTile(unsigned int tile)
{
    return board.at(tile);
}

unsigned int Board::GetWidth()
{
    return width;
}

unsigned int Board::GetHeight()
{
    return height;
}

unsigned int Board::GetSize()
{
    return width * height;
}

void Board::ToggleDebug()
{
    if (debug)
        debug = false;
    else
        debug = true;
    if (!gameOver && !victory)
    {
        for (unsigned int i = 0; i < GetSize(); i++)
        {
            if (debug && board.at(i).GetSecretState() == Tile::SecretState::MINE)
                board.at(i).SetSprite("files/images/mine.png");
            else if (!debug && board.at(i).GetState() == Tile::State::FLAGGED)
                board.at(i).SetSprite("files/images/flag.png");
            else if (!debug && board.at(i).GetSecretState() == Tile::SecretState::MINE)
                board.at(i).SetSprite("files/images/tile_hidden.png");
        }
    }
}


bool Board::GetGameOver()
{
    return gameOver;
}

void Board::SetGameOver()
{
    gameOver = true;

    for (unsigned int i = 0; i < board.size(); i++)
    {
        board.at(i).SetClickable(false);
        board.at(i).SetRightClickable(false);
        if (board.at(i).GetSecretState() == Tile::SecretState::MINE)
        {
            board.at(i).SetSprite("files/images/mine.png");
        }
    }
}

void Board::SetAllNeighbors()
{
    for (unsigned int i = 0; i < board.size(); i++)
    {
        if (i == 0)
        {
            board.at(i).AddNeighbor(&board.at(1));
            board.at(i).AddNeighbor(&board.at(width));
            board.at(i).AddNeighbor(&board.at(width + 1));
        }
        else if (i == width - 1)
        {
            board.at(i).AddNeighbor(&board.at(i - 2));
            board.at(i).AddNeighbor(&board.at(i * 2));
            board.at(i).AddNeighbor(&board.at((i * 2) + 1));
        }
        else if (i == (width * height) - width) {
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width + 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
        }
        else if (i == (width * height) - 1)
        {
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width - 1));
            board.at(i).AddNeighbor(&board.at(i - 1));
        }
        else if (i > 0 && i < width - 1)
        {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + width - 1));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i + width + 1));
        }
        else if (i > (width * height) - width && i < (width * height) - 1)
        {
            board.at(i).AddNeighbor(&board.at(i - width - 1));
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width + 1));
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
        }
        else if (i % width == 0)
        {
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + 1 - width));
            board.at(i).AddNeighbor(&board.at(i + 1 + width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - width));
        }
        else if ((i + 1) % width == 0)
        {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - 1 - width));
            board.at(i).AddNeighbor(&board.at(i - 1 + width));
        }

        else
        {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i - 1 + width));
            board.at(i).AddNeighbor(&board.at(i - 1 - width));
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + 1 + width));
            board.at(i).AddNeighbor(&board.at(i + 1 - width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - width));
        }
    }
}

void Board::SetNeighborNumbers()
{
    for (unsigned int i = 0; i < board.size(); i++)
    {
        if (board.at(i).GetSecretState() != Tile::SecretState::MINE)
        {
            int count = 0;
            for (unsigned int j = 0; j < board.at(i).GetNeighborCount(); j++)
            {
                Tile* t = board.at(i).GetNeighbor(j);
                if (t->GetSecretState() == Tile::SecretState::MINE)
                    count++;
            }

            if (count == 1)
                board.at(i).SetSecretState(Tile::SecretState::ONE);
            else if (count == 2)
                board.at(i).SetSecretState(Tile::SecretState::TWO);
            else if (count == 3)
                board.at(i).SetSecretState(Tile::SecretState::THREE);
            else if (count == 4)
                board.at(i).SetSecretState(Tile::SecretState::FOUR);
            else if (count == 5)
                board.at(i).SetSecretState(Tile::SecretState::FIVE);
            else if (count == 6)
                board.at(i).SetSecretState(Tile::SecretState::SIX);
            else if (count == 7)
                board.at(i).SetSecretState(Tile::SecretState::SEVEN);
            else if (count == 8)
                board.at(i).SetSecretState(Tile::SecretState::EIGHT);

            else
                board.at(i).SetSecretState(Tile::SecretState::EMPTY);
        }
    }
}

void Board::RevealTile(Tile* tile)
{
    bool end = tile->Reveal();
    if (end)
        SetGameOver();
}

void Board::ToggleFlag(Tile* tile)
{
    tile->ToggleFlag(debug);
}

bool Board::GetVictory()
{
    int count = 0;
    for (unsigned int i = 0; i < GetSize(); i++)
    {
        if (board.at(i).GetState() == Tile::State::REVEALED)
            count++;
    }
    if (GetSize() - count - mineCount == 0)
    {
        for (unsigned int i = 0; i < board.size(); i++)
        {
            board.at(i).SetClickable(false);
            board.at(i).SetRightClickable(false);
            if (board.at(i).GetSecretState() == Tile::SecretState::MINE)
            {
                board.at(i).SetSprite("files/images/flag.png");
            }
        }
        victory = true;
        return true;
    }
    return false;
}

int Board::GetFlagCount()
{
    flagCount = 0;
    for (unsigned int i = 0; i < GetSize(); i++)
    {
        if (board.at(i).GetState() == Tile::State::FLAGGED)
            flagCount++;
    }
    return flagCount;
}

int Board::GetMineCount()
{
    return mineCount;
}

void Board::TogglePause()
{
    if (pause)
        pause = false;
    else
        pause = true;
    if (!gameOver && !victory) {
        for (unsigned int i = 0; i < GetSize(); i++)
        {
            if (pause)
            {
                board.at(i).SetSprite("files/images/tile_revealed.png");
                board.at(i).SetClickable(false);
                board.at(i).SetRightClickable(false);
            }
            else if (!pause && board.at(i).GetState() == Tile::State::HIDDEN)
            {
                board.at(i).SetSprite("files/images/tile_hidden.png");
                board.at(i).SetClickable(true);
                board.at(i).SetRightClickable(true);
            }
            else if (!pause && board.at(i).GetState() == Tile::State::FLAGGED)
            {
                board.at(i).SetSprite("files/images/flag.png");
                board.at(i).SetClickable(true);
                board.at(i).SetRightClickable(true);
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::EMPTY)
            {
                board.at(i).SetSprite("files/images/tile_revealed.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::ONE)
            {
                board.at(i).SetSprite("files/images/number_1.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::TWO)
            {
                board.at(i).SetSprite("files/images/number_2.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::THREE)
            {
                board.at(i).SetSprite("files/images/number_3.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::FOUR)
            {
                board.at(i).SetSprite("files/images/number_4.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::FIVE)
            {
                board.at(i).SetSprite("files/images/number_5.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::SIX)
            {
                board.at(i).SetSprite("files/images/number_6.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::SEVEN)
            {
                board.at(i).SetSprite("files/images/number_7.png");
            }
            else if (!pause && board.at(i).GetState() == Tile::State::REVEALED && board.at(i).GetSecretState() == Tile::SecretState::EIGHT)
            {
                board.at(i).SetSprite("files/images/number_8.png");
            }
        }
    }
}

vector<string> Board::loadLeaderboard()
{
    ifstream someFile("files/leaderboard.txt");
    vector<string> leaders;
    string temp;
    int count  = 0;
    if(someFile.is_open())
    {
        while(count < 5)
        {
            getline(someFile, temp);
            leaders.push_back(temp);
            count += 1;
        }
    }
    someFile.close();
    return leaders;
}

bool Board::addToLeaders(string name, double time)
{
    int minutes = int(time) / 60;
    int seconds = int(time) % 60;
    string minutesNew;
    if(minutes < 10)
        minutesNew = "0" + to_string(minutes);
    else
        minutesNew = to_string(minutes);
    string secondsNew;
    if(seconds < 10)
        secondsNew = "0" + to_string(seconds);
    else
        secondsNew = to_string(seconds);
    string stringTime;
    stringTime += minutesNew + ":" + secondsNew;
    string insertToLeaders = stringTime + "," + name;

    vector<string> leaders = Board::loadLeaderboard();
    leaders.push_back(insertToLeaders);
    sort(leaders.begin(), leaders.end());

    ofstream myFile("files/leaderboard.txt");
    if(myFile.is_open())
    {
        for(int i = 0; i < leaders.size(); i++)
            myFile << leaders.at(i) << "\n";
    }
    myFile.close();
    return true;
}

