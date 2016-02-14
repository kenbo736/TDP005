/* Game.h */
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "./Graphics/Graphics.h"
#include "./System/Logic.h"
#include "./System/Input.h"

#include "../World/World.h"

class Game
{
public:
  Game();
  ~Game();
  void run();

private:
  SDL_Window* window;
  Graphics* graphics;
  Logic* logic;
  Input* input;
  World* world;

  int fps;
};
