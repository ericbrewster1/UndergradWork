#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Board.h"
#include "Counter.h"
using namespace std;

void setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

string welcomeWindow()
{
    Board board = Board();
    board.readBoard();
    string username;

    sf::RenderWindow welcomeWindow(sf::VideoMode(board.GetWidth() * 32, board.GetHeight() * 32 + 100), "Minesweeper", sf::Style::Close);
    sf::RectangleShape background(sf::Vector2f(board.GetWidth() * 32, board.GetHeight() * 32 + 100));
    background.setFillColor(sf::Color::Blue);
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text welcomeMessage;
    welcomeMessage.setFont(font);
    welcomeMessage.setString("Welcome to Minesweeper!");
    welcomeMessage.setCharacterSize(24);
    welcomeMessage.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeMessage, board.GetWidth() * 32 / 2.0f, board.GetHeight() * 32 / 2.0f - 150);

    sf::Text inputMessage;
    inputMessage.setFont(font);
    inputMessage.setString("Enter your name:");
    inputMessage.setCharacterSize(20);
    inputMessage.setStyle(sf::Text::Bold);
    setText(inputMessage, board.GetWidth() * 32 / 2.0f, board.GetHeight() * 32 / 2.0f - 75);

    sf::String playerInput;
    sf::Text input;
    input.setFont(font);
    input.setFillColor(sf::Color::Yellow);
    input.setCharacterSize(18);
    input.setStyle(sf::Text::Bold);
    input.setString("|");
    setText(input, board.GetWidth() * 32 / 2.0f, board.GetHeight() * 32 / 2.0f - 45);

    int counter = 0;

    while(welcomeWindow.isOpen())
    {
        sf::Event event;
        while (welcomeWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                welcomeWindow.close();
                return " ";
            }
            else if(event.type == sf::Event::TextEntered)
            {
                if(counter == 0)
                {
                    if (event.text.unicode > 64 && event.text.unicode < 91)
                    {
                        playerInput.erase(counter);
                        playerInput += event.text.unicode;
                        playerInput += "|";
                        input.setString(playerInput);
                        counter += 1;
                    }
                    else if (event.text.unicode > 96 && event.text.unicode < 123)
                    {
                        playerInput.erase(counter);
                        playerInput += event.text.unicode - 32;
                        playerInput += "|";
                        input.setString(playerInput);
                        counter += 1;
                    }
                }
                else if(counter < 10 && counter != 0)
                {
                    if (event.text.unicode > 64 && event.text.unicode < 91)
                    {
                        playerInput.erase(counter);
                        playerInput += event.text.unicode + 32;
                        playerInput += "|";
                        input.setString(playerInput);
                        counter += 1;
                    }
                    else if(event.text.unicode > 96 && event.text.unicode < 123)
                    {
                        playerInput.erase(counter);
                        playerInput += event.text.unicode;
                        playerInput += "|";
                        input.setString(playerInput);
                        counter += 1;
                    }
                }
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Enter)
                {
                    if(counter > 0)
                    {
                        username = playerInput;
                        welcomeWindow.close();
                        username[0] = toupper(username[0]);
                        for(int i = 1; i < username.size(); i++)
                            username[i] = tolower(username[i]);
                        return username;
                    }
                }
                else if(event.key.code == sf::Keyboard::BackSpace)
                {
                    if(playerInput.getSize() >= 2)
                    {
                        playerInput.erase(counter - 1, 1);
                        counter -= 1;
                        input.setString(playerInput);
                    }
                }
            }
        }
        welcomeWindow.clear();
        welcomeWindow.draw(background);
        welcomeWindow.draw(welcomeMessage);
        welcomeWindow.draw(inputMessage);
        welcomeWindow.draw(input);
        welcomeWindow.display();
    }
    return " ";
}

int main()
{
    string username = welcomeWindow();
    size_t cursor = username.find('|');
    string correctUser = username.substr(0, cursor);
    if(username != " ")
    {
        Board board = Board();
        board.readBoard();
        sf::Clock clock;

        sf::RenderWindow window(sf::VideoMode(board.GetWidth() * 32, board.GetHeight() * 32 + 100), "Minesweeper",
                                sf::Style::Close | sf::Style::Titlebar);

        sf::RectangleShape background(sf::Vector2f(board.GetWidth() * 32, board.GetHeight() * 32 + 100));
        background.setFillColor(sf::Color(200, 200, 200));

        sf::Sprite debugButton(TextureManager::GetTexture("files/images/debug.png"));
        debugButton.setPosition(((board.GetWidth()) * 32) - 304, 32 * ((board.GetHeight()) + 0.5f));

        sf::Sprite pauseButton(TextureManager::GetTexture("files/images/pause.png"));
        pauseButton.setPosition(((board.GetWidth()) * 32) - 240, 32 * ((board.GetHeight()) + 0.5f));

        sf::Sprite playButton(TextureManager::GetTexture("files/images/play.png"));
        playButton.setPosition(((board.GetWidth()) * 32) - 240, 32 * ((board.GetHeight()) + 0.5f));

        sf::Sprite leaderboardButton(TextureManager::GetTexture("files/images/leaderboard.png"));
        leaderboardButton.setPosition(((board.GetWidth()) * 32) - 176, 32 * ((board.GetHeight()) + 0.5f));
        bool showLeaders = false;
        bool paused = false;
        int countMoves = 0;
        int gamesPlayed = 0;
        int timesStarted = 0;
        double finalTime;
        int timesAdded = 0;

        sf::Sprite smiley(TextureManager::GetTexture("files/images/face_happy.png"));
        smiley.setPosition((((board.GetWidth()) / 2.0) * 32) - 32, 32 * ((board.GetHeight()) + 0.5f));

        sf::Texture digitTexture = TextureManager::GetTexture("files/images/digits.png");
        vector<sf::IntRect> digitRects;
        for (int i = 0; i < 211; i += 21)
            digitRects.push_back(sf::IntRect(i, 0, 21, 32));
        vector<int> nums = Counter::digitizer(board.GetMineCount());
        vector<sf::Sprite> digitDisplay;
        for (int i = 0, j = 0; i < 3; i++)
        {
            digitDisplay.push_back(sf::Sprite(digitTexture));
            digitDisplay.at(i).setPosition(33 + j, 32 * (board.GetHeight() + 0.5f) + 16);
            digitDisplay.at(i).setTextureRect(digitRects.at(nums.at(i)));
            j += 21;
        }

        chrono::time_point<chrono::system_clock> start, end, current, pause, unpause;
        vector<chrono::time_point<chrono::system_clock>> pauseDurations;
        int numPauses = 0;
        double time;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(timesStarted == 0)
                {
                    start = std::chrono::system_clock::now();
                    timesStarted += 1;
                }
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2i position = sf::Mouse::getPosition(window);
                        if (position.y < board.GetHeight() * 32)
                        {
                            Tile *currentSpot = &board.GetTile(board.GetWidth() * (position.y / 32) + (position.x / 32));
                            board.RevealTile(currentSpot);
                            countMoves += 1;
                            board.GetGameOver();
                            if (board.GetVictory())
                            {
                                end = std::chrono::system_clock::now();
                                smiley.setTexture(TextureManager::GetTexture("files/images/face_win.png"));
                                for (int i = 0; i < 3; i++)
                                {
                                    nums.at(i) = 0;
                                    digitDisplay.at(i).setTextureRect(digitRects.at(nums.at(i)));
                                }
                                countMoves = 0;
                                gamesPlayed += 1;
                            }
                            if (board.GetGameOver())
                            {
                                end = std::chrono::system_clock::now();
                                smiley.setTexture(TextureManager::GetTexture("files/images/face_lose.png"));
                                countMoves = 0;
                                gamesPlayed += 1;
                            }
                        }
                        else if ((position.x > board.GetWidth() * 32 - 304 &&
                                 position.x < board.GetWidth() * 32 - 240) &&
                                 (position.y > board.GetHeight() * 32 + 18 && position.y < board.GetHeight() * 32 + 82))
                            board.ToggleDebug();

                        else if ((position.x > board.GetWidth() / 2.0 * 32 - 32 &&
                                  position.x < board.GetWidth() / 2.0 * 32 + 32) &&
                                 (position.y > board.GetHeight() * 32 + 18 &&
                                  position.y < board.GetHeight() * 32 + 82))
                        {
                            if(gamesPlayed != 0)
                                start = std::chrono::system_clock::now();
                            board.readBoard();
                            smiley.setTexture(TextureManager::GetTexture("files/images/face_happy.png"));
                            nums = Counter::digitizer(board.GetMineCount() - board.GetFlagCount());
                            for (int i = 0; i < 3; i++)
                                digitDisplay.at(i).setTextureRect(digitRects.at(nums.at(i)));
                        }
                        else if ((position.x > board.GetWidth() * 32 - 240 &&
                                   position.x < board.GetWidth() * 32 - 176) &&
                                   (position.y > board.GetHeight() * 32 + 18 &&
                                    position.y < board.GetHeight() * 32 + 82))
                        {
                            if(!board.GetVictory() && !board.GetGameOver())
                            {
                                board.TogglePause();
                                if(paused)
                                {
                                    paused = false;
                                    pause = std::chrono::system_clock::now();
                                }
                                else
                                {
                                    paused = true;
                                }
                            }

                        }
                        else if ((position.x > board.GetWidth() * 32 - 176 &&
                                   position.x < board.GetWidth() * 32 - 112) &&
                                   (position.y > board.GetHeight() * 32 + 18 &&
                                    position.y < board.GetHeight() * 32 + 82))
                        {
                            if (showLeaders)
                                showLeaders = false;
                            else
                                showLeaders = true;
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        sf::Vector2i position = sf::Mouse::getPosition(window);
                        if (position.y < board.GetHeight() * 32) {
                            Tile *currentSpot = &board.GetTile(
                                    board.GetWidth() * (position.y / 32) + (position.x / 32));
                            board.ToggleFlag(currentSpot);
                            nums = Counter::digitizer(board.GetMineCount() - board.GetFlagCount());
                            for (int i = 0; i < 3; i++)
                                digitDisplay.at(i).setTextureRect(digitRects.at(nums.at(i)));
                        }
                    }
                }
            }
            window.clear();
            window.draw(background);
            window.draw(debugButton);
            if (paused)
                window.draw(playButton);
            else
                window.draw(pauseButton);
            window.draw(leaderboardButton);
            window.draw(smiley);
            if(showLeaders)
            {
                sf::RenderWindow leaderboardWindow(sf::VideoMode(board.GetWidth() * 16, board.GetHeight() * 16 + 50), "Minesweeper",sf::Style::Close);
                sf::RectangleShape background2(sf::Vector2f(board.GetWidth() * 16, board.GetHeight() * 16 + 50));
                background2.setFillColor(sf::Color::Blue);
                sf::Font font;
                font.loadFromFile("files/font.ttf");

                sf::Text leaderMessage;
                leaderMessage.setFont(font);
                leaderMessage.setString("Leaderboard");
                leaderMessage.setCharacterSize(20);
                leaderMessage.setStyle(sf::Text::Bold | sf::Text::Underlined);
                setText(leaderMessage, board.GetWidth() * 16 / 2.0f, board.GetHeight() * 16 / 2.0f - 120);

                vector<string> leaders = board.loadLeaderboard();
                string leads;
                leads = "1.\t" + leaders[0].substr(0, 5) + "\t" + leaders[0].substr(6, leaders[0].size() - 1) + "\n\n"
                        + "2.\t" + leaders[1].substr(0, 5) + "\t" + leaders[1].substr(6, leaders[1].size() - 1) + "\n\n"
                        + "3.\t" + leaders[2].substr(0, 5) + "\t" + leaders[2].substr(6, leaders[2].size() - 1) + "\n\n"
                        + "4.\t" + leaders[3].substr(0, 5) + "\t" + leaders[3].substr(6, leaders[3].size() - 1) + "\n\n"
                        + "5.\t" + leaders[4].substr(0, 5) + "\t" + leaders[4].substr(6, leaders[4].size() - 1);
                sf::Text numOne;
                numOne.setFont(font);
                numOne.setString(leads);
                numOne.setCharacterSize(20);
                numOne.setStyle(sf::Text::Bold);
                setText(numOne, board.GetWidth() * 16 / 2.0f, board.GetHeight() * 16 / 2.0f);

                while(leaderboardWindow.isOpen())
                {
                    sf::Event event2;

                    while (leaderboardWindow.pollEvent(event2))
                    {
                        if (event2.type == sf::Event::Closed)
                        {
                            leaderboardWindow.close();
                        }
                    }
                    leaderboardWindow.clear();
                    leaderboardWindow.draw(background2);
                    leaderboardWindow.draw(leaderMessage);
                    leaderboardWindow.draw(numOne);
                    leaderboardWindow.display();
                }
                showLeaders = false;
            }
            for (int i = 0; i < 3; i++)
                window.draw(digitDisplay.at(i));

            vector<int> displayTime;
            vector<int> pauseTime;
            current = std::chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds;
            chrono::duration<double> totalPauseTime;
            elapsed_seconds = current - start;
            totalPauseTime = pause - start;
            displayTime = Counter::displayTimer(elapsed_seconds.count());
            pauseTime = Counter::displayTimer(totalPauseTime.count());

            vector<int> endTime;
            chrono::duration<double> total_seconds = end - start;
            endTime = Counter::displayTimer(total_seconds.count());
            if(board.GetVictory() && timesAdded - timesStarted == -1)
            {
                board.addToLeaders(correctUser, total_seconds.count());
                timesAdded += 1;
            }


            vector<sf::Sprite> timerDisplay;
            for (int x = 0, y = 0, z = 0; x < 4; x++)
            {
                timerDisplay.push_back(sf::Sprite(digitTexture));
                timerDisplay.at(x).setPosition((board.GetWidth() * 32) - 97 + y, 32 * (board.GetHeight() + 0.5f) + 16);
                if(board.GetGameOver() || board.GetVictory())
                    timerDisplay.at(x).setTextureRect(digitRects.at(endTime.at(x)));
                else
                    timerDisplay.at(x).setTextureRect(digitRects.at(displayTime.at(x)));
                y += 21;
                z += 1;
                if(z == 2)
                    y += 1;
            }

            for(unsigned int j = 0; j < 4; j++)
                window.draw(timerDisplay.at(j));
            for (unsigned int i = 0; i < board.GetSize(); i++)
            {
                window.draw(board.GetTile(i).GetSprite());
            }
            window.display();
        }
        TextureManager::Clear();
        return 0;
    }
}