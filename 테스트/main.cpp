#include "TetrisLib.h"

using namespace sf;

int field[GRID_HEIGHT][GRID_WIDTH] = { 0 };

int score = 0;
int level = 0;

int curX = 0;
int colorNum = 1;

float timer = 0, delay = 0.3;
void Rotate()
{
    Point p = place[1];
    for (int i = 0; i < 4; i++)
    {
        int x = place[i].y - p.y;
        int y = place[i].x - p.x;
        place[i].x = p.x - x;
        place[i].y = p.y + y;
    }
    if (!WallsCheck())
    {
        for (int i = 0; i < 4; i++)
        {
            place[i] = b[i];
        }
    }
}

void Move(int curX)
{
    for (int i = 0; i < 4; i++)
    {
        b[i] = place[i];
        place[i].x += curX;
    }
    if (!WallsCheck())
    {
        for (int i = 0; i < 4; i++)
        {
            place[i] = b[i];
        }
    }
}

void CreatePlace()
{
    int n = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        place[i].x = Piece[n][i] % 2;
        place[i].y = Piece[n][i] / 2;
    }
}

bool WallsCheck()
{
    for (int i = 0; i < 4; i++)
    {
        if (place[i].x < 0 || place[i].x >= GRID_WIDTH || place[i].y >= GRID_HEIGHT)
        {
            return 0;
        }
        else if (field[place[i].y][place[i].x])
        {
            return 0;
        }
    }
    return 1;
}

void CheckLine()
{
    int tHeight = GRID_HEIGHT - 1;

    for (int i = GRID_HEIGHT - 1; i > 0; i--)
    {
        int count = 0;

        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (field[i][j])
            {
                count++;
            }

            field[tHeight][j] = field[i][j];
        }
        if (count < GRID_WIDTH) //한줄이 완성시
        {
            tHeight--;
  
        }
        else
        {
            score++;
            FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, effect[1], 0, &efxChannel[1]);
        }
    }
}

void Tick()
{
    if (timer > delay)
    {
        //블록 4칸을 밑으로 내려보낸다.
        for (int i = 0; i < 4; i++)
        {
            b[i] = place[i];
            place[i].y += 1;
        }
        if (!WallsCheck())
        {
            colorNum = 1 + rand() % 7;
            for (int i = 0; i < 4; i++)
            {
                field[b[i].y][b[i].x] = colorNum;

            }
            int n = rand() % 7;

            for (int i = 0; i < 4; i++)
            {
                place[i].x = Piece[n][i] % 2;
                place[i].y = Piece[n][i] / 2;
            }
        }
        timer = 0;
    }
}

void FMODSetting()
{
    FMOD_System_Create(&g_System);
    FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);

    FMOD_System_CreateSound(g_System, "music/Voyage.mp3", FMOD_DEFAULT, 0, &bgm);
    FMOD_System_CreateSound(g_System, "music/BlockDown.wav", FMOD_DEFAULT, 0, &effect[0]);
    FMOD_System_CreateSound(g_System, "music/LineEfx.wav", FMOD_DEFAULT, 0, &effect[1]);
}

int main()
{
    string scoreStr;
    Clock clock;
    sf::Font font;

    FMODSetting();
    FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, bgm, 0, &channel); //bgm 재생

    srand(time(0));
    if (!font.loadFromFile("Coiny-Regular.ttf"))
    {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "KENEWTRIS", sf::Style::Default);
    sf::Texture tilesTexture, backgroundTexture, forgroundTexture;
    window.setFramerateLimit(60);

    if (!tilesTexture.loadFromFile("tiles.png"))
        exit(1);
    if (!backgroundTexture.loadFromFile("background1.png"))
        exit(1);

    sf::Sprite sprite(tilesTexture), background(backgroundTexture);
    sprite.scale(sf::Vector2f(zoomNum, zoomNum));
    background.scale(Vector2f(zoomNum, zoomNum));
    background.setPosition(34, 100);

    sf::RectangleShape bgRect{ { SCREEN_WIDTH, SCREEN_HEIGHT } };
    bgRect.setFillColor(sf::Color(89, 89, 89));
    bgRect.setPosition({ 0, 0 });

    sf::Text scoreTextConst{ "SCORE : ", font, 35 };
    scoreTextConst.setPosition({ 600, 600 });
    scoreTextConst.setStyle(sf::Text::Bold);

    sf::Text TetrisName{ "KENEWTRIS", font, 35 };
    TetrisName.setPosition({ 600, 500 });
    TetrisName.setStyle(sf::Text::Bold);

    CreatePlace();
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                    Rotate();
                else if (event.key.code == Keyboard::Left)
                    curX = -1;
                else if (event.key.code == Keyboard::Right)
                    curX = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            delay = 0.05;
        }

        Move(curX);
        Tick();
        CheckLine();
        
        curX = 0;
        delay = 0.3;
     
        window.draw(bgRect);
        window.draw(background);
        for (int i = 0; i < GRID_HEIGHT; i++)
        {
            for (int j = 0; j < GRID_WIDTH; j++)
            {
                if (field[i][j] == 0)
                {
                    continue;
                }
                sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                sprite.setPosition(j * scaleSet, i * scaleSet);
                sprite.move(fieldOffX, fieldOffY);
                window.draw(sprite);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            sprite.setPosition(place[i].x * scaleSet, place[i].y * scaleSet);
            sprite.move(fieldOffX, fieldOffY); //offset

            
            window.draw(sprite);
        }

        scoreStr = to_string(score);
        sf::Text scoreText{ scoreStr, font, 30 };
        scoreText.setPosition({ 780, 605 });
        scoreText.setStyle(sf::Text::Bold);
        window.draw(TetrisName);
        window.draw(scoreTextConst);
        window.draw(scoreText);

        window.display();
    }

    return EXIT_SUCCESS;
}