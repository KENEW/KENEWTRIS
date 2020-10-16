#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <time.h>
#include <string>
#include <iostream>

#include<fmod.h>

using namespace std;

const int scaleSet = 18 * 3;
const int fieldOffX = 10 * 3;
const int fieldOffY = 31 * 3;
const float zoomNum = (float)(1 * 3);

const int GRID_HEIGHT = 20;
const int GRID_WIDTH = 10;
const int SCREEN_WIDTH = 860;
const int SCREEN_HEIGHT = 1420;

struct Point
{
    int x, y;
}place[4], b[4];

int Piece[][4] =
{
    {3,5,4,7}, //T
    {1,3,5,7}, //I
    {3,5,4,6}, //S
    {3,5,7,6}, //J
    {2,3,4,5}, //O
    {2,4,5,7}, //Z
    {2,3,5,7}, //L
};

class Game
{
public:
    
};
bool WallsCheck();
void Rotate();
void CreatePlace();
void FMODSetting();
void Tick();
void CheckLine();
void Move();

FMOD_SYSTEM* g_System;  // FMOD system 변수 선언
FMOD_SOUND* bgm;
FMOD_SOUND* effect[2];
FMOD_CHANNEL* channel;
FMOD_CHANNEL* efxChannel[2];