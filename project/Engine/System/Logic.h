/* Logic.h */
#pragma once
#include <SDL2/SDL.h>
#include "../../World/World.h"
#include <utility>

class Logic
{
public:
  Logic(SDL_Window* window, int fps);
  ~Logic();
  void think(World& world);

private:
  //std::pair<bool, bool> worldBoundary(World& world, Sprite* sprite, double deltatime);
  void move_sprites(World& world, double deltatime);
  void apply_gravity_and_friction(World& world, double deltatime);
  void remove_old_sprites(World& world);
  SDL_Window* window;
  unsigned int last_time;
  int fps;
  int gravity;
  int bottom;
};
